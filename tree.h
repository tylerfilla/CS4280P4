/*
 * Tyler Filla
 * CS 4280
 * Project 2
 */

#ifndef P2_TREE_H
#define P2_TREE_H

#include "token.h"

namespace p2
{

namespace tree
{

struct node;
struct node_program;
struct node_block;
struct node_vars;
struct node_mvars;
struct node_mvars_p1;
struct node_mvars_p2;
struct node_expr;
struct node_expr_2;
struct node_expr_2_p1;
struct node_expr_2_p2;
struct node_expr_2_p3;
struct node_expr_2_p4;
struct node_M;
struct node_M_p1;
struct node_M_p2;
struct node_R;
struct node_R_p1;
struct node_R_p2;
struct node_R_p3;
struct node_stats;
struct node_mStat;
struct node_stat;
struct node_stat_p1;
struct node_stat_p2;
struct node_stat_p3;
struct node_stat_p4;
struct node_stat_p5;
struct node_stat_p6;
struct node_in;
struct node_out;
struct node_if;
struct node_loop;
struct node_assign;
struct node_RO;
struct node_RO_p1;
struct node_RO_p2;
struct node_RO_p3;
struct node_RO_lt;
struct node_RO_gt;
struct node_RO_eq;

/**
 * A node in a parse tree.
 */
struct node
{
};

/**
 * BNF: <program> -> program <vars> <block>
 * Tokens: none
 * Children: 2, 3
 */
struct node_program : node
{
    /** Item 2. */
    node_vars* nd_vars;

    /** Item 3. */
    node_block* nd_block;
};

/**
 * BNF: <block> -> start <vars> <stats> stop
 * Tokens: none
 * Children: 2, 3
 */
struct node_block : node
{
    /** Item 2. */
    node_vars* nd_vars;

    /** Item 3. */
    node_stats* nd_stats;
};

/**
 * BNF: <vars> -> var IDENTIFIER = INTEGER <mvars>
 * Tokens: 2, 4
 * Children: 5
 * Nullable.
 */
struct node_vars : node
{
    /** Item 2. */
    token tk_identifier;

    /** Item 4. */
    token tk_integer;

    /** Item 5. */
    node_mvars* nd_mvars;
};

/**
 * BNF: <mvars>
 * Unknown production.
 */
struct node_mvars : node
{
    // nothing
};

/**
 * BNF: <mvars> -> .
 * Tokens: none
 * Children: none
 * Terminal.
 */
struct node_mvars_p1 : node_mvars
{
    // nothing
};

/**
 * BNF: <mvars> -> : IDENTIFIER <mvars>
 * Tokens: 2
 * Children: 3
 */
struct node_mvars_p2 : node_mvars
{
    /** Item 2. */
    token tk_identifier;

    /** Item 3. */
    node_mvars* nd_mvars;
};

/**
 * BNF: <expr> -> <M> <expr_2>
 * Tokens: none
 * Children: 1, 2
 */
struct node_expr : node
{
    /** Item 1. */
    node_M* nd_M;

    /** Item 2. */
    node_expr_2* nd_expr_2;
};

/**
 * BNF: <expr_2>
 * Unknown production.
 * Nullable.
 */
struct node_expr_2 : node
{
    // nothing
};

/**
 * BNF: <expr_2> -> + <expr>
 * Tokens: none
 * Children: 2
 */
struct node_expr_2_p1 : node_expr_2
{
    /** Item 2. */
    node_expr* nd_rhs;
};

/**
 * BNF: <expr_2> -> - <expr>
 * Tokens: none
 * Children: 2
 */
struct node_expr_2_p2 : node_expr_2
{
    /** Item 2. */
    node_expr* nd_rhs;
};

/**
 * BNF: <expr_2> -> / <expr>
 * Tokens: none
 * Children: 2
 */
struct node_expr_2_p3 : node_expr_2
{
    /** Item 2. */
    node_expr* nd_rhs;
};

/**
 * BNF: <expr_2> -> * <expr>
 * Tokens: none
 * Children: 2
 */
struct node_expr_2_p4 : node_expr_2
{
    /** Item 2. */
    node_expr* nd_rhs;
};

/**
 * BNF: <M>
 * Unknown production.
 */
struct node_M : node
{
    // nothing
};

/**
 * BNF: <M> -> % <M>
 * Tokens: none
 * Children: 2
 */
struct node_M_p1 : node_M
{
    /** Item 2. */
    node_M* nd_M;
};

/**
 * BNF: <M> -> <R>
 * Tokens: none
 * Children: 1
 */
struct node_M_p2 : node_M
{
    /** Item 1. */
    node_R* nd_R;
};

/**
 * BNF: <R>
 * Unknown production.
 */
struct node_R : node
{
    // nothing
};

/**
 * BNF: <R> -> ( <expr> )
 * Tokens: none
 * Children: 2
 */
struct node_R_p1 : node_R
{
    /** Item 2. */
    node_expr* nd_expr;
};

/**
 * BNF: <R> -> IDENTIFIER
 * Tokens: 1
 * Children: none
 * Terminal.
 */
struct node_R_p2 : node_R
{
    /** Item 1. */
    token tk_identifier;
};

/**
 * BNF: <R> -> INTEGER
 * Tokens: 1
 * Children: none
 * Terminal.
 */
struct node_R_p3 : node_R
{
    /** Item 1. */
    token tk_integer;
};

/**
 * BNF: <stats> -> <stat> <mStat>
 * Tokens: none
 * Children: 1, 2
 */
struct node_stats : node
{
    /** Item 1. */
    node_stat* nd_stat;

    /** Item 2. */
    node_mStat* nd_mStat;
};

/**
 * BNF: <mStat> -> <stat> <mStat>
 * Tokens: none
 * Children: 1, 2
 * Nullable.
 */
struct node_mStat : node
{
    /** Item 1. */
    node_stat* nd_stat;

    /** Item 2. */
    node_mStat* nd_mStat;
};

/**
 * BNF: <stat>
 * Unknown production.
 */
struct node_stat : node
{
    // nothing
};

/**
 * BNF: <stat> -> <in>
 * Tokens: none
 * Children: 1
 */
struct node_stat_p1 : node_stat
{
    /** Item 1. */
    node_in* nd_in;
};

/**
 * BNF: <stat> -> <out>
 * Tokens: none
 * Children: 1
 */
struct node_stat_p2 : node_stat
{
    /** Item 1. */
    node_out* nd_out;
};

/**
 * BNF: <stat> -> <block>
 * Tokens: none
 * Children: 1
 */
struct node_stat_p3 : node_stat
{
    /** Item 1. */
    node_block* nd_block;
};

/**
 * BNF: <stat> -> <if>
 * Tokens: none
 * Children: 1
 */
struct node_stat_p4 : node_stat
{
    /** Item 1. */
    node_if* nd_if;
};

/**
 * BNF: <stat> -> <loop>
 * Tokens: none
 * Children: 1
 */
struct node_stat_p5 : node_stat
{
    /** Item 1. */
    node_loop* nd_loop;
};

/**
 * BNF: <stat> -> <assign>
 * Tokens: none
 * Children: 1
 */
struct node_stat_p6 : node_stat
{
    /** Item 1. */
    node_assign* nd_assign;
};

/**
 * BNF: <in> -> read IDENTIFIER .
 * Tokens: 2
 * Children: none
 * Terminal.
 */
struct node_in : node
{
    /** Item 2. */
    token tk_identifier;
};

/**
 * BNF: <out> -> print <expr> .
 * Tokens: none
 * Children: 2
 */
struct node_out : node
{
    /** Item 2. */
    node_expr* nd_expr;
};

/**
 * BNF: <if> -> iff ( <expr> <RO> <expr> ) <stat>
 * Tokens: none
 * Children: 3, 4, 5, 7
 */
struct node_if : node
{
    /** Item 3. */
    node_expr* nd_lhs;

    /** Item 4. */
    node_RO* nd_operator;

    /** Item 5. */
    node_expr* nd_rhs;

    /** Item 7. */
    node_stat* nd_body;
};

/**
 * BNF: <loop> -> iter ( <expr> <RO> <expr> ) <stat>
 * Tokens: none
 * Children: 3, 4, 5, 7
 */
struct node_loop : node
{
    /** Item 3. */
    node_expr* nd_lhs;

    /** Item 4. */
    node_RO* nd_operator;

    /** Item 5. */
    node_expr* nd_rhs;

    /** Item 7. */
    node_stat* nd_body;
};

/**
 * BNF: <assign> -> let IDENTIFIER = <expr> .
 * Tokens: 2
 * Children: 4
 */
struct node_assign : node
{
    /** Item 2. */
    token tk_identifier;

    /** Item 4. */
    node_expr* nd_expr;
};

/**
 * BNF: <RO>
 * Unknown production.
 */
struct node_RO : node
{
    // nothing
};

/**
 * BNF: <RO> -> < <RO_lt>
 * Tokens: none
 * Children: 2
 */
struct node_RO_p1 : node_RO
{
    /** Item 2. */
    node_RO_lt* nd_lt;
};

/**
 * BNF: <RO> -> > <RO_gt>
 * Tokens: none
 * Children: 2
 */
struct node_RO_p2 : node_RO
{
    /** Item 2. */
    node_RO_gt* nd_gt;
};

/**
 * BNF: <RO> -> = <RO_eq>
 * Tokens: none
 * Children: 2
 */
struct node_RO_p3 : node_RO
{
    /** Item 2. */
    node_RO_eq* nd_eq;
};

/**
 * BNF: <RO_lt>
 * Tokens: none
 * Children: none
 * Terminal.
 * Nullable.
 */
struct node_RO_lt : node
{
    // nothing
};

/**
 * BNF: <RO_gt>
 * Tokens: none
 * Children: none
 * Terminal.
 * Nullable.
 */
struct node_RO_gt : node
{
    // nothing
};

/**
 * BNF: <RO_eq>
 * Tokens: none
 * Children: none
 * Terminal.
 * Nullable.
 */
struct node_RO_eq : node
{
    // nothing
};

} // namespace tree

} // namespace p2

#endif // #ifndef P2_TREE_H
