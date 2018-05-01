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

void p4::codegen::stage_1_traverse(tree::node* root)
{
    //
    // Stage 1: Rough Sketch
    //   * Produces a sequence of gen*'s outlining the final code
    //   * No storage allocation is performed
    //       - Variable references are kept as strings
    //       - Temporary variables are named "" (empty string)
    //

    if (auto node = dynamic_cast<tree::node_program*>(root))
    {
        stage_1_traverse(node->nd_vars);
        stage_1_traverse(node->nd_block);
    }
    else if (auto node = dynamic_cast<tree::node_block*>(root))
    {
        stage_1_traverse(node->nd_vars);
        stage_1_traverse(node->nd_stats);
    }
    else if (auto node = dynamic_cast<tree::node_vars*>(root))
    {
        stage_1_traverse(node->nd_mvars);
    }
    else if (auto node = dynamic_cast<tree::node_mvars_p1*>(root))
    {
    }
    else if (auto node = dynamic_cast<tree::node_mvars_p2*>(root))
    {
        stage_1_traverse(node->nd_mvars);
    }
    else if (auto node = dynamic_cast<tree::node_expr*>(root))
    {
        stage_1_traverse(node->nd_M);
        stage_1_traverse(node->nd_expr_2);
    }
    else if (auto node = dynamic_cast<tree::node_expr_2_p1*>(root))
    {
        stage_1_traverse(node->nd_rhs);
    }
    else if (auto node = dynamic_cast<tree::node_expr_2_p2*>(root))
    {
        stage_1_traverse(node->nd_rhs);
    }
    else if (auto node = dynamic_cast<tree::node_expr_2_p3*>(root))
    {
        stage_1_traverse(node->nd_rhs);
    }
    else if (auto node = dynamic_cast<tree::node_expr_2_p4*>(root))
    {
        stage_1_traverse(node->nd_rhs);
    }
    else if (auto node = dynamic_cast<tree::node_M_p1*>(root))
    {
        stage_1_traverse(node->nd_M);
    }
    else if (auto node = dynamic_cast<tree::node_M_p2*>(root))
    {
        stage_1_traverse(node->nd_R);
    }
    else if (auto node = dynamic_cast<tree::node_R_p1*>(root))
    {
        stage_1_traverse(node->nd_expr);
    }
    else if (auto node = dynamic_cast<tree::node_R_p2*>(root))
    {
    }
    else if (auto node = dynamic_cast<tree::node_R_p3*>(root))
    {
    }
    else if (auto node = dynamic_cast<tree::node_stats*>(root))
    {
        stage_1_traverse(node->nd_stat);
        stage_1_traverse(node->nd_mStat);
    }
    else if (auto node = dynamic_cast<tree::node_mStat*>(root))
    {
        stage_1_traverse(node->nd_stat);
        stage_1_traverse(node->nd_mStat);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p1*>(root))
    {
        stage_1_traverse(node->nd_in);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p2*>(root))
    {
        stage_1_traverse(node->nd_out);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p3*>(root))
    {
        stage_1_traverse(node->nd_block);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p4*>(root))
    {
        stage_1_traverse(node->nd_if);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p5*>(root))
    {
        stage_1_traverse(node->nd_loop);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p6*>(root))
    {
        stage_1_traverse(node->nd_assign);
    }
    else if (auto node = dynamic_cast<tree::node_in*>(root))
    {
        // Variable
        auto v = new var;
        v->name = node->tk_identifier.content;

        // Code: Read to variable
        auto instr = new gen_instr_read;
        instr->dest = v;
        m_gens.push_back(instr);
    }
    else if (auto node = dynamic_cast<tree::node_out*>(root))
    {
        stage_1_traverse(node->nd_expr);

        // Temporary variable
        auto v = new var;
        v->name = "";

        // Code: Store ACC into temporary
        auto instr1 = new gen_instr_store;
        instr1->dest = v;
        m_gens.push_back(instr1);

        // Code: Write from temporary
        auto instr2 = new gen_instr_write_var;
        instr2->src = v;
        m_gens.push_back(instr2);
    }
    else if (auto node = dynamic_cast<tree::node_if*>(root))
    {
        stage_1_traverse(node->nd_lhs);
        stage_1_traverse(node->nd_operator);
        stage_1_traverse(node->nd_rhs);
        stage_1_traverse(node->nd_body);
    }
    else if (auto node = dynamic_cast<tree::node_loop*>(root))
    {
        stage_1_traverse(node->nd_lhs);
        stage_1_traverse(node->nd_operator);
        stage_1_traverse(node->nd_rhs);
        stage_1_traverse(node->nd_body);
    }
    else if (auto node = dynamic_cast<tree::node_assign*>(root))
    {
        stage_1_traverse(node->nd_value);
    }
    else if (auto node = dynamic_cast<tree::node_RO_p1*>(root))
    {
        stage_1_traverse(node->nd_lt);
    }
    else if (auto node = dynamic_cast<tree::node_RO_p2*>(root))
    {
        stage_1_traverse(node->nd_gt);
    }
    else if (auto node = dynamic_cast<tree::node_RO_p3*>(root))
    {
        stage_1_traverse(node->nd_eq);
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

    // Finish with stop instruction
    m_gens.push_back(new gen_instr_stop);
}

void p4::codegen::do_stage_1()
{
    // Generate instructions from parse tree
    stage_1_traverse(m_tree);
}

void p4::codegen::do_stage_2()
{
    // TODO: Create gens for var declarations and resolve their refs
}

void p4::codegen::do_stage_3()
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
    do_stage_1();
    do_stage_2();
    do_stage_3();

    // Compose the final output
    compose();
}
