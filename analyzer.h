/*
 * Tyler Filla
 * CS 4280
 * Project 3
 */

#ifndef P3_ANALYZER_H
#define P3_ANALYZER_H

#include "tree.h"

namespace p3
{

/**
 * A semantic analyzer for the language.
 */
class analyzer
{
    /** The parse tree. */
    tree::node* m_tree;

public:
    analyzer(tree::node* p_tree) : m_tree(p_tree)
    {
    }

    analyzer(const analyzer& rhs) = delete;

    ~analyzer() = default;

    analyzer& operator=(const analyzer& rhs) = delete;

private:
    /**
     * Analyze a "program" node.
     */
    void analyze_program(tree::node_program* program)
    {
        std::cout << "program\n";
    }

    /**
     * Analyze a single node.
     */
    void analyze_node(tree::node* p_node)
    {
        if (!p_node)
            return;

        if (auto node = dynamic_cast<tree::node_program*>(p_node))
        {
            analyze_program(node);
            analyze_node(node->nd_vars);
            analyze_node(node->nd_block);
        }
        else if (auto node = dynamic_cast<tree::node_block*>(p_node))
        {
            analyze_node(node->nd_vars);
            analyze_node(node->nd_stats);
        }
        else if (auto node = dynamic_cast<tree::node_vars*>(p_node))
        {
            analyze_node(node->nd_mvars);
        }
        else if (auto node = dynamic_cast<tree::node_mvars_p1*>(p_node))
        {
        }
        else if (auto node = dynamic_cast<tree::node_mvars_p2*>(p_node))
        {
            analyze_node(node->nd_mvars);
        }
        else if (auto node = dynamic_cast<tree::node_expr*>(p_node))
        {
            analyze_node(node->nd_M);
            analyze_node(node->nd_expr_2);
        }
        else if (auto node = dynamic_cast<tree::node_expr_2_p1*>(p_node))
        {
            analyze_node(node->nd_rhs);
        }
        else if (auto node = dynamic_cast<tree::node_expr_2_p2*>(p_node))
        {
            analyze_node(node->nd_rhs);
        }
        else if (auto node = dynamic_cast<tree::node_expr_2_p3*>(p_node))
        {
            analyze_node(node->nd_rhs);
        }
        else if (auto node = dynamic_cast<tree::node_expr_2_p4*>(p_node))
        {
            analyze_node(node->nd_rhs);
        }
        else if (auto node = dynamic_cast<tree::node_M_p1*>(p_node))
        {
            analyze_node(node->nd_M);
        }
        else if (auto node = dynamic_cast<tree::node_M_p2*>(p_node))
        {
            analyze_node(node->nd_R);
        }
        else if (auto node = dynamic_cast<tree::node_R_p1*>(p_node))
        {
            analyze_node(node->nd_expr);
        }
        else if (auto node = dynamic_cast<tree::node_R_p2*>(p_node))
        {
        }
        else if (auto node = dynamic_cast<tree::node_R_p3*>(p_node))
        {
        }
        else if (auto node = dynamic_cast<tree::node_stats*>(p_node))
        {
            analyze_node(node->nd_stat);
            analyze_node(node->nd_mStat);
        }
        else if (auto node = dynamic_cast<tree::node_mStat*>(p_node))
        {
            analyze_node(node->nd_stat);
            analyze_node(node->nd_mStat);
        }
        else if (auto node = dynamic_cast<tree::node_stat_p1*>(p_node))
        {
            analyze_node(node->nd_in);
        }
        else if (auto node = dynamic_cast<tree::node_stat_p2*>(p_node))
        {
            analyze_node(node->nd_out);
        }
        else if (auto node = dynamic_cast<tree::node_stat_p3*>(p_node))
        {
            analyze_node(node->nd_block);
        }
        else if (auto node = dynamic_cast<tree::node_stat_p4*>(p_node))
        {
            analyze_node(node->nd_if);
        }
        else if (auto node = dynamic_cast<tree::node_stat_p5*>(p_node))
        {
            analyze_node(node->nd_loop);
        }
        else if (auto node = dynamic_cast<tree::node_stat_p6*>(p_node))
        {
            analyze_node(node->nd_assign);
        }
        else if (auto node = dynamic_cast<tree::node_in*>(p_node))
        {
        }
        else if (auto node = dynamic_cast<tree::node_out*>(p_node))
        {
            analyze_node(node->nd_expr);
        }
        else if (auto node = dynamic_cast<tree::node_if*>(p_node))
        {
            analyze_node(node->nd_lhs);
            analyze_node(node->nd_operator);
            analyze_node(node->nd_rhs);
            analyze_node(node->nd_body);
        }
        else if (auto node = dynamic_cast<tree::node_loop*>(p_node))
        {
            analyze_node(node->nd_lhs);
            analyze_node(node->nd_operator);
            analyze_node(node->nd_rhs);
            analyze_node(node->nd_body);
        }
        else if (auto node = dynamic_cast<tree::node_assign*>(p_node))
        {
            analyze_node(node->nd_value);
        }
        else if (auto node = dynamic_cast<tree::node_RO_p1*>(p_node))
        {
            analyze_node(node->nd_lt);
        }
        else if (auto node = dynamic_cast<tree::node_RO_p2*>(p_node))
        {
            analyze_node(node->nd_gt);
        }
        else if (auto node = dynamic_cast<tree::node_RO_p3*>(p_node))
        {
            analyze_node(node->nd_eq);
        }
        else if (auto node = dynamic_cast<tree::node_RO_lt*>(p_node))
        {
        }
        else if (auto node = dynamic_cast<tree::node_RO_gt*>(p_node))
        {
        }
        else if (auto node = dynamic_cast<tree::node_RO_eq*>(p_node))
        {
        }
        else
        {
        }
    }

public:
    /**
     * Run semantic analysis on the given tree.
     */
    void run()
    {
        analyze_node(m_tree);
    }
};

} // namespace p3

#endif // #ifndef P3_ANALYZER_H
