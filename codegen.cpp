/*
 * Tyler Filla
 * CS 4280
 * Project 4
 */

#include <utility>
#include "codegen.h"

p4::codegen::codegen()
        : m_tree(nullptr)
        , m_output {}
        , m_output_ss {}
        , m_labels {}
        , m_vars {}
        , m_gens {}
{
}

void p4::codegen::traverse(tree::node* root, std::function<void(tree::node*)> cb_pre, std::function<void(tree::node*)> cb_post)
{
    if (!root)
        return;

    // Call preorder callback
    if (cb_pre)
    {
        cb_pre(root);
    }

    // Traverse node children
    // This depends on the dynamic type of the node
    if (auto node = dynamic_cast<tree::node_program*>(root))
    {
        traverse(node->nd_vars, cb_pre, cb_post);
        traverse(node->nd_block, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_block*>(root))
    {
        traverse(node->nd_vars, cb_pre, cb_post);
        traverse(node->nd_stats, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_vars*>(root))
    {
        traverse(node->nd_mvars, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_mvars_p1*>(root))
    {
    }
    else if (auto node = dynamic_cast<tree::node_mvars_p2*>(root))
    {
        traverse(node->nd_mvars, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_expr*>(root))
    {
        traverse(node->nd_M, cb_pre, cb_post);
        traverse(node->nd_expr_2, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_expr_2_p1*>(root))
    {
        traverse(node->nd_rhs, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_expr_2_p2*>(root))
    {
        traverse(node->nd_rhs, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_expr_2_p3*>(root))
    {
        traverse(node->nd_rhs, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_expr_2_p4*>(root))
    {
        traverse(node->nd_rhs, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_M_p1*>(root))
    {
        traverse(node->nd_M, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_M_p2*>(root))
    {
        traverse(node->nd_R, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_R_p1*>(root))
    {
        traverse(node->nd_expr, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_R_p2*>(root))
    {
    }
    else if (auto node = dynamic_cast<tree::node_R_p3*>(root))
    {
    }
    else if (auto node = dynamic_cast<tree::node_stats*>(root))
    {
        traverse(node->nd_stat, cb_pre, cb_post);
        traverse(node->nd_mStat, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_mStat*>(root))
    {
        traverse(node->nd_stat, cb_pre, cb_post);
        traverse(node->nd_mStat, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p1*>(root))
    {
        traverse(node->nd_in, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p2*>(root))
    {
        traverse(node->nd_out, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p3*>(root))
    {
        traverse(node->nd_block, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p4*>(root))
    {
        traverse(node->nd_if, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p5*>(root))
    {
        traverse(node->nd_loop, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p6*>(root))
    {
        traverse(node->nd_assign, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_in*>(root))
    {
    }
    else if (auto node = dynamic_cast<tree::node_out*>(root))
    {
        traverse(node->nd_expr, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_if*>(root))
    {
        traverse(node->nd_lhs, cb_pre, cb_post);
        traverse(node->nd_operator, cb_pre, cb_post);
        traverse(node->nd_rhs, cb_pre, cb_post);
        traverse(node->nd_body, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_loop*>(root))
    {
        traverse(node->nd_lhs, cb_pre, cb_post);
        traverse(node->nd_operator, cb_pre, cb_post);
        traverse(node->nd_rhs, cb_pre, cb_post);
        traverse(node->nd_body, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_assign*>(root))
    {
        traverse(node->nd_value, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_RO_p1*>(root))
    {
        traverse(node->nd_lt, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_RO_p2*>(root))
    {
        traverse(node->nd_gt, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_RO_p3*>(root))
    {
        traverse(node->nd_eq, cb_pre, cb_post);
    }
    else if (auto node = dynamic_cast<tree::node_RO_lt*>(root))
    {
    }
    else if (auto node = dynamic_cast<tree::node_RO_gt*>(root))
    {
    }
    else if (auto node = dynamic_cast<tree::node_RO_eq*>(root))
    {
    }

    // Call postorder callback
    if (cb_post)
    {
        cb_post(root);
    }
}

void p4::codegen::do_pass_1()
{
    // TODO: Create gens for instructions w/ named vars/labels

    auto&& cb_post = [](tree::node* node)
    {
        if (auto node = dynamic_cast<tree::node_program*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_block*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_vars*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_mvars_p1*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_mvars_p2*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_expr*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_expr_2_p1*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_expr_2_p2*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_expr_2_p3*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_expr_2_p4*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_M_p1*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_M_p2*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_R_p1*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_R_p2*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_R_p3*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_stats*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_mStat*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_stat_p1*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_stat_p2*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_stat_p3*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_stat_p4*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_stat_p5*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_stat_p6*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_in*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_out*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_if*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_loop*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_assign*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_RO_p1*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_RO_p2*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_RO_p3*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_RO_lt*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_RO_gt*>(root))
        {
        }
        else if (auto node = dynamic_cast<tree::node_RO_eq*>(root))
        {
        }
    };

    traverse(m_tree, nullptr, cb_post);
}

void p4::codegen::do_pass_2()
{
    // TODO: Create gens for var declarations and resolve their refs
}

void p4::codegen::do_pass_3()
{
    // TODO: Create gens for label declarations and resolve their refs
}

void p4::codegen::compose()
{
    // TODO: Iterate over all gens and produce formatted output

    // Set final output
    m_output = std::move(m_output_ss.str());
}

void p4::codegen::run()
{
    // Run generation passes
    do_pass_1();
    do_pass_2();
    do_pass_3();

    // Compose the final output
    compose();
}
