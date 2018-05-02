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
        , m_ro_type {}
{
}

void p4::codegen::stage_1_traverse(tree::node* root)
{
    if (auto node = dynamic_cast<tree::node_program*>(root))
    {
        stage_1_traverse(node->nd_vars);
        stage_1_traverse(node->nd_block);

        // Code: Stop the program
        m_gens.push_back(new gen_instr_stop);
    }
    else if (auto node = dynamic_cast<tree::node_block*>(root))
    {
        // Passthrough
        stage_1_traverse(node->nd_vars);
        stage_1_traverse(node->nd_stats);
    }
    else if (auto node = dynamic_cast<tree::node_vars*>(root))
    {
        node->tk_name;
        node->tk_value;

        stage_1_traverse(node->nd_mvars);
    }
    else if (auto node = dynamic_cast<tree::node_mvars_p1*>(root))
    {
        // Empty
    }
    else if (auto node = dynamic_cast<tree::node_mvars_p2*>(root))
    {
        node->tk_identifier;

        stage_1_traverse(node->nd_mvars);
    }
    else if (auto node = dynamic_cast<tree::node_expr*>(root))
    {
        stage_1_traverse(node->nd_M);
        stage_1_traverse(node->nd_expr_2);
    }
    else if (auto node = dynamic_cast<tree::node_expr_2_p1*>(root))
    {
        // Temporary variable
        auto tmp_lhs = new var {};
        tmp_lhs->name = "";

        // Code: Store lhs (acc) to lhs (tmp_lhs)
        {
            auto instr = new gen_instr_store {};
            instr->dest = tmp_lhs;
            m_gens.push_back(instr);
        }

        // Call rhs
        stage_1_traverse(node->nd_rhs);

        // Code: Add rhs (acc) by lhs (tmp_lhs)
        {
            auto instr = new gen_instr_add_var {};
            instr->rhs = tmp_lhs;
            m_gens.push_back(instr);
        }
    }
    else if (auto node = dynamic_cast<tree::node_expr_2_p2*>(root))
    {
        // Code: Reserve stack space
        {
            auto instr = new gen_instr_push {};
            m_gens.push_back(instr);
        }

        // Code: Copy lhs (acc) to stack
        {
            auto instr = new gen_instr_stackw {};
            instr->offset = 0;
            m_gens.push_back(instr);
        }

        // Call rhs
        stage_1_traverse(node->nd_rhs);

        // Temporary variable
        auto tmp_rhs = new var {};
        tmp_rhs->name = "";

        // Code: Store rhs (acc) to rhs (tmp_rhs)
        {
            auto instr = new gen_instr_store {};
            instr->dest = tmp_rhs;
            m_gens.push_back(instr);
        }

        // Code: Load lhs (stack 0) to lhs (acc)
        {
            auto instr = new gen_instr_stackr {};
            m_gens.push_back(instr);
        }

        // Code: Subtract lhs (acc) by rhs (tmp_rhs)
        {
            auto instr = new gen_instr_sub_var {};
            instr->rhs = tmp_rhs;
            m_gens.push_back(instr);
        }

        // Code: Free stack space
        {
            auto instr = new gen_instr_pop {};
            m_gens.push_back(instr);
        }
    }
    else if (auto node = dynamic_cast<tree::node_expr_2_p3*>(root))
    {
        // Code: Reserve stack space
        {
            auto instr = new gen_instr_push {};
            m_gens.push_back(instr);
        }

        // Code: Copy lhs (acc) to stack
        {
            auto instr = new gen_instr_stackw {};
            instr->offset = 0;
            m_gens.push_back(instr);
        }

        // Call rhs
        stage_1_traverse(node->nd_rhs);

        // Temporary variable
        auto tmp_rhs = new var {};
        tmp_rhs->name = "";

        // Code: Store rhs (acc) to rhs (tmp_rhs)
        {
            auto instr = new gen_instr_store {};
            instr->dest = tmp_rhs;
            m_gens.push_back(instr);
        }

        // Code: Load lhs (stack 0) to lhs (acc)
        {
            auto instr = new gen_instr_stackr {};
            m_gens.push_back(instr);
        }

        // Code: Divide lhs (acc) by rhs (tmp_rhs)
        {
            auto instr = new gen_instr_div_var {};
            instr->rhs = tmp_rhs;
            m_gens.push_back(instr);
        }

        // Code: Free stack space
        {
            auto instr = new gen_instr_pop {};
            m_gens.push_back(instr);
        }
    }
    else if (auto node = dynamic_cast<tree::node_expr_2_p4*>(root))
    {
        // Temporary variable
        auto tmp_lhs = new var {};
        tmp_lhs->name = "";

        // Code: Store lhs (acc) to lhs (tmp_lhs)
        {
            auto instr = new gen_instr_store {};
            instr->dest = tmp_lhs;
            m_gens.push_back(instr);
        }

        // Call rhs
        stage_1_traverse(node->nd_rhs);

        // Code: Multiply rhs (acc) by lhs (tmp_lhs)
        {
            auto instr = new gen_instr_mult_var {};
            instr->rhs = tmp_lhs;
            m_gens.push_back(instr);
        }
    }
    else if (auto node = dynamic_cast<tree::node_M_p1*>(root))
    {
        // Call M
        stage_1_traverse(node->nd_M);

        // Code: Multiply by -1
        {
            auto instr = new gen_instr_mult_imm {};
            instr->rhs = -1;
            m_gens.push_back(instr);
        }
    }
    else if (auto node = dynamic_cast<tree::node_M_p2*>(root))
    {
        // Passthrough
        stage_1_traverse(node->nd_R);
    }
    else if (auto node = dynamic_cast<tree::node_R_p1*>(root))
    {
        // Passthrough
        stage_1_traverse(node->nd_expr);
    }
    else if (auto node = dynamic_cast<tree::node_R_p2*>(root))
    {
        // Variable
        auto ref = new var {};
        ref->name = node->tk_identifier.content;

        // Code: Load value (ref)
        {
            auto instr = new gen_instr_load_var {};
            instr->src = ref;
            m_gens.push_back(instr);
        }
    }
    else if (auto node = dynamic_cast<tree::node_R_p3*>(root))
    {
        int value = 0;

        std::stringstream ss {};
        ss << node->tk_integer.content;
        ss >> value;

        // Code: Load value (immediate)
        {
            auto instr = new gen_instr_load_imm {};
            instr->value = value;
            m_gens.push_back(instr);
        }
    }
    else if (auto node = dynamic_cast<tree::node_stats*>(root))
    {
        // Passthrough
        stage_1_traverse(node->nd_stat);
        stage_1_traverse(node->nd_mStat);
    }
    else if (auto node = dynamic_cast<tree::node_mStat*>(root))
    {
        // Passthrough
        stage_1_traverse(node->nd_stat);
        stage_1_traverse(node->nd_mStat);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p1*>(root))
    {
        // Passthrough
        stage_1_traverse(node->nd_in);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p2*>(root))
    {
        // Passthrough
        stage_1_traverse(node->nd_out);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p3*>(root))
    {
        // Passthrough
        stage_1_traverse(node->nd_block);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p4*>(root))
    {
        // Passthrough
        stage_1_traverse(node->nd_if);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p5*>(root))
    {
        // Passthrough
        stage_1_traverse(node->nd_loop);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p6*>(root))
    {
        // Passthrough
        stage_1_traverse(node->nd_assign);
    }
    else if (auto node = dynamic_cast<tree::node_in*>(root))
    {
        // Variable
        auto ref = new var {};
        ref->name = node->tk_identifier.content;

        // Code: Read to variable
        {
            auto instr = new gen_instr_read {};
            instr->dest = ref;
            m_gens.push_back(instr);
        }
    }
    else if (auto node = dynamic_cast<tree::node_out*>(root))
    {
        stage_1_traverse(node->nd_expr);

        // Temporary variable
        auto tmp = new var {};
        tmp->name = "";

        // Code: Store value (acc) into temporary
        {
            auto instr = new gen_instr_store {};
            instr->dest = tmp;
            m_gens.push_back(instr);
        }

        // Code: Write from temporary
        {
            auto instr = new gen_instr_write_var {};
            instr->src = tmp;
            m_gens.push_back(instr);
        }
    }
    else if (auto node = dynamic_cast<tree::node_if*>(root))
    {
        // Call rhs
        stage_1_traverse(node->nd_rhs);

        // Temporary variable
        auto tmp = new var {};
        tmp->name = "";

        // Code: Store rhs (acc) to rhs (tmp)
        {
            auto instr = new gen_instr_store {};
            instr->dest = tmp;
            m_gens.push_back(instr);
        }

        // Call lhs
        stage_1_traverse(node->nd_lhs);

        // Code: Subtract rhs (tmp) from lhs (acc)
        {
            auto instr = new gen_instr_sub_var {};
            instr->rhs = tmp;
            m_gens.push_back(instr);
        }

        // Call operator
        stage_1_traverse(node->nd_operator);

        // Jump label
        auto jmp = new label {};
        jmp->name = "";

        // Handle operator type
        switch (m_ro_type)
        {
        case ro_type::LT:
            {
                // OP: Less than

                // Code: Jump if diff >= 0
                auto instr = new gen_instr_brzpos {};
                instr->target = jmp;
                m_gens.push_back(instr);
            }
            break;
        case ro_type::LTE:
            {
                // OP: Less than or equal to

                // Code: Jump if diff > 0
                auto instr = new gen_instr_brpos {};
                instr->target = jmp;
                m_gens.push_back(instr);
            }
            break;
        case ro_type::GT:
            {
                // OP: Greater than

                // Code: Jump if diff <= 0
                auto instr = new gen_instr_brzneg {};
                instr->target = jmp;
                m_gens.push_back(instr);
            }
            break;
        case ro_type::GTE:
            {
                // OP: Greater than or equal to

                // Code: Jump if diff < 0
                auto instr = new gen_instr_brneg {};
                instr->target = jmp;
                m_gens.push_back(instr);
            }
            break;
        case ro_type::EQ:
            {
                // OP: Equal to

                // Code: Jump if diff < 0
                {
                    auto instr = new gen_instr_brneg {};
                    instr->target = jmp;
                    m_gens.push_back(instr);
                }

                // Code: Jump if diff > 0
                {
                    auto instr = new gen_instr_brpos {};
                    instr->target = jmp;
                    m_gens.push_back(instr);
                }
            }
            break;
        case ro_type::NEQ:
            {
                // OP: Not equal to

                // Code: Jump if diff == 0
                auto instr = new gen_instr_brzero {};
                instr->target = jmp;
                m_gens.push_back(instr);
            }
            break;
        }

        // Reset operator type
        m_ro_type = ro_type::NONE;

        // Call body block
        stage_1_traverse(node->nd_body);

        // Code: No operation (branch target)
        {
            auto instr = new gen_instr_noop {};
            m_gens.push_back(instr);
            jmp->target = instr;
        }
    }
    else if (auto node = dynamic_cast<tree::node_loop*>(root))
    {
        // TODO
    }
    else if (auto node = dynamic_cast<tree::node_assign*>(root))
    {
        // Call value
        stage_1_traverse(node->nd_value);

        // Variable
        auto ref = new var {};
        ref->name = node->tk_name.content;

        // Code: Store value (acc) to variable (ref)
        {
            auto instr = new gen_instr_store {};
            instr->dest = ref;
            m_gens.push_back(instr);
        }
    }
    else if (auto node = dynamic_cast<tree::node_RO_p1*>(root))
    {
        // Call optional second <
        stage_1_traverse(node->nd_lt);

        // If not "less than or equal to", it's just "less than"
        if (m_ro_type == ro_type::NONE)
        {
            m_ro_type = ro_type::LT;
        }
    }
    else if (auto node = dynamic_cast<tree::node_RO_p2*>(root))
    {
        // Call optional second >
        stage_1_traverse(node->nd_gt);

        // If not "greater than or equal to", it's just "greater than"
        if (m_ro_type == ro_type::NONE)
        {
            m_ro_type = ro_type::GT;
        }
    }
    else if (auto node = dynamic_cast<tree::node_RO_p3*>(root))
    {
        // Call optional second =
        stage_1_traverse(node->nd_eq);

        // If not "not equal to", it's just "equal to"
        if (m_ro_type == ro_type::NONE)
        {
            m_ro_type = ro_type::EQ;
        }
    }
    else if (auto node = dynamic_cast<tree::node_RO_lt*>(root))
    {
        // Less than or equal to
        m_ro_type = ro_type::LTE;
    }
    else if (auto node = dynamic_cast<tree::node_RO_gt*>(root))
    {
        // Greater than or equal to
        m_ro_type = ro_type::GTE;
    }
    else if (auto node = dynamic_cast<tree::node_RO_eq*>(root))
    {
        // Not equal to
        m_ro_type = ro_type::NEQ;
    }
}

void p4::codegen::do_stage_1()
{
    //
    // Stage 1: Roughly sketch the program
    //   * Produces a sequence of objects outlining the final code
    //   * No storage allocation is performed
    //       - Variable references are kept as strings
    //       - Temporary variables are named "" (empty string)
    //   * The parse tree only needs to be traversed once
    //

    // Perform traversal
    stage_1_traverse(m_tree);
}

void p4::codegen::do_stage_2()
{
    //
    // Stage 2: Allocate variables
    //

    // TODO: Create gens for var declarations and resolve their refs
    // This assigns global namespace names to global variables and stack offsets for local variables
    // This inserts stackr and stackw gens to enable local variable access
}

void p4::codegen::do_stage_3()
{
    //
    // Stage 3: Allocate labels
    //

    // TODO: Create gens for label declarations and resolve their refs
    // This assigns global namespace names to labels
}

void p4::codegen::compose()
{
    for (auto&& gen_i : m_gens)
    {
        if (auto g = dynamic_cast<gen_decl_label*>(gen_i))
        {
            m_output_ss << g->name << ":";
        }
        else if (auto g = dynamic_cast<gen_decl_var*>(gen_i))
        {
            m_output_ss << g->name << " " << g->value << "\n";
        }
        else if (auto g = dynamic_cast<gen_instr_br*>(gen_i))
        {
            m_output_ss << "BR <target>\n";
        }
        else if (auto g = dynamic_cast<gen_instr_brneg*>(gen_i))
        {
            m_output_ss << "BRNEG <target>\n";
        }
        else if (auto g = dynamic_cast<gen_instr_brzneg*>(gen_i))
        {
            m_output_ss << "BRZNEG <target>\n";
        }
        else if (auto g = dynamic_cast<gen_instr_brpos*>(gen_i))
        {
            m_output_ss << "BRPOS <target>\n";
        }
        else if (auto g = dynamic_cast<gen_instr_brzpos*>(gen_i))
        {
            m_output_ss << "BRZPOS <target>\n";
        }
        else if (auto g = dynamic_cast<gen_instr_brzero*>(gen_i))
        {
            m_output_ss << "BRZERO <target>\n";
        }
        else if (auto g = dynamic_cast<gen_instr_copy*>(gen_i))
        {
            m_output_ss << "COPY <dest> <src>\n";
        }
        else if (auto g = dynamic_cast<gen_instr_add_var*>(gen_i))
        {
            m_output_ss << "ADD <var>\n";
        }
        else if (auto g = dynamic_cast<gen_instr_add_imm*>(gen_i))
        {
            m_output_ss << "ADD " << g->rhs << "\n";
        }
        else if (auto g = dynamic_cast<gen_instr_sub_var*>(gen_i))
        {
            m_output_ss << "SUB <var>\n";
        }
        else if (auto g = dynamic_cast<gen_instr_sub_imm*>(gen_i))
        {
            m_output_ss << "SUB " << g->rhs << "\n";
        }
        else if (auto g = dynamic_cast<gen_instr_div_var*>(gen_i))
        {
            m_output_ss << "DIV <var>\n";
        }
        else if (auto g = dynamic_cast<gen_instr_div_imm*>(gen_i))
        {
            m_output_ss << "DIV " << g->rhs << "\n";
        }
        else if (auto g = dynamic_cast<gen_instr_mult_var*>(gen_i))
        {
            m_output_ss << "MULT <var>\n";
        }
        else if (auto g = dynamic_cast<gen_instr_mult_imm *>(gen_i))
        {
            m_output_ss << "MULT " << g->rhs << "\n";
        }
        else if (auto g = dynamic_cast<gen_instr_read*>(gen_i))
        {
            m_output_ss << "READ <var>\n";
        }
        else if (auto g = dynamic_cast<gen_instr_write_var*>(gen_i))
        {
            m_output_ss << "WRITE <var>\n";
        }
        else if (auto g = dynamic_cast<gen_instr_write_imm*>(gen_i))
        {
            m_output_ss << "WRITE " << g->value << "\n";
        }
        else if (auto g = dynamic_cast<gen_instr_stop*>(gen_i))
        {
            m_output_ss << "STOP\n";
        }
        else if (auto g = dynamic_cast<gen_instr_store*>(gen_i))
        {
            m_output_ss << "STORE <var>\n";
        }
        else if (auto g = dynamic_cast<gen_instr_load_var*>(gen_i))
        {
            m_output_ss << "LOAD <var>\n";
        }
        else if (auto g = dynamic_cast<gen_instr_load_imm*>(gen_i))
        {
            m_output_ss << "LOAD " << g->value << "\n";
        }
        else if (auto g = dynamic_cast<gen_instr_noop*>(gen_i))
        {
            m_output_ss << "NOOP\n";
        }
        else if (auto g = dynamic_cast<gen_instr_push*>(gen_i))
        {
            m_output_ss << "PUSH\n";
        }
        else if (auto g = dynamic_cast<gen_instr_pop*>(gen_i))
        {
            m_output_ss << "POP\n";
        }
        else if (auto g = dynamic_cast<gen_instr_stackw*>(gen_i))
        {
            m_output_ss << "STACKW " << g->offset << "\n";
        }
        else if (auto g = dynamic_cast<gen_instr_stackr*>(gen_i))
        {
            m_output_ss << "STACKR " << g->offset << "\n";
        }
    }

    m_output = std::move(m_output_ss.str());
    m_output_ss.str("");
    m_output_ss.clear();
}

void p4::codegen::run()
{
    do_stage_1();
    do_stage_2();
    do_stage_3();
    compose();
}
