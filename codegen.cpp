/*
 * Tyler Filla
 * CS 4280
 * Project 4
 */

#include <iostream>
#include "codegen.h"

p4::codegen::codegen()
        : m_tree(nullptr)
        , m_output {}
        , m_output_ss {}
        , m_frags {}
        , m_ro_type {}
{
}

void p4::codegen::stage_1_traverse(tree::node* root)
{
    if (!root)
        return;

    if (auto node = dynamic_cast<tree::node_program*>(root))
    {
        // Enter global scope
        m_frags.push_back(new frag_scope_enter {});

        // Call variables
        stage_1_traverse(node->nd_vars);

        // Call block
        stage_1_traverse(node->nd_block);

        // Leave global scope
        m_frags.push_back(new frag_scope_leave {});

        // Stop the program
        m_frags.push_back(new frag_ins_stop);
    }
    else if (auto node = dynamic_cast<tree::node_block*>(root))
    {
        // Enter local scope
        m_frags.push_back(new frag_scope_enter {});

        // Call variables
        stage_1_traverse(node->nd_vars);

        // Call statements
        stage_1_traverse(node->nd_stats);

        // Leave local scope
        m_frags.push_back(new frag_scope_leave {});
    }
    else if (auto node = dynamic_cast<tree::node_vars*>(root))
    {
        // The initial value
        int value;

        // Convert initial value to integer
        {
            std::stringstream ss;
            ss << node->tk_value.content;
            ss >> value;
        }

        // Declare the variable
        auto var = new frag_decl_var {};
        var->name = node->tk_name.content;
        var->value = value;
        m_frags.push_back(var);

        // Call more variables
        stage_1_traverse(node->nd_mvars);
    }
    else if (auto node = dynamic_cast<tree::node_mvars_p1*>(root))
    {
    }
    else if (auto node = dynamic_cast<tree::node_mvars_p2*>(root))
    {
        // Declare the variable and zero it
        auto var = new frag_decl_var {};
        var->name = node->tk_identifier.content;
        var->value = 0;
        m_frags.push_back(var);

        // Call more variables
        stage_1_traverse(node->nd_mvars);
    }
    else if (auto node = dynamic_cast<tree::node_expr*>(root))
    {
        // Call the expression components
        stage_1_traverse(node->nd_M);
        stage_1_traverse(node->nd_expr_2);
    }
    else if (auto node = dynamic_cast<tree::node_expr_2_p1*>(root))
    {
        // Create temporary variable
        auto tmp_lhs = new frag_ref_var {};
        tmp_lhs->name = "";
        m_frags.push_back(tmp_lhs);

        // Store lhs (acc) to lhs (tmp_lhs)
        {
            auto i = new frag_ins_store {};
            i->dest = tmp_lhs;
            m_frags.push_back(i);
        }

        // Call rhs
        stage_1_traverse(node->nd_rhs);

        // Add lhs (tmp_lhs) to rhs (acc)
        {
            auto i = new frag_ins_add_var {};
            i->rhs = tmp_lhs;
            m_frags.push_back(i);
        }
    }
    else if (auto node = dynamic_cast<tree::node_expr_2_p2*>(root))
    {
        // Reserve a stack slot
        m_frags.push_back(new frag_ins_push {});

        // Copy lhs (acc) to stack
        {
            auto i = new frag_ins_stackw {};
            i->offset = 0;
            m_frags.push_back(i);
        }

        // Call rhs
        stage_1_traverse(node->nd_rhs);

        // Create temporary variable
        auto tmp_rhs = new frag_ref_var {};
        tmp_rhs->name = "";

        // Store rhs (acc) to rhs (tmp_rhs)
        {
            auto i = new frag_ins_store {};
            i->dest = tmp_rhs;
            m_frags.push_back(i);
        }

        // Load lhs (stack 0) to lhs (acc)
        m_frags.push_back(new frag_ins_stackr {});

        // Subtract rhs (tmp_rhs) from lhs (acc)
        {
            auto i = new frag_ins_sub_var {};
            i->rhs = tmp_rhs;
            m_frags.push_back(i);
        }

        // Free stack space
        m_frags.push_back(new frag_ins_pop {});
    }
    else if (auto node = dynamic_cast<tree::node_expr_2_p3*>(root))
    {
        // Reserve a stack slot
        m_frags.push_back(new frag_ins_push {});

        // Copy lhs (acc) to stack
        {
            auto i = new frag_ins_stackw {};
            i->offset = 0;
            m_frags.push_back(i);
        }

        // Call rhs
        stage_1_traverse(node->nd_rhs);

        // Create temporary variable
        auto tmp_rhs = new frag_ref_var {};
        tmp_rhs->name = "";

        // Store rhs (acc) to rhs (tmp_rhs)
        {
            auto i = new frag_ins_store {};
            i->dest = tmp_rhs;
            m_frags.push_back(i);
        }

        // Load lhs (stack 0) to lhs (acc)
        m_frags.push_back(new frag_ins_stackr {});

        // Divide lhs (acc) by rhs (tmp_rhs)
        {
            auto i = new frag_ins_div_var {};
            i->rhs = tmp_rhs;
            m_frags.push_back(i);
        }

        // Free stack space
        m_frags.push_back(new frag_ins_pop {});
    }
    else if (auto node = dynamic_cast<tree::node_expr_2_p4*>(root))
    {
        // Create temporary variable
        auto tmp_lhs = new frag_ref_var {};
        tmp_lhs->name = "";
        m_frags.push_back(tmp_lhs);

        // Store lhs (acc) to lhs (tmp_lhs)
        {
            auto i = new frag_ins_store {};
            i->dest = tmp_lhs;
            m_frags.push_back(i);
        }

        // Call rhs
        stage_1_traverse(node->nd_rhs);

        // Multiply lhs (tmp_lhs) to rhs (acc)
        {
            auto i = new frag_ins_mult_var {};
            i->rhs = tmp_lhs;
            m_frags.push_back(i);
        }
    }
    else if (auto node = dynamic_cast<tree::node_M_p1*>(root))
    {
        // Call body expressions
        stage_1_traverse(node->nd_M);

        // Multiply by -1
        {
            auto i = new frag_ins_mult_imm {};
            i->rhs = -1;
            m_frags.push_back(i);
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
        // Variable reference
        auto ref = new frag_ref_var {};
        ref->name = node->tk_identifier.content;
        m_frags.push_back(ref);

        // Load value (ref)
        {
            auto i = new frag_ins_load_var {};
            i->src = ref;
            m_frags.push_back(i);
        }
    }
    else if (auto node = dynamic_cast<tree::node_R_p3*>(root))
    {
        // The integer value
        int value;

        // Convert token content to an integer
        {
            std::stringstream ss {};
            ss << node->tk_integer.content;
            ss >> value;
        }

        // Load value (immediate)
        {
            auto i = new frag_ins_load_imm {};
            i->src = value;
            m_frags.push_back(i);
        }
    }
    else if (auto node = dynamic_cast<tree::node_stats*>(root))
    {
        // Call statement
        stage_1_traverse(node->nd_stat);

        // Call more statements
        stage_1_traverse(node->nd_mStat);
    }
    else if (auto node = dynamic_cast<tree::node_mStat*>(root))
    {
        // Call statement
        stage_1_traverse(node->nd_stat);

        // Call more statements
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
        // Variable reference
        auto ref = new frag_ref_var {};
        ref->name = node->tk_identifier.content;
        m_frags.push_back(ref);

        // Read to variable
        {
            auto i = new frag_ins_read {};
            i->dest = ref;
            m_frags.push_back(i);
        }
    }
    else if (auto node = dynamic_cast<tree::node_out*>(root))
    {
        // Call operand expression
        stage_1_traverse(node->nd_expr);

        // Create temporary variable
        auto tmp = new frag_ref_var {};
        tmp->name = "";
        m_frags.push_back(tmp);

        // Store operand value (acc) into temporary
        {
            auto i = new frag_ins_store {};
            i->dest = tmp;
            m_frags.push_back(i);
        }

        // Write from temporary
        {
            auto i = new frag_ins_write_var {};
            i->src = tmp;
            m_frags.push_back(i);
        }
    }
    else if (auto node = dynamic_cast<tree::node_if*>(root))
    {
        // Call rhs
        stage_1_traverse(node->nd_rhs);

        // Create temporary variable
        auto tmp = new frag_ref_var {};
        tmp->name = "";
        m_frags.push_back(tmp);

        // Store rhs value (acc) to rhs (tmp)
        {
            auto i = new frag_ins_store {};
            i->dest = tmp;
            m_frags.push_back(i);
        }

        // Call lhs
        stage_1_traverse(node->nd_lhs);

        // Subtract rhs (tmp) from lhs (acc)
        {
            auto i = new frag_ins_sub_var {};
            i->rhs = tmp;
            m_frags.push_back(i);
        }

        // Call operator
        // This does not generate code
        stage_1_traverse(node->nd_operator);

        // Create jump label
        auto jmp = new frag_ref_label {};
        jmp->name = "";

        // Handle operator
        switch (m_ro_type)
        {
        case ro_type::LT:
            {
                // OP: Less than

                // Jump if diff >= 0
                auto i = new frag_ins_brzpos {};
                i->target = jmp;
                m_frags.push_back(i);
            }
            break;
        case ro_type::LTE:
            {
                // OP: Less than or equal to

                // Jump if diff > 0
                auto i = new frag_ins_brpos {};
                i->target = jmp;
                m_frags.push_back(i);
            }
            break;
        case ro_type::GT:
            {
                // OP: Greater than

                // Jump if diff <= 0
                auto i = new frag_ins_brzneg {};
                i->target = jmp;
                m_frags.push_back(i);
            }
            break;
        case ro_type::GTE:
            {
                // OP: Greater than or equal to

                // Jump if diff < 0
                auto i = new frag_ins_brneg {};
                i->target = jmp;
                m_frags.push_back(i);
            }
            break;
        case ro_type::EQ:
            {
                // OP: Equal to

                // Jump if diff < 0
                {
                    auto i = new frag_ins_brneg {};
                    i->target = jmp;
                    m_frags.push_back(i);
                }

                // Jump if diff > 0
                {
                    auto i = new frag_ins_brpos {};
                    i->target = jmp;
                    m_frags.push_back(i);
                }
            }
            break;
        case ro_type::NEQ:
            {
                // OP: Not equal to

                // Jump if diff == 0
                auto i = new frag_ins_brzero {};
                i->target = jmp;
                m_frags.push_back(i);
            }
            break;
        }

        // Reset operator type
        m_ro_type = ro_type::NONE;

        // Call body block
        stage_1_traverse(node->nd_body);

        // Append jump label after body
        m_frags.push_back(jmp);
    }
    else if (auto node = dynamic_cast<tree::node_loop*>(root))
    {
        // TODO: Implement loop
    }
    else if (auto node = dynamic_cast<tree::node_assign*>(root))
    {
        // Call value expression
        stage_1_traverse(node->nd_value);

        // Variable reference
        auto ref = new frag_ref_var {};
        ref->name = node->tk_name.content;
        m_frags.push_back(ref);

        // Store value (acc) to variable (ref)
        {
            auto i = new frag_ins_store {};
            i->dest = ref;
            m_frags.push_back(i);
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
    //   * Produces a sequence of fragments outlining the final program
    //   * No storage allocation is performed
    //       - Variable references are kept as strings
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

    // TODO: Preserve initial value

    // The variables encountered so far
    std::vector<std::string> vars;

    // The scopes encountered so far
    // These are indexes into the vars array
    std::vector<int> scopes;

    for (auto i = m_frags.begin(); i != m_frags.end(); ++i)
    {
        auto&& frag_i = *i;

        if (auto f = dynamic_cast<frag_decl_label*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_decl_var*>(frag_i))
        {
            // Store variable in current scope
            vars.push_back(f->name);

            // Erase declaration and revalidate iterator
            auto idx = i - m_frags.begin();
            m_frags.erase(i);
            i = m_frags.begin() + (idx - 1);
        }
        else if (auto f = dynamic_cast<frag_ref_label*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ref_var*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_br*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_brneg*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_brzneg*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_brpos*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_brzpos*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_brzero*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_copy*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_add_var*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_add_imm*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_sub_var*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_sub_imm*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_div_var*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_div_imm*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_mult_var*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_mult_imm*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_read*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_write_var*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_write_imm*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_stop*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_store*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_load_var*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_load_imm*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_noop*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_push*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_pop*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_stackw*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_stackr*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_scope_enter*>(frag_i))
        {
            scopes.push_back(vars.size());
        }
        else if (auto f = dynamic_cast<frag_scope_leave*>(frag_i))
        {
            // If this leave fragment does not correspond to the global scope
            if (scopes.size() > 1)
            {
                // Forget all non-global variables
                // We need the globals to stick around for proper (global) allocation later
                vars.erase(vars.begin() + scopes.back(), vars.end());

                // Pop scope
                scopes.erase(scopes.end() - 1);
            }
        }
    }

    // Declare global variables for real
    for (auto&& v : vars)
    {
        auto f = new frag_decl_var {};
        f->name = v;
        m_frags.push_back(f);

        // TODO: Escape them so they don't collide with temporaries
    }
}

void p4::codegen::do_stage_3()
{
    //
    // Stage 3: Allocate labels
    //

    // TODO: Create gens for label declarations and resolve their refs
    // This assigns global namespace names to labels

    for (auto i = m_frags.begin(); i != m_frags.end(); ++i)
    {
        auto&& frag_i = *i;

        if (auto f = dynamic_cast<frag_decl_label*>(frag_i))
        {
            // TODO: Store label

            // Erase declaration and revalidate iterator
            auto idx = i - m_frags.begin();
            m_frags.erase(i);
            i = m_frags.begin() + (idx - 1);
        }
        else if (auto f = dynamic_cast<frag_decl_var*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ref_label*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ref_var*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_br*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_brneg*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_brzneg*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_brpos*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_brzpos*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_brzero*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_copy*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_add_var*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_add_imm*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_sub_var*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_sub_imm*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_div_var*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_div_imm*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_mult_var*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_mult_imm*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_read*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_write_var*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_write_imm*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_stop*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_store*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_load_var*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_load_imm*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_noop*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_push*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_pop*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_stackw*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_ins_stackr*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_scope_enter*>(frag_i))
        {
        }
        else if (auto f = dynamic_cast<frag_scope_leave*>(frag_i))
        {
        }
    }
}

void p4::codegen::compose()
{
    for (auto&& frag_i : m_frags)
    {
        if (auto f = dynamic_cast<frag_decl_label*>(frag_i))
        {
            m_output_ss << f->name << ": ";
        }
        else if (auto f = dynamic_cast<frag_decl_var*>(frag_i))
        {
            m_output_ss << f->name << " " << f->value << "\n";
        }
        else if (auto f = dynamic_cast<frag_ref_label*>(frag_i))
        {
            m_output_ss << "(ref_label) " << f->name << "\n";
        }
        else if (auto f = dynamic_cast<frag_ref_var*>(frag_i))
        {
            m_output_ss << "(ref_var) " << f->name << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_br*>(frag_i))
        {
            m_output_ss << "BR <ref_label>\n";
        }
        else if (auto f = dynamic_cast<frag_ins_brneg*>(frag_i))
        {
            m_output_ss << "BRNEG <ref_label>\n";
        }
        else if (auto f = dynamic_cast<frag_ins_brzneg*>(frag_i))
        {
            m_output_ss << "BRZNEG <ref_label>\n";
        }
        else if (auto f = dynamic_cast<frag_ins_brpos*>(frag_i))
        {
            m_output_ss << "BRPOS <ref_label>\n";
        }
        else if (auto f = dynamic_cast<frag_ins_brzpos*>(frag_i))
        {
            m_output_ss << "BRZPOS <ref_label>\n";
        }
        else if (auto f = dynamic_cast<frag_ins_brzero*>(frag_i))
        {
            m_output_ss << "BRZERO <ref_label>\n";
        }
        else if (auto f = dynamic_cast<frag_ins_copy*>(frag_i))
        {
            m_output_ss << "COPY <ref_var> <ref_var>\n";
        }
        else if (auto f = dynamic_cast<frag_ins_add_var*>(frag_i))
        {
//          f->rhs;
            m_output_ss << "ADD <ref_var>\n";
        }
        else if (auto f = dynamic_cast<frag_ins_add_imm*>(frag_i))
        {
            m_output_ss << "ADD " << f->rhs << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_sub_var*>(frag_i))
        {
//          f->rhs;
            m_output_ss << "SUB <ref_var>\n";
        }
        else if (auto f = dynamic_cast<frag_ins_sub_imm*>(frag_i))
        {
            m_output_ss << "SUB " << f->rhs << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_div_var*>(frag_i))
        {
//          f->rhs;
            m_output_ss << "DIV <ref_var>\n";
        }
        else if (auto f = dynamic_cast<frag_ins_div_imm*>(frag_i))
        {
//          f->rhs;
            m_output_ss << "DIV " << f->rhs << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_mult_var*>(frag_i))
        {
//          f->rhs;
            m_output_ss << "MULT <ref_var>\n";
        }
        else if (auto f = dynamic_cast<frag_ins_mult_imm*>(frag_i))
        {
            m_output_ss << "MULT " << f->rhs << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_read*>(frag_i))
        {
//          f->dest;
            m_output_ss << "READ <ref_var>\n";
        }
        else if (auto f = dynamic_cast<frag_ins_write_var*>(frag_i))
        {
//          f->src;
            m_output_ss << "WRITE <ref_var>\n";
        }
        else if (auto f = dynamic_cast<frag_ins_write_imm*>(frag_i))
        {
            m_output_ss << "WRITE " << f->src << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_stop*>(frag_i))
        {
            m_output_ss << "STOP\n";
        }
        else if (auto f = dynamic_cast<frag_ins_store*>(frag_i))
        {
//          f->dest;
            m_output_ss << "STORE <ref_var>\n";
        }
        else if (auto f = dynamic_cast<frag_ins_load_var*>(frag_i))
        {
//          f->src;
            m_output_ss << "LOAD <ref_var>\n";
        }
        else if (auto f = dynamic_cast<frag_ins_load_imm*>(frag_i))
        {
            m_output_ss << "LOAD " << f->src << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_noop*>(frag_i))
        {
            m_output_ss << "NOOP\n";
        }
        else if (auto f = dynamic_cast<frag_ins_push*>(frag_i))
        {
            m_output_ss << "PUSH\n";
        }
        else if (auto f = dynamic_cast<frag_ins_pop*>(frag_i))
        {
            m_output_ss << "POP\n";
        }
        else if (auto f = dynamic_cast<frag_ins_stackw*>(frag_i))
        {
            m_output_ss << "STACKW " << f->offset << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_stackr*>(frag_i))
        {
            m_output_ss << "STACKR " << f->offset << "\n";
        }
        else if (auto f = dynamic_cast<frag_scope_enter*>(frag_i))
        {
            m_output_ss << "<scope_enter>\n";
        }
        else if (auto f = dynamic_cast<frag_scope_leave*>(frag_i))
        {
            m_output_ss << "<scope_leave>\n";
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
