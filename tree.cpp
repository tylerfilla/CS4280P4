/*
 * Tyler Filla
 * CS 4280
 * Project 4
 */

#include "tree.h"

p4::tree::node_program::~node_program()
{
    delete nd_vars;
    delete nd_block;
}

p4::tree::node_block::~node_block()
{
    delete nd_vars;
    delete nd_stats;
}

p4::tree::node_vars::~node_vars()
{
    delete nd_mvars;
}

p4::tree::node_mvars_p2::~node_mvars_p2()
{
    delete nd_mvars;
}

p4::tree::node_expr::~node_expr()
{
    delete nd_M;
    delete nd_expr_2;
}

p4::tree::node_expr_2_p1::~node_expr_2_p1()
{
    delete nd_rhs;
}

p4::tree::node_expr_2_p2::~node_expr_2_p2()
{
    delete nd_rhs;
}

p4::tree::node_expr_2_p3::~node_expr_2_p3()
{
    delete nd_rhs;
}

p4::tree::node_expr_2_p4::~node_expr_2_p4()
{
    delete nd_rhs;
}

p4::tree::node_M_p1::~node_M_p1()
{
    delete nd_M;
}

p4::tree::node_M_p2::~node_M_p2()
{
    delete nd_R;
}

p4::tree::node_R_p1::~node_R_p1()
{
    delete nd_expr;
}

p4::tree::node_stats::~node_stats()
{
    delete nd_stat;
    delete nd_mStat;
}

p4::tree::node_mStat::~node_mStat()
{
    delete nd_stat;
    delete nd_mStat;
}

p4::tree::node_stat_p1::~node_stat_p1()
{
    delete nd_in;
}

p4::tree::node_stat_p2::~node_stat_p2()
{
    delete nd_out;
}

p4::tree::node_stat_p3::~node_stat_p3()
{
    delete nd_block;
}

p4::tree::node_stat_p4::~node_stat_p4()
{
    delete nd_if;
}

p4::tree::node_stat_p5::~node_stat_p5()
{
    delete nd_loop;
}

p4::tree::node_stat_p6::~node_stat_p6()
{
    delete nd_assign;
}

p4::tree::node_out::~node_out()
{
    delete nd_expr;
}

p4::tree::node_if::~node_if()
{
    delete nd_lhs;
    delete nd_operator;
    delete nd_rhs;
    delete nd_body;
}

p4::tree::node_loop::~node_loop()
{
    delete nd_lhs;
    delete nd_operator;
    delete nd_rhs;
    delete nd_body;
}

p4::tree::node_assign::~node_assign()
{
    delete nd_value;
}

p4::tree::node_RO_p1::~node_RO_p1()
{
    delete nd_lt;
}

p4::tree::node_RO_p2::~node_RO_p2()
{
    delete nd_gt;
}

p4::tree::node_RO_p3::~node_RO_p3()
{
    delete nd_eq;
}
