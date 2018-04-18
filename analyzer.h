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
    void analyze_program(tree::node_program* node)
    {
        std::cout << "program\n";
    }

    /**
     * Analyze a "block" node.
     */
    void analyze_block(tree::node_block* node)
    {
        std::cout << "block\n";
    }

    /**
     * Analyze a "vars" node.
     */
    void analyze_vars(tree::node_vars* node)
    {
        std::cout << "vars\n";
    }

    /**
     * Analyze a "mvars_p1" node.
     */
    void analyze_mvars_p1(tree::node_mvars_p1* node)
    {
        std::cout << "mvars_p1\n";
    }

    /**
     * Analyze a "mvars_p2" node.
     */
    void analyze_mvars_p2(tree::node_mvars_p2* node)
    {
        std::cout << "mvars_p2\n";
    }

    /**
     * Analyze a "expr" node.
     */
    void analyze_expr(tree::node_expr* node)
    {
        std::cout << "expr\n";
    }

    /**
     * Analyze a "expr_2_p1" node.
     */
    void analyze_expr_2_p1(tree::node_expr_2_p1* node)
    {
        std::cout << "expr_2_p1\n";
    }

    /**
     * Analyze a "expr_2_p2" node.
     */
    void analyze_expr_2_p2(tree::node_expr_2_p2* node)
    {
        std::cout << "expr_2_p2\n";
    }

    /**
     * Analyze a "expr_2_p3" node.
     */
    void analyze_expr_2_p3(tree::node_expr_2_p3* node)
    {
        std::cout << "expr_2_p3\n";
    }

    /**
     * Analyze a "expr_2_p4" node.
     */
    void analyze_expr_2_p4(tree::node_expr_2_p4* node)
    {
        std::cout << "expr_2_p4\n";
    }

    /**
     * Analyze a "M_p1" node.
     */
    void analyze_M_p1(tree::node_M_p1* node)
    {
        std::cout << "M_p1\n";
    }

    /**
     * Analyze a "M_p2" node.
     */
    void analyze_M_p2(tree::node_M_p2* node)
    {
        std::cout << "M_p2\n";
    }

    /**
     * Analyze a "R_p1" node.
     */
    void analyze_R_p1(tree::node_R_p1* node)
    {
        std::cout << "R_p1\n";
    }

    /**
     * Analyze a "R_p2" node.
     */
    void analyze_R_p2(tree::node_R_p2* node)
    {
        std::cout << "R_p2\n";
    }

    /**
     * Analyze a "R_p3" node.
     */
    void analyze_R_p3(tree::node_R_p3* node)
    {
        std::cout << "R_p3\n";
    }

    /**
     * Analyze a "stats" node.
     */
    void analyze_stats(tree::node_stats* node)
    {
        std::cout << "stats\n";
    }

    /**
     * Analyze a "mStat" node.
     */
    void analyze_mStat(tree::node_mStat* node)
    {
        std::cout << "mStat\n";
    }

    /**
     * Analyze a "stat_p1" node.
     */
    void analyze_stat_p1(tree::node_stat_p1* node)
    {
        std::cout << "stat_p1\n";
    }

    /**
     * Analyze a "stat_p2" node.
     */
    void analyze_stat_p2(tree::node_stat_p2* node)
    {
        std::cout << "stat_p2\n";
    }

    /**
     * Analyze a "stat_p3" node.
     */
    void analyze_stat_p3(tree::node_stat_p3* node)
    {
        std::cout << "stat_p3\n";
    }

    /**
     * Analyze a "stat_p4" node.
     */
    void analyze_stat_p4(tree::node_stat_p4* node)
    {
        std::cout << "stat_p4\n";
    }

    /**
     * Analyze a "stat_p5" node.
     */
    void analyze_stat_p5(tree::node_stat_p5* node)
    {
        std::cout << "stat_p5\n";
    }

    /**
     * Analyze a "stat_p6" node.
     */
    void analyze_stat_p6(tree::node_stat_p6* node)
    {
        std::cout << "stat_p6\n";
    }

    /**
     * Analyze a "in" node.
     */
    void analyze_in(tree::node_in* node)
    {
        std::cout << "in\n";
    }

    /**
     * Analyze a "out" node.
     */
    void analyze_out(tree::node_out* node)
    {
        std::cout << "out\n";
    }

    /**
     * Analyze a "if" node.
     */
    void analyze_if(tree::node_if* node)
    {
        std::cout << "if\n";
    }

    /**
     * Analyze a "loop" node.
     */
    void analyze_loop(tree::node_loop* node)
    {
        std::cout << "loop\n";
    }

    /**
     * Analyze a "assign" node.
     */
    void analyze_assign(tree::node_assign* node)
    {
        std::cout << "assign\n";
    }

    /**
     * Analyze a "RO_p1" node.
     */
    void analyze_RO_p1(tree::node_RO_p1* node)
    {
        std::cout << "RO_p1\n";
    }

    /**
     * Analyze a "RO_p2" node.
     */
    void analyze_RO_p2(tree::node_RO_p2* node)
    {
        std::cout << "RO_p2\n";
    }

    /**
     * Analyze a "RO_p3" node.
     */
    void analyze_RO_p3(tree::node_RO_p3* node)
    {
        std::cout << "RO_p3\n";
    }

    /**
     * Analyze a "RO_lt" node.
     */
    void analyze_RO_lt(tree::node_RO_lt* node)
    {
        std::cout << "RO_lt\n";
    }

    /**
     * Analyze a "RO_gt" node.
     */
    void analyze_RO_gt(tree::node_RO_gt* node)
    {
        std::cout << "RO_gt\n";
    }

    /**
     * Analyze a "RO_eq" node.
     */
    void analyze_RO_eq(tree::node_RO_eq* node)
    {
        std::cout << "RO_eq\n";
    }

    /**
     * Analyze a single node.
     */
    void analyze(tree::node* p_node)
    {
        if (!p_node)
            return;

        if (auto node = dynamic_cast<tree::node_program*>(p_node))
        {
            analyze_program(node);
            analyze(node->nd_vars);
            analyze(node->nd_block);
        }
        else if (auto node = dynamic_cast<tree::node_block*>(p_node))
        {
            analyze_block(node);
            analyze(node->nd_vars);
            analyze(node->nd_stats);
        }
        else if (auto node = dynamic_cast<tree::node_vars*>(p_node))
        {
            analyze_vars(node);
            analyze(node->nd_mvars);
        }
        else if (auto node = dynamic_cast<tree::node_mvars_p1*>(p_node))
        {
            analyze_mvars_p1(node);
        }
        else if (auto node = dynamic_cast<tree::node_mvars_p2*>(p_node))
        {
            analyze_mvars_p2(node);
            analyze(node->nd_mvars);
        }
        else if (auto node = dynamic_cast<tree::node_expr*>(p_node))
        {
            analyze_expr(node);
            analyze(node->nd_M);
            analyze(node->nd_expr_2);
        }
        else if (auto node = dynamic_cast<tree::node_expr_2_p1*>(p_node))
        {
            analyze_expr_2_p1(node);
            analyze(node->nd_rhs);
        }
        else if (auto node = dynamic_cast<tree::node_expr_2_p2*>(p_node))
        {
            analyze_expr_2_p2(node);
            analyze(node->nd_rhs);
        }
        else if (auto node = dynamic_cast<tree::node_expr_2_p3*>(p_node))
        {
            analyze_expr_2_p3(node);
            analyze(node->nd_rhs);
        }
        else if (auto node = dynamic_cast<tree::node_expr_2_p4*>(p_node))
        {
            analyze_expr_2_p4(node);
            analyze(node->nd_rhs);
        }
        else if (auto node = dynamic_cast<tree::node_M_p1*>(p_node))
        {
            analyze_M_p1(node);
            analyze(node->nd_M);
        }
        else if (auto node = dynamic_cast<tree::node_M_p2*>(p_node))
        {
            analyze_M_p2(node);
            analyze(node->nd_R);
        }
        else if (auto node = dynamic_cast<tree::node_R_p1*>(p_node))
        {
            analyze_R_p1(node);
            analyze(node->nd_expr);
        }
        else if (auto node = dynamic_cast<tree::node_R_p2*>(p_node))
        {
            analyze_R_p2(node);
        }
        else if (auto node = dynamic_cast<tree::node_R_p3*>(p_node))
        {
            analyze_R_p3(node);
        }
        else if (auto node = dynamic_cast<tree::node_stats*>(p_node))
        {
            analyze_stats(node);
            analyze(node->nd_stat);
            analyze(node->nd_mStat);
        }
        else if (auto node = dynamic_cast<tree::node_mStat*>(p_node))
        {
            analyze_mStat(node);
            analyze(node->nd_stat);
            analyze(node->nd_mStat);
        }
        else if (auto node = dynamic_cast<tree::node_stat_p1*>(p_node))
        {
            analyze_stat_p1(node);
            analyze(node->nd_in);
        }
        else if (auto node = dynamic_cast<tree::node_stat_p2*>(p_node))
        {
            analyze_stat_p2(node);
            analyze(node->nd_out);
        }
        else if (auto node = dynamic_cast<tree::node_stat_p3*>(p_node))
        {
            analyze_stat_p3(node);
            analyze(node->nd_block);
        }
        else if (auto node = dynamic_cast<tree::node_stat_p4*>(p_node))
        {
            analyze_stat_p4(node);
            analyze(node->nd_if);
        }
        else if (auto node = dynamic_cast<tree::node_stat_p5*>(p_node))
        {
            analyze_stat_p5(node);
            analyze(node->nd_loop);
        }
        else if (auto node = dynamic_cast<tree::node_stat_p6*>(p_node))
        {
            analyze_stat_p6(node);
            analyze(node->nd_assign);
        }
        else if (auto node = dynamic_cast<tree::node_in*>(p_node))
        {
            analyze_in(node);
        }
        else if (auto node = dynamic_cast<tree::node_out*>(p_node))
        {
            analyze_out(node);
            analyze(node->nd_expr);
        }
        else if (auto node = dynamic_cast<tree::node_if*>(p_node))
        {
            analyze_if(node);
            analyze(node->nd_lhs);
            analyze(node->nd_operator);
            analyze(node->nd_rhs);
            analyze(node->nd_body);
        }
        else if (auto node = dynamic_cast<tree::node_loop*>(p_node))
        {
            analyze_loop(node);
            analyze(node->nd_lhs);
            analyze(node->nd_operator);
            analyze(node->nd_rhs);
            analyze(node->nd_body);
        }
        else if (auto node = dynamic_cast<tree::node_assign*>(p_node))
        {
            analyze_assign(node);
            analyze(node->nd_value);
        }
        else if (auto node = dynamic_cast<tree::node_RO_p1*>(p_node))
        {
            analyze_RO_p1(node);
            analyze(node->nd_lt);
        }
        else if (auto node = dynamic_cast<tree::node_RO_p2*>(p_node))
        {
            analyze_RO_p2(node);
            analyze(node->nd_gt);
        }
        else if (auto node = dynamic_cast<tree::node_RO_p3*>(p_node))
        {
            analyze_RO_p3(node);
            analyze(node->nd_eq);
        }
        else if (auto node = dynamic_cast<tree::node_RO_lt*>(p_node))
        {
            analyze_RO_lt(node);
        }
        else if (auto node = dynamic_cast<tree::node_RO_gt*>(p_node))
        {
            analyze_RO_gt(node);
        }
        else if (auto node = dynamic_cast<tree::node_RO_eq*>(p_node))
        {
            analyze_RO_eq(node);
        }
    }

public:
    /**
     * Run semantic analysis on the given tree.
     */
    void run()
    {
        analyze(m_tree);
    }
};

} // namespace p3

#endif // #ifndef P3_ANALYZER_H
