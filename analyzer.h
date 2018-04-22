/*
 * Tyler Filla
 * CS 4280
 * Project 3
 */

#ifndef P3_ANALYZER_H
#define P3_ANALYZER_H

#include <string>
#include <vector>

#include "token.h"
#include "tree.h"

namespace p3
{

/**
 * An error thrown by an analyzer.
 */
class analyzer_error : public std::exception
{
protected:
    analyzer_error() : std::exception()
    {
    }

public:
    virtual ~analyzer_error() = default;

    const char* what() const noexcept override
    { return "a semantics error has occurred"; }

    virtual std::string really_what() const
    { return what(); }
};

/**
 * An error thrown by an analyzer when it encounters the use of an undefined
 * or out-of-scope variable.
 */
class analyzer_undefined_variable_error : public analyzer_error
{
    std::string m_name;

public:
    analyzer_undefined_variable_error(std::string p_name)
            : analyzer_error()
            , m_name(std::move(p_name))
    {
    }

    const std::string& get_name() const
    { return m_name; }

    const char* what() const noexcept final
    { return "undefined variable"; }

    std::string really_what() const
    { return std::string("undefined variable: ") + m_name; }
};

/**
 * An error thrown by an analyzer when it encounters the redefinition of an in-
 * scope variable.
 */
class analyzer_redefined_variable_error : public analyzer_error
{
    std::string m_name;
    unsigned int m_defined_line;
    unsigned int m_defined_column;

public:
    analyzer_redefined_variable_error(std::string p_name, unsigned int p_defined_line, unsigned int p_defined_column)
            : analyzer_error()
            , m_name(std::move(p_name))
            , m_defined_line(0)
            , m_defined_column(0)
    {
    }

    const std::string& get_name() const
    { return m_name; }

    unsigned int get_defined_line() const
    { return m_defined_line; }

    unsigned int get_defined_column() const
    { return m_defined_column; }

    const char* what() const noexcept final
    { return "redefined variable"; }

    std::string really_what() const
    { return std::string("redefined variable: ") + m_name; }
};

/**
 * A semantic analyzer for the language.
 */
class analyzer
{
    /**
     * A variable in a program.
     */
    struct variable
    {
        /** The variable name. */
        std::string name;

        /** The line on which the variable was defined. */
        unsigned int defined_line;

        /** The column on which the variable was defined. */
        unsigned int defined_column;
    };

    /**
     * A scope in a program.
     */
    struct scope
    {
        /** The variables defined in this scope. */
        std::vector<variable> variables;
    };

    /** The parse tree. */
    tree::node* m_tree;

    /** The encountered scopes. */
    std::vector<scope> m_scopes;

public:
    analyzer(tree::node* p_tree)
            : m_tree(p_tree)
            , m_scopes {}
    {
    }

    analyzer(const analyzer& rhs) = delete;

    ~analyzer() = default;

    analyzer& operator=(const analyzer& rhs) = delete;

private:
    /**
     * Enter a new scope.
     */
    void enter_scope()
    {
        // Push a new scope
        m_scopes.emplace_back();
    }

    /**
     * Leave the current scope.
     */
    void leave_scope()
    {
        // Pop the last scope
        m_scopes.erase(m_scopes.end() - 1);
    }

    /**
     * Find a variable by name.
     *
     * @param name The variable name
     * @return 0 if out-of-scope, 1 if in-scope, or 2 if in-block
     */
    int find_variable(const std::string& name)
    {
        for (auto s = m_scopes.crbegin(); s != m_scopes.crend(); ++s)
        {
            for (auto&& v : s->variables)
            {
                if (v.name == name)
                {
                    // Test if in-block
                    if (s == m_scopes.crbegin())
                        return 2;

                    // Not in-block, but must be in-scope, at least
                    return 1;
                }
            }
        }

        // Variable is out-of-scope
        return 0;
    }

    /**
     * Add a variable to the current scope.
     *
     * @param name The variable name
     * @param line The definition line
     * @param column The definition column
     */
    void add_variable(std::string name, unsigned int line, unsigned int column)
    {
        // Do not allow redefinition within blocks
        if (find_variable(name) == 2)
            throw analyzer_redefined_variable_error(name, line, column);

        // Create variable descriptor
        variable v;
        v.name = std::move(name);
        v.defined_line = line;
        v.defined_column = column;

        // Add variable to current scope
        m_scopes.back().variables.push_back(v);
    }

    /**
     * Analyze a single node.
     * @param node The node
     */
    void analyze(tree::node* p_node)
    {
        if (!p_node)
            return;

        if (auto node = dynamic_cast<tree::node_program*>(p_node))
        {
            analyze(node->nd_vars);
            analyze(node->nd_block);
        }
        else if (auto node = dynamic_cast<tree::node_block*>(p_node))
        {
            enter_scope();
            analyze(node->nd_vars);
            analyze(node->nd_stats);
            leave_scope();
        }
        else if (auto node = dynamic_cast<tree::node_vars*>(p_node))
        {
            auto& tk = node->tk_name;
            add_variable(tk.content, tk.line_begin, tk.column_begin);
            analyze(node->nd_mvars);
        }
        else if (auto node = dynamic_cast<tree::node_mvars_p1*>(p_node))
        {
        }
        else if (auto node = dynamic_cast<tree::node_mvars_p2*>(p_node))
        {
            auto& tk = node->tk_identifier;
            add_variable(tk.content, tk.line_begin, tk.column_begin);
            analyze(node->nd_mvars);
        }
        else if (auto node = dynamic_cast<tree::node_expr*>(p_node))
        {
            analyze(node->nd_M);
            analyze(node->nd_expr_2);
        }
        else if (auto node = dynamic_cast<tree::node_expr_2_p1*>(p_node))
        {
            analyze(node->nd_rhs);
        }
        else if (auto node = dynamic_cast<tree::node_expr_2_p2*>(p_node))
        {
            analyze(node->nd_rhs);
        }
        else if (auto node = dynamic_cast<tree::node_expr_2_p3*>(p_node))
        {
            analyze(node->nd_rhs);
        }
        else if (auto node = dynamic_cast<tree::node_expr_2_p4*>(p_node))
        {
            analyze(node->nd_rhs);
        }
        else if (auto node = dynamic_cast<tree::node_M_p1*>(p_node))
        {
            analyze(node->nd_M);
        }
        else if (auto node = dynamic_cast<tree::node_M_p2*>(p_node))
        {
            analyze(node->nd_R);
        }
        else if (auto node = dynamic_cast<tree::node_R_p1*>(p_node))
        {
            analyze(node->nd_expr);
        }
        else if (auto node = dynamic_cast<tree::node_R_p2*>(p_node))
        {
            const auto& name = node->tk_identifier.content;

            if (!find_variable(name))
                throw analyzer_undefined_variable_error(name);
        }
        else if (auto node = dynamic_cast<tree::node_R_p3*>(p_node))
        {
        }
        else if (auto node = dynamic_cast<tree::node_stats*>(p_node))
        {
            analyze(node->nd_stat);
            analyze(node->nd_mStat);
        }
        else if (auto node = dynamic_cast<tree::node_mStat*>(p_node))
        {
            analyze(node->nd_stat);
            analyze(node->nd_mStat);
        }
        else if (auto node = dynamic_cast<tree::node_stat_p1*>(p_node))
        {
            analyze(node->nd_in);
        }
        else if (auto node = dynamic_cast<tree::node_stat_p2*>(p_node))
        {
            analyze(node->nd_out);
        }
        else if (auto node = dynamic_cast<tree::node_stat_p3*>(p_node))
        {
            analyze(node->nd_block);
        }
        else if (auto node = dynamic_cast<tree::node_stat_p4*>(p_node))
        {
            analyze(node->nd_if);
        }
        else if (auto node = dynamic_cast<tree::node_stat_p5*>(p_node))
        {
            analyze(node->nd_loop);
        }
        else if (auto node = dynamic_cast<tree::node_stat_p6*>(p_node))
        {
            analyze(node->nd_assign);
        }
        else if (auto node = dynamic_cast<tree::node_in*>(p_node))
        {
        }
        else if (auto node = dynamic_cast<tree::node_out*>(p_node))
        {
            analyze(node->nd_expr);
        }
        else if (auto node = dynamic_cast<tree::node_if*>(p_node))
        {
            analyze(node->nd_lhs);
            analyze(node->nd_operator);
            analyze(node->nd_rhs);
            analyze(node->nd_body);
        }
        else if (auto node = dynamic_cast<tree::node_loop*>(p_node))
        {
            analyze(node->nd_lhs);
            analyze(node->nd_operator);
            analyze(node->nd_rhs);
            analyze(node->nd_body);
        }
        else if (auto node = dynamic_cast<tree::node_assign*>(p_node))
        {
            analyze(node->nd_value);
        }
        else if (auto node = dynamic_cast<tree::node_RO_p1*>(p_node))
        {
            analyze(node->nd_lt);
        }
        else if (auto node = dynamic_cast<tree::node_RO_p2*>(p_node))
        {
            analyze(node->nd_gt);
        }
        else if (auto node = dynamic_cast<tree::node_RO_p3*>(p_node))
        {
            analyze(node->nd_eq);
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
    }

public:
    /**
     * Run semantic analysis on the given parse tree.
     */
    void run()
    {
        enter_scope();
        analyze(m_tree);
        leave_scope();
    }
};

} // namespace p3

#endif // #ifndef P3_ANALYZER_H
