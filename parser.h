/*
 * Tyler Filla
 * CS 4280
 * Project 2
 */

#ifndef P2_PARSER_H
#define P2_PARSER_H

#include "scanner.h"

namespace p2
{

/**
 * An error thrown by a parser.
 */
class parser_error : std::exception
{
public:
    virtual ~parser_error() = default;
};

/**
 * A parser error due to an unexpected token.
 */
class parser_unexpected_token_error : public parser_error
{
    token m_unexpected_token;

public:
    parser_unexpected_token_error(token p_unexpected_token) : m_unexpected_token(p_unexpected_token)
    {
    }

    ~parser_unexpected_token_error() override = default;

    token get_unexpected_token() const
    { return m_unexpected_token; }

    void set_unexpected_token(token p_unexpected_token)
    { m_unexpected_token = p_unexpected_token; }
};

/**
 * A recursive descent parser for the language.
 */
template<typename InputIteratorT>
class parser
{
    using token_iterator = typename p2::scanner<InputIteratorT>::token_iterator;

    /**
     * An iterator to the current location in the input token stream.
     */
    token_iterator m_token_current;

    /**
     * An iterator to the end of the input token stream.
     */
    token_iterator m_token_end;

    /**
     * BNF: <program>
     */
    void parse_program()
    {
        switch (m_token_current->type)
        {
        case TK_KW_PROGRAM:
            ++m_token_current;
            parse_vars();
            parse_block();
            // success
            return;
        default:
            throw parser_unexpected_token_error(*m_token_current);
        }
    }

    /**
     * BNF: <block>
     */
    void parse_block()
    {
        switch (m_token_current->type)
        {
        case TK_KW_START:
            ++m_token_current;
            parse_vars();
            parse_stats();
            switch (m_token_current->type)
            {
            case TK_KW_STOP:
                ++m_token_current;
                // success
                return;
            default:
                throw parser_unexpected_token_error(*m_token_current);
            }
        default:
            throw parser_unexpected_token_error(*m_token_current);
        }
    }

    /**
     * BNF: <vars>
     */
    void parse_vars()
    {
        switch (m_token_current->type)
        {
        case TK_KW_VAR:
            ++m_token_current;
            switch (m_token_current->type)
            {
            case TK_IDENTIFIER:
                ++m_token_current;
                switch (m_token_current->type)
                {
                case TK_OP_EQ:
                    ++m_token_current;
                    switch (m_token_current->type)
                    {
                    case TK_INTEGER:
                        ++m_token_current;
                        parse_mvars();
                        // success
                        return;
                    default:
                        throw parser_unexpected_token_error(*m_token_current);
                    }
                default:
                    throw parser_unexpected_token_error(*m_token_current);
                }
            default:
                throw parser_unexpected_token_error(*m_token_current);
            }
        default:
            // epsilon
            return;
        }
    }

    /**
     * BNF: <mvars>
     */
    void parse_mvars()
    {
        switch (m_token_current->type)
        {
        case TK_OP_DOT:
            ++m_token_current;
            // success
            return;
        case TK_OP_COLON:
            ++m_token_current;
            switch (m_token_current->type)
            {
            case TK_IDENTIFIER:
                ++m_token_current;
                parse_mvars();
                // success
                return;
            default:
                throw parser_unexpected_token_error(*m_token_current);
            }
        default:
            throw parser_unexpected_token_error(*m_token_current);
        }
    }

    /**
     * BNF: <expr> (left-factorized)
     */
    void parse_expr()
    {
        parse_M();
        parse_expr_2();
        // success
        return;
    }

    /**
     * BNF: <expr_2> (left-factorized, added by me)
     */
    void parse_expr_2()
    {
        switch (m_token_current->type)
        {
        case TK_OP_PLUS:
            ++m_token_current;
            parse_expr();
            // success
            return;
        case TK_OP_MINUS:
            ++m_token_current;
            parse_expr();
            // success
            return;
        case TK_OP_SLASH:
            ++m_token_current;
            parse_expr();
            // success
            return;
        case TK_OP_ASTERISK:
            ++m_token_current;
            parse_expr();
            // success
            return;
        default:
            // epsilon
            return;
        }
    }

    /**
     * BNF: <M>
     */
    void parse_M()
    {
        switch (m_token_current->type)
        {
        case TK_OP_PERCENT:
            ++m_token_current;
            parse_M();
            // success
            return;
        // FIRST(R)
        case TK_OP_OPAREN:
        case TK_IDENTIFIER:
        case TK_INTEGER:
            // unparsed nonterminal: do not consume token
            parse_R();
            // success
            return;
        default:
            throw parser_unexpected_token_error(*m_token_current);
        }
    }

    /**
     * BNF: <R>
     */
    void parse_R()
    {
        switch (m_token_current->type)
        {
        case TK_OP_OPAREN:
            ++m_token_current;
            parse_expr();
            switch (m_token_current->type)
            {
            case TK_OP_CPAREN:
                ++m_token_current;
                // success
                return;
            default:
                throw parser_unexpected_token_error(*m_token_current);
            }
        case TK_IDENTIFIER:
            ++m_token_current;
            // success
            return;
        case TK_INTEGER:
            ++m_token_current;
            // success
            return;
        default:
            throw parser_unexpected_token_error(*m_token_current);
        }
    }

    /**
     * BNF: <stats>
     */
    void parse_stats()
    {
        // unparsed nonterminal: do not consume token
        parse_stat();
        parse_mStat();
        // success
        return;
    }

    /**
     * BNF: <mStat>
     */
    void parse_mStat()
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
            // unparsed nonterminal: do not consume token
            parse_stat();
            parse_mStat();
            // success
            return;
        default:
            // epsilon
            return;
        }
    }

    /**
     * BNF: <stat>
     */
    void parse_stat()
    {
        switch (m_token_current->type)
        {
        // FIRST(in)
        case TK_KW_READ:
            // unparsed nonterminal: do not consume token
            parse_in();
            // success
            return;
        // FIRST(out)
        case TK_KW_PRINT:
            // unparsed nonterminal: do not consume token
            parse_out();
            // success
            return;
        // FIRST(block)
        case TK_KW_START:
            // unparsed nonterminal: do not consume token
            parse_block();
            // success
            return;
        // FIRST(if)
        case TK_KW_IFF:
            // unparsed nonterminal: do not consume token
            parse_if();
            // success
            return;
        // FIRST(loop)
        case TK_KW_ITER:
            // unparsed nonterminal: do not consume token
            parse_loop();
            // success
            return;
        // FIRST(assign)
        case TK_KW_LET:
            // unparsed nonterminal: do not consume token
            parse_assign();
            // success
            return;
        default:
            throw parser_unexpected_token_error(*m_token_current);
        }
    }

    /**
     * BNF: <in>
     */
    void parse_in()
    {
        switch (m_token_current->type)
        {
        case TK_KW_READ:
            ++m_token_current;
            switch (m_token_current->type)
            {
            case TK_IDENTIFIER:
                ++m_token_current;
                switch (m_token_current->type)
                {
                case TK_OP_DOT:
                    ++m_token_current;
                    // success
                    return;
                default:
                    throw parser_unexpected_token_error(*m_token_current);
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
    void parse_out()
    {
        switch (m_token_current->type)
        {
        case TK_KW_PRINT:
            ++m_token_current;
            parse_expr();
            switch (m_token_current->type)
            {
            case TK_OP_DOT:
                ++m_token_current;
                // success
                return;
            default:
                throw parser_unexpected_token_error(*m_token_current);
            }
        default:
            throw parser_unexpected_token_error(*m_token_current);
        }
    }

    /**
     * BNF: <if>
     */
    void parse_if()
    {
        switch (m_token_current->type)
        {
        case TK_KW_IFF:
            ++m_token_current;
            switch (m_token_current->type)
            {
            case TK_OP_OPAREN:
                ++m_token_current;
                parse_expr();
                parse_RO();
                parse_expr();
                switch (m_token_current->type)
                {
                case TK_OP_CPAREN:
                    ++m_token_current;
                    parse_stat();
                    // success
                    return;
                default:
                    throw parser_unexpected_token_error(*m_token_current);
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
    void parse_loop()
    {
        switch (m_token_current->type)
        {
        case TK_KW_ITER:
            ++m_token_current;
            switch (m_token_current->type)
            {
            case TK_OP_OPAREN:
                ++m_token_current;
                parse_expr();
                parse_RO();
                parse_expr();
                switch (m_token_current->type)
                {
                case TK_OP_CPAREN:
                    ++m_token_current;
                    parse_stat();
                    // success
                    return;
                default:
                    throw parser_unexpected_token_error(*m_token_current);
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
    void parse_assign()
    {
        switch (m_token_current->type)
        {
        case TK_KW_LET:
            ++m_token_current;
            switch (m_token_current->type)
            {
            case TK_IDENTIFIER:
                ++m_token_current;
                switch (m_token_current->type)
                {
                case TK_OP_EQ:
                    ++m_token_current;
                    parse_expr();
                    switch (m_token_current->type)
                    {
                    case TK_OP_DOT:
                        ++m_token_current;
                        // success
                        return;
                    default:
                        throw parser_unexpected_token_error(*m_token_current);
                    }
                default:
                    throw parser_unexpected_token_error(*m_token_current);
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
    void parse_RO()
    {
        switch (m_token_current->type)
        {
        case TK_OP_LT:
            ++m_token_current;
            parse_RO_lt();
            // success
            return;
        case TK_OP_GT:
            ++m_token_current;
            parse_RO_gt();
            // success
            return;
        case TK_OP_EQ:
            ++m_token_current;
            parse_RO_eq();
            // success
            return;
        default:
            throw parser_unexpected_token_error(*m_token_current);
        }
    }

    /**
     * BNF: <RO_lt> (left-factorized, added by me)
     */
    void parse_RO_lt()
    {
        switch (m_token_current->type)
        {
        case TK_OP_LT:
            ++m_token_current;
            // success
            return;
        default:
            // epsilon
            return;
        }
    }

    /**
     * BNF: <RO_gt> (left-factorized, added by me)
     */
    void parse_RO_gt()
    {
        switch (m_token_current->type)
        {
        case TK_OP_GT:
            ++m_token_current;
            // success
            return;
        default:
            // epsilon
            return;
        }
    }

    /**
     * BNF: <RO_eq> (left-factorized, added by me)
     */
    void parse_RO_eq()
    {
        switch (m_token_current->type)
        {
        case TK_OP_EQ:
            ++m_token_current;
            // success
            return;
        default:
            // epsilon
            return;
        }
    }

public:
    parser(token_iterator p_token_current, token_iterator p_token_end)
            : m_token_current(p_token_current)
            , m_token_end(p_token_end)
    {
    }

    ~parser() = default;

    int parse() // FIXME: Return tree
    {
        parse_program();
        ++m_token_current;

        if (m_token_current->type != TK_EOF)
            throw parser_unexpected_token_error(*m_token_current);

        return 0;
    }
};

std::ostream& operator<<(std::ostream& stream, const token& rhs)
{
    stream << rhs.content;
    return stream;
}

} // namespace p2

#endif // #ifndef P2_PARSER_H
