/*
 * Tyler Filla
 * CS 4280
 * Project 4
 */

#ifndef P4_TREE_H
#define P4_TREE_H

#include <string>
#include "token.h"

namespace p4
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
    virtual ~node() = default;

    virtual std::string get_name() const
    { return ""; }

    virtual std::string get_prod() const
    { return ""; }
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

    ~node_program() final;

    std::string get_name() const final
    { return "program"; }

    std::string get_prod() const final
    { return "<program>"; }
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

    ~node_block() final;

    std::string get_name() const final
    { return "block"; }

    std::string get_prod() const final
    { return "<block>"; }
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
    token tk_name;

    /** Item 4. */
    token tk_value;

    /** Item 5. */
    node_mvars* nd_mvars;

    ~node_vars() final;

    std::string get_name() const final
    { return "vars"; }

    std::string get_prod() const final
    { return "<vars>"; }
};

/**
 * BNF: <mvars>
 * Unknown production.
 */
struct node_mvars : node
{
    ~node_mvars() override = default;
};

/**
 * BNF: <mvars> -> .
 * Tokens: none
 * Children: none
 * Terminal.
 */
struct node_mvars_p1 : node_mvars
{
    ~node_mvars_p1() final = default;

    std::string get_name() const final
    { return "mvars"; }

    std::string get_prod() const final
    { return "<mvars> #1 (starts with dot)"; }
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

    ~node_mvars_p2() final;

    std::string get_name() const final
    { return "mvars"; }

    std::string get_prod() const final
    { return "<mvars> #2 (starts with colon)"; }
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

    ~node_expr() final;

    std::string get_name() const final
    { return "expr"; }

    std::string get_prod() const final
    { return "<expr>"; }
};

/**
 * BNF: <expr_2>
 * Unknown production.
 * Nullable.
 */
struct node_expr_2 : node
{
    ~node_expr_2() override = default;
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

    ~node_expr_2_p1() final;

    std::string get_name() const final
    { return "expr_2"; }

    std::string get_prod() const final
    { return "<expr_2> #1 (starts with plus)"; }
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

    ~node_expr_2_p2() final;

    std::string get_name() const final
    { return "expr_2"; }

    std::string get_prod() const final
    { return "<expr_2> #2 (starts with minus)"; }
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

    ~node_expr_2_p3() final;

    std::string get_name() const final
    { return "expr_2"; }

    std::string get_prod() const final
    { return "<expr_2> #3 (starts with slash)"; }
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

    ~node_expr_2_p4() final;

    std::string get_name() const final
    { return "expr_2"; }

    std::string get_prod() const final
    { return "<expr_2> #4 (starts with asterisk)"; }
};

/**
 * BNF: <M>
 * Unknown production.
 */
struct node_M : node
{
    ~node_M() override = default;
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

    ~node_M_p1() final;

    std::string get_name() const final
    { return "M"; }

    std::string get_prod() const final
    { return "<M> #1 (starts with percent)"; }
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

    ~node_M_p2() final;

    std::string get_name() const final
    { return "M"; }

    std::string get_prod() const final
    { return "<M> #2 (starts with <R>)"; }
};

/**
 * BNF: <R>
 * Unknown production.
 */
struct node_R : node
{
    ~node_R() override = default;
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

    ~node_R_p1() final;

    std::string get_name() const final
    { return "R"; }

    std::string get_prod() const final
    { return "<R> #1 (starts with o-paren)"; }
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

    ~node_R_p2() final = default;

    std::string get_name() const final
    { return "R"; }

    std::string get_prod() const final
    { return "<R> #2 (starts with identifier)"; }
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

    ~node_R_p3() final = default;

    std::string get_name() const final
    { return "R"; }

    std::string get_prod() const final
    { return "<R> #3 (starts with integer)"; }
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

    ~node_stats() final;

    std::string get_name() const final
    { return "stats"; }

    std::string get_prod() const final
    { return "<stats>"; }
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

    ~node_mStat() final;

    std::string get_name() const final
    { return "mStat"; }

    std::string get_prod() const final
    { return "<mStat>"; }
};

/**
 * BNF: <stat>
 * Unknown production.
 */
struct node_stat : node
{
    ~node_stat() override = default;
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

    ~node_stat_p1() final;

    std::string get_name() const final
    { return "stat"; }

    std::string get_prod() const final
    { return "<stat> #1 (starts with <in>)"; }
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

    ~node_stat_p2() final;

    std::string get_name() const final
    { return "stat"; }

    std::string get_prod() const final
    { return "<stat> #2 (starts with <out>)"; }
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

    ~node_stat_p3() final;

    std::string get_name() const final
    { return "stat"; }

    std::string get_prod() const final
    { return "<stat> #3 (starts with <block>)"; }
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

    ~node_stat_p4() final;

    std::string get_name() const final
    { return "stat"; }

    std::string get_prod() const final
    { return "<stat> #4 (starts with <if>)"; }
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

    ~node_stat_p5() final;

    std::string get_name() const final
    { return "stat"; }

    std::string get_prod() const final
    { return "<stat> #5 (starts with <loop>)"; }
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

    ~node_stat_p6() final;

    std::string get_name() const final
    { return "stat"; }

    std::string get_prod() const final
    { return "<stat> #6 (starts with <assign>)"; }
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

    ~node_in() final = default;

    std::string get_name() const final
    { return "in"; }

    std::string get_prod() const final
    { return "<in>"; }
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

    ~node_out() final;

    std::string get_name() const final
    { return "out"; }

    std::string get_prod() const final
    { return "<out>"; }
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

    ~node_if() final;

    std::string get_name() const final
    { return "if"; }

    std::string get_prod() const final
    { return "<if>"; }
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

    ~node_loop() final;

    std::string get_name() const final
    { return "loop"; }

    std::string get_prod() const final
    { return "<loop>"; }
};

/**
 * BNF: <assign> -> let IDENTIFIER = <expr> .
 * Tokens: 2
 * Children: 4
 */
struct node_assign : node
{
    /** Item 2. */
    token tk_name;

    /** Item 4. */
    node_expr* nd_value;

    ~node_assign() final;

    std::string get_name() const final
    { return "assign"; }

    std::string get_prod() const final
    { return "<assign>"; }
};

/**
 * BNF: <RO>
 * Unknown production.
 */
struct node_RO : node
{
    ~node_RO() override = default;
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

    ~node_RO_p1() final;

    std::string get_name() const final
    { return "RO"; }

    std::string get_prod() const final
    { return "<RO> #1 (starts with lt-sign)"; }
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

    ~node_RO_p2() final;

    std::string get_name() const final
    { return "RO"; }

    std::string get_prod() const final
    { return "<RO> #2 (starts with gt-sign)"; }
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

    ~node_RO_p3() final;

    std::string get_name() const final
    { return "RO"; }

    std::string get_prod() const final
    { return "<RO> #3 (starts with eq-sign)"; }
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
    ~node_RO_lt() final = default;

    std::string get_name() const final
    { return "RO_lt"; }

    std::string get_prod() const final
    { return "<RO_lt>"; }
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
    ~node_RO_gt() final = default;

    std::string get_name() const final
    { return "RO_gt"; }

    std::string get_prod() const final
    { return "<RO_gt>"; }
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
    ~node_RO_eq() final = default;

    std::string get_name() const final
    { return "RO_eq"; }

    std::string get_prod() const final
    { return "<RO_eq>"; }
};

} // namespace tree

} // namespace p4

#endif // #ifndef P4_TREE_H
