/*
 * Tyler Filla
 * CS 4280
 * Project 4
 */

#include <iostream> // FIXME
#include <limits>
#include "codegen.h"

p4::codegen::codegen()
        : m_tree(nullptr)
        , m_output {}
        , m_output_ss {}
        , m_frags {}
        , m_globals {}
        , m_vars {}
        , m_scopes {}
        , m_next_global(-1)
        , m_labels {}
        , m_next_label(0)
        , m_ro_type {}
{
}

void p4::codegen::push_scope()
{
    // Push scope
    m_scopes.push_back(m_vars.size());
}

void p4::codegen::pop_scope()
{
    // If not global scope
    if (m_scopes.size() > 1)
    {
        // Pop all local variables
        for (int i = 0; i < m_vars.size() - m_scopes.back(); ++i)
        {
            m_frags.push_back(new frag_ins_pop {});
        }

        // Forget all non-global variables
        // We need the globals to stick around for proper (global) allocation later
        m_vars.erase(m_vars.begin() + m_scopes.back(), m_vars.end());

        // Pop scope
        m_scopes.erase(m_scopes.end() - 1);
    }
    else
    {
        // Declare all global variables
        for (int i = -1; i > m_next_global; --i)
        {
            auto&& g = m_globals[i];

            auto decl = new frag_decl_gvar {};
            decl->name = g.first;
            decl->value = g.second;
            m_frags.push_back(decl);
        }
    }
}

std::string p4::codegen::mangle_user_variable(std::string name)
{
    return std::string("USR_") + name;
}

int p4::codegen::locate_variable(std::string name)
{
    auto&& name_mangled = mangle_user_variable(name);

    for (int i = m_vars.size() - 1; i >= 0; --i)
    {
        if (m_vars[i] == name_mangled)
        {
            // If variable is global
            if (m_scopes.size() == 1 || (m_scopes.size() > 1 && i < m_scopes[1]))
            {
                // The variable is global
                // Look up its ordinal value

                for (auto&& g : m_globals)
                {
                    if (g.second.first == name_mangled)
                        return g.first;
                }
            }
            else
            {
                // The variable is local
                // Calculate its stack offset

                return (m_vars.size() - 1 - m_scopes.back()) - (i - m_scopes.back());
            }

            // Variable found
            // After static analysis, this is guaranteed
            break;
        }
    }

    return std::numeric_limits<int>::max();
}

int p4::codegen::make_temp_gvar(std::string name, int value)
{
    int var = m_next_global--;

    // Build name
    std::ostringstream ss;
    ss << "TMP_" << -var << "_" << name;

    // Map variable
    m_globals[var] = std::make_pair(ss.str(), value);

    return var;
}

int p4::codegen::reserve_label(std::string name)
{
    int label = m_next_label++;

    // Build name
    std::ostringstream ss;
    ss << "JMP_" << label << "_" << name;

    // Map label
    m_labels[label] = ss.str();

    return label;
}

void p4::codegen::place_label(int label)
{
    // If the last fragment so far is already a label
    // This is problematic, as two labels cannot follow back-to-back
    if (auto l = dynamic_cast<frag_decl_label*>(m_frags.back()))
    {
        // Insert a no-op instruction
        m_frags.push_back(new frag_ins_noop {});
    }

    // Look up label name
    auto&& name = m_labels[label];

    // Insert label declaration
    auto i = new frag_decl_label {};
    i->name = name;
    m_frags.push_back(i);
}

void p4::codegen::traverse(tree::node* root)
{
    if (!root)
        return;

    if (auto node = dynamic_cast<tree::node_program*>(root))
    {
        push_scope();

        // Call variables
        traverse(node->nd_vars);

        // Call block
        traverse(node->nd_block);

        // Stop the program
        m_frags.push_back(new frag_ins_stop);

        pop_scope();
    }
    else if (auto node = dynamic_cast<tree::node_block*>(root))
    {
        push_scope();

        // Call variables
        traverse(node->nd_vars);

        // Call statements
        traverse(node->nd_stats);

        pop_scope();
    }
    else if (auto node = dynamic_cast<tree::node_vars*>(root))
    {
        auto&& name = mangle_user_variable(node->tk_name.content);

        // Add variable name to current scope
        m_vars.push_back(name);

        // The initial value
        int value;

        // Convert initial value to integer
        {
            std::stringstream ss;
            ss << node->tk_value.content;
            ss >> value;
        }

        // If variable is local
        if (m_scopes.size() > 1)
        {
            // Load initial value
            {
                auto i = new frag_ins_load_imm {};
                i->src = value;
                m_frags.push_back(i);
            }

            // Push variable onto stack
            m_frags.push_back(new frag_ins_push {});
            m_frags.push_back(new frag_ins_stackw {});
        }
        else
        {
            // Assign a number to the global
            m_globals[m_next_global--] = std::make_pair(name, value);
        }

        // Call more variables
        traverse(node->nd_mvars);
    }
    else if (auto node = dynamic_cast<tree::node_mvars_p1*>(root))
    {
    }
    else if (auto node = dynamic_cast<tree::node_mvars_p2*>(root))
    {
        auto&& name = mangle_user_variable(node->tk_identifier.content);

        // Add variable name to current scope
        m_vars.push_back(name);

        // If variable is local
        if (m_scopes.size() > 1)
        {
            // Push zeroed variable onto stack
            m_frags.push_back(new frag_ins_load_imm {});
            m_frags.push_back(new frag_ins_push {});
            m_frags.push_back(new frag_ins_stackw {});
        }
        else
        {
            // Assign a number to the global
            m_globals[m_next_global--] = std::make_pair(name, 0);
        }

        // Call more variables
        traverse(node->nd_mvars);
    }
    else if (auto node = dynamic_cast<tree::node_expr*>(root))
    {
        // Call the expression components
        traverse(node->nd_M);
        traverse(node->nd_expr_2);
    }
    else if (auto node = dynamic_cast<tree::node_expr_2_p1*>(root))
    {
        // Create temporary global
        int tmp_lhs = make_temp_gvar("add_lhs");

        // Store lhs (acc) to lhs (tmp_lhs)
        {
            auto i = new frag_ins_store_gvar {};
            i->dest = tmp_lhs;
            m_frags.push_back(i);
        }

        // Call rhs
        traverse(node->nd_rhs);

        // Add lhs (tmp_lhs) to rhs (acc)
        {
            auto i = new frag_ins_add_gvar {};
            i->rhs = tmp_lhs;
            m_frags.push_back(i);
        }
    }
    else if (auto node = dynamic_cast<tree::node_expr_2_p2*>(root))
    {
        // Reserve a stack slot
        m_frags.push_back(new frag_ins_push {});

        // Write lhs (acc) to stack
        {
            auto i = new frag_ins_stackw {};
            i->offset = 0;
            m_frags.push_back(i);
        }

        // Call rhs
        traverse(node->nd_rhs);

        // Create temporary global
        int tmp_rhs = make_temp_gvar("sub_rhs");

        // Store rhs (acc) to rhs (tmp_rhs)
        {
            auto i = new frag_ins_store_gvar {};
            i->dest = tmp_rhs;
            m_frags.push_back(i);
        }

        // Pop lhs (stack 0) to lhs (acc)
        m_frags.push_back(new frag_ins_stackr {});
        m_frags.push_back(new frag_ins_pop {});

        // Subtract rhs (tmp_rhs) from lhs (acc)
        {
            auto i = new frag_ins_sub_gvar {};
            i->rhs = tmp_rhs;
            m_frags.push_back(i);
        }
    }
    else if (auto node = dynamic_cast<tree::node_expr_2_p3*>(root))
    {
        // Reserve a stack slot
        m_frags.push_back(new frag_ins_push {});

        // Write lhs (acc) to stack
        {
            auto i = new frag_ins_stackw {};
            i->offset = 0;
            m_frags.push_back(i);
        }

        // Call rhs
        traverse(node->nd_rhs);

        // Create temporary global
        int tmp_rhs = make_temp_gvar("div_rhs");

        // Store rhs (acc) to rhs (tmp_rhs)
        {
            auto i = new frag_ins_store_gvar {};
            i->dest = tmp_rhs;
            m_frags.push_back(i);
        }

        // Pop lhs (stack 0) to lhs (acc)
        m_frags.push_back(new frag_ins_stackr {});
        m_frags.push_back(new frag_ins_pop {});

        // Divide lhs (acc) by rhs (tmp_rhs)
        {
            auto i = new frag_ins_div_gvar {};
            i->rhs = tmp_rhs;
            m_frags.push_back(i);
        }
    }
    else if (auto node = dynamic_cast<tree::node_expr_2_p4*>(root))
    {
        // Create temporary global
        int tmp_lhs = make_temp_gvar("mult_lhs");

        // Store lhs (acc) to lhs (tmp_lhs)
        {
            auto i = new frag_ins_store_gvar {};
            i->dest = tmp_lhs;
            m_frags.push_back(i);
        }

        // Call rhs
        traverse(node->nd_rhs);

        // Multiply rhs (acc) by lhs (tmp_lhs)
        {
            auto i = new frag_ins_mult_gvar {};
            i->rhs = tmp_lhs;
            m_frags.push_back(i);
        }
    }
    else if (auto node = dynamic_cast<tree::node_M_p1*>(root))
    {
        // Call body expressions
        traverse(node->nd_M);

        // Multiply by -1
        {
            auto i = new frag_ins_mult_imm {};
            i->rhs = -1;
            m_frags.push_back(i);
        }
    }
    else if (auto node = dynamic_cast<tree::node_M_p2*>(root))
    {
        // Call R
        traverse(node->nd_R);
    }
    else if (auto node = dynamic_cast<tree::node_R_p1*>(root))
    {
        // Call expression
        traverse(node->nd_expr);
    }
    else if (auto node = dynamic_cast<tree::node_R_p2*>(root))
    {
        // Locate referenced variable
        int var = locate_variable(node->tk_identifier.content);

        if (var < 0)
        {
            // Global
            auto i = new frag_ins_load_gvar {};
            i->src = var;
            m_frags.push_back(i);
        }
        else
        {
            // Local
            auto i = new frag_ins_stackr {};
            i->offset = var;
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
        traverse(node->nd_stat);

        // Call more statements
        traverse(node->nd_mStat);
    }
    else if (auto node = dynamic_cast<tree::node_mStat*>(root))
    {
        // Call statement
        traverse(node->nd_stat);

        // Call more statements
        traverse(node->nd_mStat);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p1*>(root))
    {
        // Call in
        traverse(node->nd_in);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p2*>(root))
    {
        // Call out
        traverse(node->nd_out);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p3*>(root))
    {
        // Call block
        traverse(node->nd_block);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p4*>(root))
    {
        // Call if
        traverse(node->nd_if);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p5*>(root))
    {
        // Call loop
        traverse(node->nd_loop);
    }
    else if (auto node = dynamic_cast<tree::node_stat_p6*>(root))
    {
        // Call assign
        traverse(node->nd_assign);
    }
    else if (auto node = dynamic_cast<tree::node_in*>(root))
    {
        // Locate referenced variable
        int var = locate_variable(node->tk_identifier.content);

        if (var < 0)
        {
            // Global

            // Read directly to global
            auto i = new frag_ins_read_gvar {};
            i->dest = var;
            m_frags.push_back(i);
        }
        else
        {
            // Local

            // Create temporary global
            int tmp = make_temp_gvar("read");

            // Read to temporary
            {
                auto i = new frag_ins_read_gvar {};
                i->dest = tmp;
                m_frags.push_back(i);
            }

            // Load from temporary
            {
                auto i = new frag_ins_load_gvar {};
                i->src = tmp;
                m_frags.push_back(i);
            }

            // Store to local
            {
                auto i = new frag_ins_stackw {};
                i->offset = var;
                m_frags.push_back(i);
            }
        }
    }
    else if (auto node = dynamic_cast<tree::node_out*>(root))
    {
        // Call operand expression
        traverse(node->nd_expr);

        // Create temporary global
        int tmp = make_temp_gvar("write");

        // Store operand value (acc) into temporary
        {
            auto i = new frag_ins_store_gvar {};
            i->dest = tmp;
            m_frags.push_back(i);
        }

        // Write from temporary
        {
            auto i = new frag_ins_write_gvar {};
            i->src = tmp;
            m_frags.push_back(i);
        }
    }
    else if (auto node = dynamic_cast<tree::node_if*>(root))
    {
        // Call rhs
        traverse(node->nd_rhs);

        // Create temporary global
        int tmp_rhs = make_temp_gvar("iff_rhs");

        // Store rhs value (acc) to rhs (tmp_rhs)
        {
            auto i = new frag_ins_store_gvar {};
            i->dest = tmp_rhs;
            m_frags.push_back(i);
        }

        // Call lhs
        traverse(node->nd_lhs);

        // Subtract rhs (tmp_rhs) from lhs (acc)
        {
            auto i = new frag_ins_sub_gvar {};
            i->rhs = tmp_rhs;
            m_frags.push_back(i);
        }

        // Call operator (not code-generating)
        traverse(node->nd_operator);

        // Make jump label
        int jmp = reserve_label("iff");

        // Handle operator
        switch (m_ro_type)
        {
        case ro_type::LT:
            {
                // OP: Less than

                // Bail if diff >= 0
                auto i = new frag_ins_brzpos {};
                i->target = jmp;
                m_frags.push_back(i);
            }
            break;
        case ro_type::LTE:
            {
                // OP: Less than or equal to

                // Bail if diff > 0
                auto i = new frag_ins_brpos {};
                i->target = jmp;
                m_frags.push_back(i);
            }
            break;
        case ro_type::GT:
            {
                // OP: Greater than

                // Bail if diff <= 0
                auto i = new frag_ins_brzneg {};
                i->target = jmp;
                m_frags.push_back(i);
            }
            break;
        case ro_type::GTE:
            {
                // OP: Greater than or equal to

                // Bail if diff < 0
                auto i = new frag_ins_brneg {};
                i->target = jmp;
                m_frags.push_back(i);
            }
            break;
        case ro_type::EQ:
            {
                // OP: Equal to

                // Bail if diff < 0
                {
                    auto i = new frag_ins_brneg {};
                    i->target = jmp;
                    m_frags.push_back(i);
                }

                // Bail if diff > 0
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

                // Bail if diff == 0
                auto i = new frag_ins_brzero {};
                i->target = jmp;
                m_frags.push_back(i);
            }
            break;
        }

        // Reset operator type
        m_ro_type = ro_type::NONE;

        // Call body statement
        traverse(node->nd_body);

        // Place jump label
        place_label(jmp);
    }
    else if (auto node = dynamic_cast<tree::node_loop*>(root))
    {
        // Reserve in and out jump labels
        int jmp_in = reserve_label("iter_in");
        int jmp_out = reserve_label("iter_out");

        // Place in jump label
        place_label(jmp_in);

        // Call rhs
        traverse(node->nd_rhs);

        // Create temporary global
        int tmp_rhs = make_temp_gvar("loop_rhs");

        // Store rhs value (acc) to rhs (tmp_rhs)
        {
            auto i = new frag_ins_store_gvar {};
            i->dest = tmp_rhs;
            m_frags.push_back(i);
        }

        // Call lhs
        traverse(node->nd_lhs);

        // Subtract rhs (tmp) from lhs (acc)
        {
            auto i = new frag_ins_sub_gvar {};
            i->rhs = tmp_rhs;
            m_frags.push_back(i);
        }

        // Call operator (not code-generating)
        traverse(node->nd_operator);

        // Handle operator
        switch (m_ro_type)
        {
        case ro_type::LT:
            {
                // OP: Less than

                // Bail if diff >= 0
                auto i = new frag_ins_brzpos {};
                i->target = jmp_out;
                m_frags.push_back(i);
            }
            break;
        case ro_type::LTE:
            {
                // OP: Less than or equal to

                // Bail if diff > 0
                auto i = new frag_ins_brpos {};
                i->target = jmp_out;
                m_frags.push_back(i);
            }
            break;
        case ro_type::GT:
            {
                // OP: Greater than

                // Bail if diff <= 0
                auto i = new frag_ins_brzneg {};
                i->target = jmp_out;
                m_frags.push_back(i);
            }
            break;
        case ro_type::GTE:
            {
                // OP: Greater than or equal to

                // Bail if diff < 0
                auto i = new frag_ins_brneg {};
                i->target = jmp_out;
                m_frags.push_back(i);
            }
            break;
        case ro_type::EQ:
            {
                // OP: Equal to

                // Bail if diff < 0
                {
                    auto i = new frag_ins_brneg {};
                    i->target = jmp_out;
                    m_frags.push_back(i);
                }

                // Bail if diff > 0
                {
                    auto i = new frag_ins_brpos {};
                    i->target = jmp_out;
                    m_frags.push_back(i);
                }
            }
            break;
        case ro_type::NEQ:
            {
                // OP: Not equal to

                // Bail if diff == 0
                auto i = new frag_ins_brzero {};
                i->target = jmp_out;
                m_frags.push_back(i);
            }
            break;
        }

        // Reset operator type
        m_ro_type = ro_type::NONE;

        // Call body statment
        traverse(node->nd_body);

        // Jump back in
        {
            auto i = new frag_ins_br {};
            i->target = jmp_in;
            m_frags.push_back(i);
        }

        // Place out jump label
        place_label(jmp_out);
    }
    else if (auto node = dynamic_cast<tree::node_assign*>(root))
    {
        // Call value expression
        traverse(node->nd_value);

        // Locate referenced variable
        int var = locate_variable(node->tk_name.content);

        if (var < 0)
        {
            // Global

            // Store value (acc) to global variable
            auto i = new frag_ins_store_gvar {};
            i->dest = var;
            m_frags.push_back(i);
        }
        else
        {
            // Local

            // Write value (acc) to local variable
            auto i = new frag_ins_stackw {};
            i->offset = var;
            m_frags.push_back(i);
        }
    }
    else if (auto node = dynamic_cast<tree::node_RO_p1*>(root))
    {
        // Call optional second <
        traverse(node->nd_lt);

        // If not "less than or equal to", it's just "less than"
        if (m_ro_type == ro_type::NONE)
        {
            m_ro_type = ro_type::LT;
        }
    }
    else if (auto node = dynamic_cast<tree::node_RO_p2*>(root))
    {
        // Call optional second >
        traverse(node->nd_gt);

        // If not "greater than or equal to", it's just "greater than"
        if (m_ro_type == ro_type::NONE)
        {
            m_ro_type = ro_type::GT;
        }
    }
    else if (auto node = dynamic_cast<tree::node_RO_p3*>(root))
    {
        // Call optional second =
        traverse(node->nd_eq);

        // If not "equal to", it's "not equal to"
        if (m_ro_type == ro_type::NONE)
        {
            m_ro_type = ro_type::NEQ;
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
        // Equal to
        m_ro_type = ro_type::EQ;
    }
}

void p4::codegen::optimize()
{
    frag* frag_prev = nullptr;

    for (auto i = m_frags.begin(); i != m_frags.end(); ++i)
    {
        if (auto f1 = dynamic_cast<frag_ins_store_gvar*>(*i))
        {
            if (!frag_prev)
                continue;

            if (auto f2 = dynamic_cast<frag_ins_load_gvar*>(frag_prev))
            {
                int dest = f1->dest;
                int src = f2->src;

                delete f1;
                delete f2;

                // Replace store with direct copy
                {
                    auto c = new frag_ins_copy {};
                    c->dest = dest;
                    c->src = src;
                    *i = c;
                }

                // Erase load
                auto idx = i - m_frags.begin();
                m_frags.erase(i - 1, i);
                i = m_frags.begin() + idx;
            }
        }

        frag_prev = *i;
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
        else if (auto f = dynamic_cast<frag_decl_gvar*>(frag_i))
        {
            m_output_ss << f->name << " " << f->value << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_br*>(frag_i))
        {
            m_output_ss << "BR " << m_labels[f->target] << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_brneg*>(frag_i))
        {
            m_output_ss << "BRNEG " << m_labels[f->target] << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_brzneg*>(frag_i))
        {
            m_output_ss << "BRZNEG " << m_labels[f->target] << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_brpos*>(frag_i))
        {
            m_output_ss << "BRPOS " << m_labels[f->target] << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_brzpos*>(frag_i))
        {
            m_output_ss << "BRZPOS " << m_labels[f->target] << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_brzero*>(frag_i))
        {
            m_output_ss << "BRZERO " << m_labels[f->target] << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_copy*>(frag_i))
        {
            m_output_ss << "COPY " << m_globals[f->dest].first << " " << m_globals[f->src].first << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_add_gvar*>(frag_i))
        {
            m_output_ss << "ADD " << m_globals[f->rhs].first << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_add_imm*>(frag_i))
        {
            m_output_ss << "ADD " << f->rhs << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_sub_gvar*>(frag_i))
        {
            m_output_ss << "SUB " << m_globals[f->rhs].first << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_sub_imm*>(frag_i))
        {
            m_output_ss << "SUB " << f->rhs << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_div_gvar*>(frag_i))
        {
            m_output_ss << "DIV " << m_globals[f->rhs].first << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_div_imm*>(frag_i))
        {
            m_output_ss << "DIV " << f->rhs << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_mult_gvar*>(frag_i))
        {
            m_output_ss << "MULT " << m_globals[f->rhs].first << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_mult_imm*>(frag_i))
        {
            m_output_ss << "MULT " << f->rhs << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_read_gvar*>(frag_i))
        {
            m_output_ss << "READ " << m_globals[f->dest].first << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_write_gvar*>(frag_i))
        {
            m_output_ss << "WRITE " << m_globals[f->src].first << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_write_imm*>(frag_i))
        {
            m_output_ss << "WRITE " << f->src << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_stop*>(frag_i))
        {
            m_output_ss << "STOP\n";
        }
        else if (auto f = dynamic_cast<frag_ins_store_gvar*>(frag_i))
        {
            m_output_ss << "STORE " << m_globals[f->dest].first << "\n";
        }
        else if (auto f = dynamic_cast<frag_ins_load_gvar*>(frag_i))
        {
            m_output_ss << "LOAD " << m_globals[f->src].first << "\n";
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
    }

    m_output = std::move(m_output_ss.str());
    m_output_ss.str("");
    m_output_ss.clear();
}

void p4::codegen::run()
{
    // Traverse the parse tree
    traverse(m_tree);

    // Optimize the code
    optimize();

    // Compose the output
    compose();
}
