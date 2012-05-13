/* This file is generated from src/core/oplist by tools/update_ops_h.p6. */

/* Bank name defines. */
#define MVM_OP_BANK_primitives 0

/* Op name defines for bank primitives. */
#define MVM_OP_no_op 0
#define MVM_OP_goto 1
#define MVM_OP_if_i 2
#define MVM_OP_unless_i 3
#define MVM_OP_if_n 4
#define MVM_OP_unless_n 5
#define MVM_OP_if_s 6
#define MVM_OP_unless_s 7
#define MVM_OP_if_s0 8
#define MVM_OP_unless_s0 9
#define MVM_OP_if_o 10
#define MVM_OP_unless_o 11
#define MVM_OP_set 12
#define MVM_OP_extend_u8 13
#define MVM_OP_extend_u16 14
#define MVM_OP_extend_u32 15
#define MVM_OP_extend_i8 16
#define MVM_OP_extend_i16 17
#define MVM_OP_extend_i32 18
#define MVM_OP_trunc_u8 19
#define MVM_OP_trunc_u16 20
#define MVM_OP_trunc_u32 21
#define MVM_OP_trunc_i8 22
#define MVM_OP_trunc_i16 23
#define MVM_OP_trunc_i32 24
#define MVM_OP_extend_n32 25
#define MVM_OP_trunc_n32 26
#define MVM_OP_get_lex 27
#define MVM_OP_bind_lex 28
#define MVM_OP_get_lex_lo 29
#define MVM_OP_bind_lex_lo 30
#define MVM_OP_get_lex_ni 31
#define MVM_OP_get_lex_nn 32
#define MVM_OP_get_lex_ns 33
#define MVM_OP_get_lex_no 34
#define MVM_OP_bind_lex_ni 35
#define MVM_OP_bind_lex_nn 36
#define MVM_OP_bind_lex_ns 37
#define MVM_OP_bind_lex_no 38
#define MVM_OP_return_i 39
#define MVM_OP_return_n 40
#define MVM_OP_return_s 41
#define MVM_OP_return_o 42
#define MVM_OP_return 43

static MVMOpInfo MVM_op_info_primitives[] = {
    {
        MVM_OP_no_op,
        "no_op",
        0,
    },
    {
        MVM_OP_goto,
        "goto",
        1,
        { MVM_operand_ins }
    },
    {
        MVM_OP_if_i,
        "if_i",
        2,
        { MVM_operand_read_reg | MVM_operand_int64, MVM_operand_ins }
    },
    {
        MVM_OP_unless_i,
        "unless_i",
        2,
        { MVM_operand_read_reg | MVM_operand_int64, MVM_operand_ins }
    },
    {
        MVM_OP_if_n,
        "if_n",
        2,
        { MVM_operand_read_reg | MVM_operand_num64, MVM_operand_ins }
    },
    {
        MVM_OP_unless_n,
        "unless_n",
        2,
        { MVM_operand_read_reg | MVM_operand_num64, MVM_operand_ins }
    },
    {
        MVM_OP_if_s,
        "if_s",
        2,
        { MVM_operand_read_reg | MVM_operand_str, MVM_operand_ins }
    },
    {
        MVM_OP_unless_s,
        "unless_s",
        2,
        { MVM_operand_read_reg | MVM_operand_str, MVM_operand_ins }
    },
    {
        MVM_OP_if_s0,
        "if_s0",
        2,
        { MVM_operand_read_reg | MVM_operand_str, MVM_operand_ins }
    },
    {
        MVM_OP_unless_s0,
        "unless_s0",
        2,
        { MVM_operand_read_reg | MVM_operand_str, MVM_operand_ins }
    },
    {
        MVM_OP_if_o,
        "if_o",
        2,
        { MVM_operand_read_reg | MVM_operand_obj, MVM_operand_ins }
    },
    {
        MVM_OP_unless_o,
        "unless_o",
        2,
        { MVM_operand_read_reg | MVM_operand_obj, MVM_operand_ins }
    },
    {
        MVM_OP_set,
        "set",
        2,
        { MVM_operand_write_reg | MVM_operand_type_var, MVM_operand_read_reg | MVM_operand_type_var }
    },
    {
        MVM_OP_extend_u8,
        "extend_u8",
        2,
        { MVM_operand_write_reg | MVM_operand_int64, MVM_operand_read_reg | MVM_operand_int8 }
    },
    {
        MVM_OP_extend_u16,
        "extend_u16",
        2,
        { MVM_operand_write_reg | MVM_operand_int64, MVM_operand_read_reg | MVM_operand_int16 }
    },
    {
        MVM_OP_extend_u32,
        "extend_u32",
        2,
        { MVM_operand_write_reg | MVM_operand_int64, MVM_operand_read_reg | MVM_operand_int32 }
    },
    {
        MVM_OP_extend_i8,
        "extend_i8",
        2,
        { MVM_operand_write_reg | MVM_operand_int64, MVM_operand_read_reg | MVM_operand_int8 }
    },
    {
        MVM_OP_extend_i16,
        "extend_i16",
        2,
        { MVM_operand_write_reg | MVM_operand_int64, MVM_operand_read_reg | MVM_operand_int16 }
    },
    {
        MVM_OP_extend_i32,
        "extend_i32",
        2,
        { MVM_operand_write_reg | MVM_operand_int64, MVM_operand_read_reg | MVM_operand_int32 }
    },
    {
        MVM_OP_trunc_u8,
        "trunc_u8",
        2,
        { MVM_operand_write_reg | MVM_operand_int8, MVM_operand_read_reg | MVM_operand_int64 }
    },
    {
        MVM_OP_trunc_u16,
        "trunc_u16",
        2,
        { MVM_operand_write_reg | MVM_operand_int16, MVM_operand_read_reg | MVM_operand_int64 }
    },
    {
        MVM_OP_trunc_u32,
        "trunc_u32",
        2,
        { MVM_operand_write_reg | MVM_operand_int32, MVM_operand_read_reg | MVM_operand_int64 }
    },
    {
        MVM_OP_trunc_i8,
        "trunc_i8",
        2,
        { MVM_operand_write_reg | MVM_operand_int8, MVM_operand_read_reg | MVM_operand_int64 }
    },
    {
        MVM_OP_trunc_i16,
        "trunc_i16",
        2,
        { MVM_operand_write_reg | MVM_operand_int16, MVM_operand_read_reg | MVM_operand_int64 }
    },
    {
        MVM_OP_trunc_i32,
        "trunc_i32",
        2,
        { MVM_operand_write_reg | MVM_operand_int32, MVM_operand_read_reg | MVM_operand_int64 }
    },
    {
        MVM_OP_extend_n32,
        "extend_n32",
        2,
        { MVM_operand_write_reg | MVM_operand_num64, MVM_operand_read_reg | MVM_operand_num32 }
    },
    {
        MVM_OP_trunc_n32,
        "trunc_n32",
        2,
        { MVM_operand_write_reg | MVM_operand_num32, MVM_operand_read_reg | MVM_operand_num64 }
    },
    {
        MVM_OP_get_lex,
        "get_lex",
        2,
        { MVM_operand_write_reg | MVM_operand_type_var, MVM_operand_read_lex | MVM_operand_type_var }
    },
    {
        MVM_OP_bind_lex,
        "bind_lex",
        2,
        { MVM_operand_write_lex | MVM_operand_type_var, MVM_operand_read_reg | MVM_operand_type_var }
    },
    {
        MVM_OP_get_lex_lo,
        "get_lex_lo",
        3,
        { MVM_operand_write_reg | MVM_operand_type_var, MVM_operand_lex_outer, MVM_operand_read_lex | MVM_operand_type_var }
    },
    {
        MVM_OP_bind_lex_lo,
        "bind_lex_lo",
        3,
        { MVM_operand_write_lex | MVM_operand_type_var, MVM_operand_lex_outer, MVM_operand_read_reg | MVM_operand_type_var }
    },
    {
        MVM_OP_get_lex_ni,
        "get_lex_ni",
        2,
        { MVM_operand_write_reg | MVM_operand_int64, MVM_operand_str }
    },
    {
        MVM_OP_get_lex_nn,
        "get_lex_nn",
        2,
        { MVM_operand_write_reg | MVM_operand_num64, MVM_operand_str }
    },
    {
        MVM_OP_get_lex_ns,
        "get_lex_ns",
        2,
        { MVM_operand_write_reg | MVM_operand_str, MVM_operand_str }
    },
    {
        MVM_OP_get_lex_no,
        "get_lex_no",
        2,
        { MVM_operand_write_reg | MVM_operand_obj, MVM_operand_str }
    },
    {
        MVM_OP_bind_lex_ni,
        "bind_lex_ni",
        2,
        { MVM_operand_str, MVM_operand_read_reg | MVM_operand_int64 }
    },
    {
        MVM_OP_bind_lex_nn,
        "bind_lex_nn",
        2,
        { MVM_operand_str, MVM_operand_read_reg | MVM_operand_num64 }
    },
    {
        MVM_OP_bind_lex_ns,
        "bind_lex_ns",
        2,
        { MVM_operand_str, MVM_operand_read_reg | MVM_operand_str }
    },
    {
        MVM_OP_bind_lex_no,
        "bind_lex_no",
        2,
        { MVM_operand_str, MVM_operand_read_reg | MVM_operand_obj }
    },
    {
        MVM_OP_return_i,
        "return_i",
        1,
        { MVM_operand_read_reg | MVM_operand_int64 }
    },
    {
        MVM_OP_return_n,
        "return_n",
        1,
        { MVM_operand_read_reg | MVM_operand_num64 }
    },
    {
        MVM_OP_return_s,
        "return_s",
        1,
        { MVM_operand_read_reg | MVM_operand_str }
    },
    {
        MVM_OP_return_o,
        "return_o",
        1,
        { MVM_operand_read_reg | MVM_operand_obj }
    },
    {
        MVM_OP_return,
        "return",
        0,
    },
};
