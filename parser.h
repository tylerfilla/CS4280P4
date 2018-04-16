/*
 * Tyler Filla
 * CS 4280
 * Project 3
 */

#ifndef P3_PARSER_H
#define P3_PARSER_H

#include <stdexcept>
#include <string>

#include "scanner.h"
#include "token.h"
#include "tree.h"

namespace p3
{

/**
 * An error thrown by a parser.
 */
class parser_error : std::exception
{
protected:
    parser_error() : std::exception()
    {
    }

public:
    virtual ~parser_error() = default;

    const char* what() const noexcept override
    { return "a parser error has occurred\n"; }

    virtual std::string really_what() const
    { return what(); }
};

/**
 * A parser error due to an unexpected token.
 */
class parser_unexpected_token_error : public parser_error
{
    token m_unexpected_token;

    /**
     * Generate diagnostic text for the given token.
     *
     * @param The token in question
     * @return The diagnostic text
     */
    static std::string diag(token tk)
    {
        switch (tk.type)
        {
        case TK_EOF:
            return "eof";
        case TK_IDENTIFIER:
            return std::string("identifier \"") + tk.content + std::string("\"");
        case TK_INTEGER:
            return std::string("integer ") + tk.content;
        case TK_KW_IFF:
        case TK_KW_ITER:
        case TK_KW_LET:
        case TK_KW_PRINT:
        case TK_KW_PROGRAM:
        case TK_KW_READ:
        case TK_KW_RETURN:
        case TK_KW_START:
        case TK_KW_STOP:
        case TK_KW_THEN:
        case TK_KW_VAR:
        case TK_KW_VOID:
            return std::string("keyword \"") + tk.content + std::string("\"");
        case TK_OP_EQ:
        case TK_OP_LT:
        case TK_OP_GT:
        case TK_OP_COLON:
        case TK_OP_PLUS:
        case TK_OP_MINUS:
        case TK_OP_ASTERISK:
        case TK_OP_SLASH:
        case TK_OP_PERCENT:
        case TK_OP_DOT:
        case TK_OP_OPAREN:
        case TK_OP_CPAREN:
        case TK_OP_COMMA:
        case TK_OP_OBRACE:
        case TK_OP_CBRACE:
        case TK_OP_SEMICOLON:
        case TK_OP_OBRACKET:
        case TK_OP_CBRACKET:
            return std::string("operator ") + tk.content;
        }
    }

public:
    parser_unexpected_token_error(token p_unexpected_token)
            : parser_error(),
            m_unexpected_token(p_unexpected_token)
    {
    }

    ~parser_unexpected_token_error() override = default;

    token get_unexpected_token() const
    { return m_unexpected_token; }

    void set_unexpected_token(token p_unexpected_token)
    { m_unexpected_token = p_unexpected_token; }

    const char* what() const noexcept override
    { return "unexpected token\n"; }

    virtual std::string really_what() const
    { return std::string("unexpected token: ") + diag(m_unexpected_token); }
};

/**
 * A recursive descent parser for the language.
 */
template<typename InputIteratorT>
class parser
{
    using token_iterator = typename p3::scanner<InputIteratorT>::token_iterator;

    /**
     * An iterator to the current location in the input token stream.
     */
    token_iterator m_token_current;

    /**
     * An iterator to the end of the input token stream.
     */
    token_iterator m_token_end;

    /**
     * The completed parse tree.
     */
    tree::node* m_tree;

    /**
     * BNF: <program>
     */
    tree::node_program* parse_program()
    {
        switch (m_token_current->type)
        {
        case TK_KW_PROGRAM:
            {
                ++m_token_current;
                auto node = new tree::node_program {};
                node->nd_vars = parse_vars();
                node->nd_block = parse_block();
                // success
                return node;
            }
        default:
            throw parser_unexpected_token_error(*m_token_current);
        }
    }

    /**
     * BNF: <block>
     */
    tree::node_block* parse_block()
    {
        switch (m_token_current->type)
        {
        case TK_KW_START:
            {
                ++m_token_current;
                auto node = new tree::node_block {};
                node->nd_vars = parse_vars();
                node->nd_stats = parse_stats();
                switch (m_token_current->type)
                {
                case TK_KW_STOP:
                    ++m_token_current;
                    // success
                    return node;
                default:
                    delete node;
                    throw parser_unexpected_token_error(*m_token_current);
                }
            }
        default:
            throw parser_unexpected_token_error(*m_token_current);
        }
    }

    /**
     * BNF: <vars>
     */
    tree::node_vars* parse_vars()
    {
        switch (m_token_current->type)
        {
        case TK_KW_VAR:
            {
                ++m_token_current;
                auto node = new tree::node_vars {};
                switch (m_token_current->type)
                {
                case TK_IDENTIFIER:
                    node->tk_name = *m_token_current;
                    ++m_token_current;
                    switch (m_token_current->type)
                    {
                    case TK_OP_EQ:
                        ++m_token_current;
                        switch (m_token_current->type)
                        {
                        case TK_INTEGER:
                            node->tk_value = *m_token_current;
                            ++m_token_current;
                            node->nd_mvars = parse_mvars();
                            // success
                            return node;
                        default:
                            delete node;
                            throw parser_unexpected_token_error(*m_token_current);
                        }
                    default:
                        delete node;
                        throw parser_unexpected_token_error(*m_token_current);
                    }
                default:
                    delete node;
                    throw parser_unexpected_token_error(*m_token_current);
                }
            }
        default:
            // epsilon
            return nullptr;
        }
    }

    /**
     * BNF: <mvars>
     */
    tree::node_mvars* parse_mvars()
    {
        switch (m_token_current->type)
        {
        case TK_OP_DOT:
            {
                ++m_token_current;
                auto node = new tree::node_mvars_p1 {};
                // success
                return node;
            }
        case TK_OP_COLON:
            {
                ++m_token_current;
                auto node = new tree::node_mvars_p2 {};
                switch (m_token_current->type)
                {
                case TK_IDENTIFIER:
                    node->tk_identifier = *m_token_current;
                    ++m_token_current;
                    node->nd_mvars = parse_mvars();
                    // success
                    return node;
                default:
                    delete node;
                    throw parser_unexpected_token_error(*m_token_current);
                }
            }
        default:
            throw parser_unexpected_token_error(*m_token_current);
        }
    }

    /**
     * BNF: <expr> (left-factorized)
     */
    tree::node_expr* parse_expr()
    {
        auto node = new tree::node_expr {};
        node->nd_M = parse_M();
        node->nd_expr_2 = parse_expr_2();
        // success
        return node;
    }

    /**
     * BNF: <expr_2> (left-factorized, added by me)
     */
    tree::node_expr_2* parse_expr_2()
    {
        switch (m_token_current->type)
        {
        case TK_OP_PLUS:
            {
                ++m_token_current;
                auto node = new tree::node_expr_2_p1 {};
                node->nd_rhs = parse_expr();
                // success
                return node;
            }
        case TK_OP_MINUS:
            {
                ++m_token_current;
                auto node = new tree::node_expr_2_p2 {};
                node->nd_rhs = parse_expr();
                // success
                return node;
            }
        case TK_OP_SLASH:
            {
                ++m_token_current;
                auto node = new tree::node_expr_2_p3 {};
                node->nd_rhs = parse_expr();
                // success
                return node;
            }
        case TK_OP_ASTERISK:
            {
                ++m_token_current;
                auto node_p4 = new tree::node_expr_2_p4 {};
                node_p4->nd_rhs = parse_expr();
                // success
                return node_p4;
            }
        default:
            // epsilon
            return nullptr;
        }
    }

    /**
     * BNF: <M>
     */
    tree::node_M* parse_M()
    {
        switch (m_token_current->type)
        {
        case TK_OP_PERCENT:
            {
                ++m_token_current;
                auto node = new tree::node_M_p1 {};
                node->nd_M = parse_M();
                // success
                return node;
            }
        // FIRST(R)
        case TK_OP_OPAREN:
        case TK_IDENTIFIER:
        case TK_INTEGER:
            {
                // unparsed nonterminal: do not consume token
                auto node = new tree::node_M_p2 {};
                node->nd_R = parse_R();
                // success
                return node;
            }
        default:
            throw parser_unexpected_token_error(*m_token_current);
        }
    }

    /**
     * BNF: <R>
     */
    tree::node_R* parse_R()
    {
        switch (m_token_current->type)
        {
        case TK_OP_OPAREN:
            {
                ++m_token_current;
                auto node = new tree::node_R_p1 {};
                node->nd_expr = parse_expr();
                switch (m_token_current->type)
                {
                case TK_OP_CPAREN:
                    ++m_token_current;
                    // success
                    return node;
                default:
                    delete node;
                    throw parser_unexpected_token_error(*m_token_current);
                }
            }
        case TK_IDENTIFIER:
            {
                auto node = new tree::node_R_p2 {};
                node->tk_identifier = *m_token_current;
                ++m_token_current;
                // success
                return node;
            }
        case TK_INTEGER:
            {
                auto node = new tree::node_R_p3 {};
                node->tk_integer = *m_token_current;
                ++m_token_current;
                // success
                return node;
            }
        default:
            throw parser_unexpected_token_error(*m_token_current);
        }
    }

    /**
     * BNF: <stats>
     */
    tree::node_stats* parse_stats()
    {
        // unparsed nonterminal: do not consume token
        auto node = new tree::node_stats {};
        node->nd_stat = parse_stat();
        node->nd_mStat = parse_mStat();
        // success
        return node;
    }

    /**
     * BNF: <mStat>
     */
    tree::node_mStat* parse_mStat()
    {
        switch (m_token_current->type)
        {
        // FIRST(stat)
        case TK_KW_READ:
        case TK_KW_PRINT:
        case TK_KW_START:
        case TK_KW_IFF:
        case TK_KW_ITER:
        case TK_KW_LET:
            {
                // unparsed nonterminal: do not consume token
                auto node = new tree::node_mStat {};
                node->nd_stat = parse_stat();
                node->nd_mStat = parse_mStat();
                // success
                return node;
            }
        default:
            // epsilon
            return nullptr;
        }
    }

    /**
     * BNF: <stat>
     */
    tree::node_stat* parse_stat()
    {
        switch (m_token_current->type)
        {
        // FIRST(in)
        case TK_KW_READ:
            {
                // unparsed nonterminal: do not consume token
                auto node = new tree::node_stat_p1 {};
                node->nd_in = parse_in();
                // success
                return node;
            }
        // FIRST(out)
        case TK_KW_PRINT:
            {
                // unparsed nonterminal: do not consume token
                auto node = new tree::node_stat_p2 {};
                node->nd_out = parse_out();
                // success
                return node;
            }
        // FIRST(block)
        case TK_KW_START:
            {
                // unparsed nonterminal: do not consume token
                auto node = new tree::node_stat_p3 {};
                node->nd_block = parse_block();
                // success
                return node;
            }
        // FIRST(if)
        case TK_KW_IFF:
            {
                // unparsed nonterminal: do not consume token
                auto node_p4 = new tree::node_stat_p4 {};
                node_p4->nd_if = parse_if();
                // success
                return node_p4;
            }
        // FIRST(loop)
        case TK_KW_ITER:
            {
                // unparsed nonterminal: do not consume token
                auto node_p5 = new tree::node_stat_p5 {};
                node_p5->nd_loop = parse_loop();
                // success
                return node_p5;
            }
        // FIRST(assign)
        case TK_KW_LET:
            {
                // unparsed nonterminal: do not consume token
                auto node_p6 = new tree::node_stat_p6 {};
                node_p6->nd_assign = parse_assign();
                // success
                return node_p6;
            }
        default:
            throw parser_unexpected_token_error(*m_token_current);
        }
    }

    /**
     * BNF: <in>
     */
    tree::node_in* parse_in()
    {
        switch (m_token_current->type)
        {
        case TK_KW_READ:
            ++m_token_current;
            switch (m_token_current->type)
            {
            case TK_IDENTIFIER:
                {
                    auto node = new tree::node_in {};
                    node->tk_identifier = *m_token_current;
                    ++m_token_current;
                    switch (m_token_current->type)
                    {
                    case TK_OP_DOT:
                        ++m_token_current;
                        // success
                        return node;
                    default:
                        delete node;
                        throw parser_unexpected_token_error(*m_token_current);
                    }
                }
            default:
                throw parser_unexpected_token_error(*m_token_current);
            }
        default:
            throw parser_unexpected_token_error(*m_token_current);
        }
    }

    /**
     * BNF: <out>
     */
    tree::node_out* parse_out()
    {
        switch (m_token_current->type)
        {
        case TK_KW_PRINT:
            {
                ++m_token_current;
                auto node = new tree::node_out {};
                node->nd_expr = parse_expr();
                switch (m_token_current->type)
                {
                case TK_OP_DOT:
                    ++m_token_current;
                    // success
                    return node;
                default:
                    delete node;
                    throw parser_unexpected_token_error(*m_token_current);
                }
            }
        default:
            throw parser_unexpected_token_error(*m_token_current);
        }
    }

    /**
     * BNF: <if>
     */
    tree::node_if* parse_if()
    {
        switch (m_token_current->type)
        {
        case TK_KW_IFF:
            ++m_token_current;
            switch (m_token_current->type)
            {
            case TK_OP_OPAREN:
                {
                    ++m_token_current;
                    auto node = new tree::node_if {};
                    node->nd_lhs = parse_expr();
                    node->nd_operator = parse_RO();
                    node->nd_rhs = parse_expr();
                    switch (m_token_current->type)
                    {
                    case TK_OP_CPAREN:
                        ++m_token_current;
                        node->nd_body = parse_stat();
                        // success
                        return node;
                    default:
                        delete node;
                        throw parser_unexpected_token_error(*m_token_current);
                    }
                }
            default:
                throw parser_unexpected_token_error(*m_token_current);
            }
        default:
            throw parser_unexpected_token_error(*m_token_current);
        }
    }

    /**
     * BNF: <loop>
     */
    tree::node_loop* parse_loop()
    {
        switch (m_token_current->type)
        {
        case TK_KW_ITER:
            ++m_token_current;
            switch (m_token_current->type)
            {
            case TK_OP_OPAREN:
                {
                    ++m_token_current;
                    auto node = new tree::node_loop {};
                    node->nd_lhs = parse_expr();
                    node->nd_operator = parse_RO();
                    node->nd_rhs = parse_expr();
                    switch (m_token_current->type)
                    {
                    case TK_OP_CPAREN:
                        ++m_token_current;
                        node->nd_body = parse_stat();
                        // success
                        return node;
                    default:
                        throw parser_unexpected_token_error(*m_token_current);
                    }
                }
            default:
                throw parser_unexpected_token_error(*m_token_current);
            }
        default:
            throw parser_unexpected_token_error(*m_token_current);
        }
    }

    /**
     * BNF: <assign>
     */
    tree::node_assign* parse_assign()
    {
        switch (m_token_current->type)
        {
        case TK_KW_LET:
            ++m_token_current;
            switch (m_token_current->type)
            {
            case TK_IDENTIFIER:
                {
                    auto node = new tree::node_assign {};
                    node->tk_name = *m_token_current;
                    ++m_token_current;
                    switch (m_token_current->type)
                    {
                    case TK_OP_EQ:
                        ++m_token_current;
                        node->nd_value = parse_expr();
                        switch (m_token_current->type)
                        {
                        case TK_OP_DOT:
                            ++m_token_current;
                            // success
                            return node;
                        default:
                            delete node;
                            throw parser_unexpected_token_error(*m_token_current);
                        }
                    default:
                        delete node;
                        throw parser_unexpected_token_error(*m_token_current);
                    }
                }
            default:
                throw parser_unexpected_token_error(*m_token_current);
            }
        default:
            throw parser_unexpected_token_error(*m_token_current);
        }
    }

    /**
     * BNF: <RO> (left-factorized)
     */
    tree::node_RO* parse_RO()
    {
        switch (m_token_current->type)
        {
        case TK_OP_LT:
            {
                ++m_token_current;
                auto node = new tree::node_RO_p1 {};
                node->nd_lt = parse_RO_lt();
                // success
                return node;
            }
        case TK_OP_GT:
            {
                ++m_token_current;
                auto node = new tree::node_RO_p2 {};
                node->nd_gt = parse_RO_gt();
                // success
                return node;
            }
        case TK_OP_EQ:
            {
                ++m_token_current;
                auto node = new tree::node_RO_p3 {};
                node->nd_eq = parse_RO_eq();
                // success
                return node;
            }
        default:
            throw parser_unexpected_token_error(*m_token_current);
        }
    }

    /**
     * BNF: <RO_lt> (left-factorized, added by me)
     */
    tree::node_RO_lt* parse_RO_lt()
    {
        switch (m_token_current->type)
        {
        case TK_OP_LT:
            {
                ++m_token_current;
                auto node = new tree::node_RO_lt {};
                // success
                return node;
            }
        default:
            // epsilon
            return nullptr;
        }
    }

    /**
     * BNF: <RO_gt> (left-factorized, added by me)
     */
    tree::node_RO_gt* parse_RO_gt()
    {
        switch (m_token_current->type)
        {
        case TK_OP_GT:
            {
                ++m_token_current;
                auto node = new tree::node_RO_gt {};
                // success
                return node;
            }
        default:
            // epsilon
            return nullptr;
        }
    }

    /**
     * BNF: <RO_eq> (left-factorized, added by me)
     */
    tree::node_RO_eq* parse_RO_eq()
    {
        switch (m_token_current->type)
        {
        case TK_OP_EQ:
            {
                ++m_token_current;
                auto node = new tree::node_RO_eq {};
                // success
                return node;
            }
        default:
            // epsilon
            return nullptr;
        }
    }

public:
    parser(token_iterator p_token_current, token_iterator p_token_end)
            : m_token_current(p_token_current)
            , m_token_end(p_token_end)
            , m_tree {}
    {
    }

    parser(const parser& rhs) = delete;

    ~parser()
    { delete m_tree; }

    parser& operator=(const parser& rhs) = delete;

    /**
     * @return The completed parse tree
     */
    tree::node* get_tree() const
    { return m_tree; }

    /**
     * Parse from the configured source token stream.
     */
    void parse()
    {
        auto node = parse_program();
        switch (m_token_current->type)
        {
        case TK_EOF:
            ++m_token_current;
            // success
            m_tree = node;
            return;
        default:
            delete node;
            throw parser_unexpected_token_error(*m_token_current);
        }
    }
};

} // namespace p3

#endif // #ifndef P3_PARSER_H
