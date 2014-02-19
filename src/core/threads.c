#include "moar.h"
#include <platform/threads.h>

/* Temporary structure for passing data to thread start. */
typedef struct {
    MVMThreadContext *tc;
    MVMObject        *thread_obj;
    MVMCallsite       no_arg_callsite;
} ThreadStart;

/* Creates a new thread handle with the MVMThread representation. Does not
 * actually start execution of the thread. */
MVMObject * MVM_thread_new(MVMThreadContext *tc, MVMObject *invokee, MVMint64 app_lifetime) {
    MVMThread *thread;
    MVMROOT(tc, invokee, {
        thread = (MVMThread *)MVM_repr_alloc_init(tc, tc->instance->Thread);
    });
    thread->body.stage = MVM_thread_stage_unstarted;
    MVM_ASSIGN_REF(tc, &(thread->common.header), thread->body.invokee, invokee);
    thread->body.app_lifetime = app_lifetime;
    return (MVMObject *)thread;
}

/* This callback is passed to the interpreter code. It takes care of making
 * the initial invocation of the thread code. */
static void thread_initial_invoke(MVMThreadContext *tc, void *data) {
    /* The passed data is simply the code object to invoke. */
    ThreadStart *ts = (ThreadStart *)data;
    MVMThread *thread = (MVMThread *)ts->thread_obj;
    MVMObject *invokee = thread->body.invokee;
    thread->body.invokee = NULL;

    /* Set up the cached current usecapture CallCapture (done here so
     * we allocate it on the correct thread, and once the thread is
     * active). */
    tc->cur_usecapture = MVM_repr_alloc_init(tc, tc->instance->CallCapture);

    /* Dummy, 0-arg callsite. */
    ts->no_arg_callsite.arg_flags      = NULL;
    ts->no_arg_callsite.arg_count      = 0;
    ts->no_arg_callsite.num_pos        = 0;
    ts->no_arg_callsite.has_flattening = 0;

    /* Create initial frame, which sets up all of the interpreter state also. */
    invokee = MVM_frame_find_invokee(tc, invokee, NULL);
    STABLE(invokee)->invoke(tc, invokee, &ts->no_arg_callsite, NULL);

    /* This frame should be marked as the thread entry frame, so that any
     * return from it will cause us to drop out of the interpreter and end
     * the thread. */
    tc->thread_entry_frame = tc->cur_frame;
}

/* This callback handles starting execution of a thread. */
static void start_thread(void *data) {
    ThreadStart *ts = (ThreadStart *)data;
    MVMThreadContext *tc = ts->tc;

    /* Stash thread ID. */
    tc->thread_obj->body.thread_id = uv_thread_self();

    /* wait for the GC to finish if it's not finished stealing us. */
    MVM_gc_mark_thread_unblocked(tc);
    tc->thread_obj->body.stage = MVM_thread_stage_started;

    /* Enter the interpreter, to run code. */
    MVM_interp_run(tc, &thread_initial_invoke, ts);

    /* mark as exited, so the GC will know to clear our stuff. */
    tc->thread_obj->body.stage = MVM_thread_stage_exited;

    /* Mark ourselves as dying, so that another thread will take care
     * of GC-ing our objects and cleaning up our thread context. */
    MVM_gc_mark_thread_blocked(tc);

    /* hopefully pop the ts->thread_obj temp */
    MVM_gc_root_temp_pop(tc);
    free(ts);

    /* Exit the thread, now it's completed. */
    MVM_platform_thread_exit(NULL);
}

/* Begins execution of a thread. */
void MVM_thread_run(MVMThreadContext *tc, MVMObject *thread_obj) {
    MVMThread *child = (MVMThread *)thread_obj;
    int status;
    ThreadStart *ts;

    if (REPR(child)->ID == MVM_REPR_ID_MVMThread) {
        MVMThread * volatile *threads;
        MVMThreadContext *child_tc;

        /* Move thread to starting stage. */
        child->body.stage = MVM_thread_stage_starting;

        /* Create a new thread context and set it up. */
        child_tc = MVM_tc_create(tc->instance);
        child->body.tc = child_tc;
        child_tc->thread_obj = child;
        child_tc->thread_id = MVM_incr(&tc->instance->next_user_thread_id);

        /* Create thread state, to pass to the thread start callback. */
        ts = malloc(sizeof(ThreadStart));
        ts->tc = child_tc;
        ts->thread_obj = thread_obj;

        /* Push this to the *child* tc's temp roots. */
        MVM_gc_root_temp_push(child_tc, (MVMCollectable **)&ts->thread_obj);

        /* Signal to the GC we have a childbirth in progress. The GC
         * will null it for us. */
        MVM_gc_mark_thread_blocked(child_tc);
        MVM_ASSIGN_REF(tc, &(tc->thread_obj->common.header), tc->thread_obj->body.new_child, child);

        /* Push to starting threads list */
        threads = &tc->instance->threads;
        do {
            MVMThread *curr = *threads;
            MVM_ASSIGN_REF(tc, &(child->common.header), child->body.next, curr);
        } while (MVM_casptr(threads, child->body.next, child) != child->body.next);

        /* Do the actual thread creation. */
        status = uv_thread_create(&child->body.thread, &start_thread, ts);
        if (status < 0)
            MVM_panic(MVM_exitcode_compunit, "Could not spawn thread: errorcode %d", status);

        /* Need to run the GC to clear our new_child field in case we try
         * try to launch another thread before the GC runs and before the
         * thread starts. */
        GC_SYNC_POINT(tc);
    }
    else {
        MVM_exception_throw_adhoc(tc,
            "Thread handle passed to run must have representation MVMThread");
    }
}

/* Waits for a thread to finish. */
void MVM_thread_join(MVMThreadContext *tc, MVMObject *thread_obj) {
    if (REPR(thread_obj)->ID == MVM_REPR_ID_MVMThread) {
        MVMThread *thread = (MVMThread *)thread_obj;
        int status;
        MVM_gc_root_temp_push(tc, (MVMCollectable **)&thread);
        MVM_gc_mark_thread_blocked(tc);
        if (((MVMThread *)thread_obj)->body.stage < MVM_thread_stage_exited) {
            status = uv_thread_join(&thread->body.thread);
        }
        else {
            /* the target already ended */
            status = 0;
        }
        MVM_gc_mark_thread_unblocked(tc);
        MVM_gc_root_temp_pop(tc);
        if (status < 0)
            MVM_panic(MVM_exitcode_compunit, "Could not join thread: errorcode %d", status);
    }
    else {
        MVM_exception_throw_adhoc(tc,
            "Thread handle passed to join must have representation MVMThread");
    }
}

/* Gets the ID of a thread. */
MVMint64 MVM_thread_id(MVMThreadContext *tc, MVMObject *thread_obj) {
    if (REPR(thread_obj)->ID == MVM_REPR_ID_MVMThread) {
        MVMThread *thread = (MVMThread *)thread_obj;
        while (MVM_load(&thread->body.stage) < MVM_thread_stage_started)
            MVM_platform_thread_yield();
        return thread->body.thread_id;
    }
    else {
        MVM_exception_throw_adhoc(tc,
            "Thread handle passed to id must have representation MVMThread");
    }
}

/* Yields control to another thread. */
void MVM_thread_yield(MVMThreadContext *tc) {
    MVM_platform_thread_yield();
}

/* Gets the object representing the current thread. */
MVMObject * MVM_thread_current(MVMThreadContext *tc) {
    return (MVMObject *)tc->thread_obj;
}

void MVM_thread_cleanup_threads_list(MVMThreadContext *tc, MVMThread **head) {
    /* Assumed to be the only thread accessing the list.
     * must set next on every item. */
    MVMThread *new_list = NULL, *this = *head, *next;
    *head = NULL;
    while (this) {
        next = this->body.next;
        switch (this->body.stage) {
            case MVM_thread_stage_starting:
            case MVM_thread_stage_waiting:
            case MVM_thread_stage_started:
            case MVM_thread_stage_exited:
            case MVM_thread_stage_clearing_nursery:
                /* push it to the new starting list */
                this->body.next = new_list;
                new_list = this;
                break;
            case MVM_thread_stage_destroyed:
                /* don't put in a list */
                this->body.next = NULL;
                break;
            default:
                MVM_panic(MVM_exitcode_threads, "Thread in unknown stage: %d\n", this->body.stage);
        }
        this = next;
    }
    *head = new_list;
}
