/*
 * Tyler Filla
 * CS 4280
 * Project 2
 */

#ifndef P2_PARSER_H
#define P2_PARSER_H

#include <iostream> // FIXME: Remember to remove this
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
            std::cout << "parsed program\n";
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
                std::cout << "parsed block\n";
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
            std::cout << "epsilon on vars\n";
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
            std::cout << "parsed mvars dot\n";
            return;
        case TK_OP_COLON:
            ++m_token_current;
            switch (m_token_current->type)
            {
            case TK_IDENTIFIER:
                ++m_token_current;
                parse_mvars();
                // success
                std::cout << "parsed mvars colon\n";
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
        std::cout << "parsed expr\n";
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
            std::cout << "parsed expr_2 plus\n";
            return;
        case TK_OP_MINUS:
            ++m_token_current;
            parse_expr();
            // success
            std::cout << "parsed expr_2 minus\n";
            return;
        case TK_OP_SLASH:
            ++m_token_current;
            parse_expr();
            // success
            std::cout << "parsed expr_2 slash\n";
            return;
        case TK_OP_ASTERISK:
            ++m_token_current;
            parse_expr();
            // success
            std::cout << "parsed expr_2 asterisk\n";
            return;
        default:
            // epsilon
            std::cout << "epsilon on expr_2\n";
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
            std::cout << "parsed M percent\n";
            return;
        // FIRST(R)
        case TK_OP_OPAREN:
        case TK_IDENTIFIER:
        case TK_INTEGER:
            // unparsed nonterminal: do not consume token
            parse_R();
            // success
            std::cout << "parsed M R\n";
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
                std::cout << "parsed R oparen\n";
                return;
            default:
                throw parser_unexpected_token_error(*m_token_current);
            }
        case TK_IDENTIFIER:
            ++m_token_current;
            // success
            std::cout << "parsed R identifier\n";
            return;
        case TK_INTEGER:
            ++m_token_current;
            // success
            std::cout << "parsed R integer\n";
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
        std::cout << "parsed stats\n";
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
        case TK_KW_IFF:
        case TK_KW_ITER:
        case TK_KW_LET:
            // unparsed nonterminal: do not consume token
            parse_stat();
            parse_mStat();
            // success
            std::cout << "parsed mStat stat\n";
            return;
        default:
            // epsilon
            std::cout << "epsilon on mStat\n";
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
            std::cout << "parsed stat in\n";
            return;
        // FIRST(out)
        case TK_KW_PRINT:
            // unparsed nonterminal: do not consume token
            parse_out();
            // success
            std::cout << "parsed stat out\n";
            return;
        // FIRST(block)
        case TK_KW_START:
            // unparsed nonterminal: do not consume token
            parse_block();
            // success
            std::cout << "parsed stat block\n";
            return;
        // FIRST(if)
        case TK_KW_IFF:
            // unparsed nonterminal: do not consume token
            parse_if();
            // success
            std::cout << "parsed stat if\n";
            return;
        // FIRST(loop)
        case TK_KW_ITER:
            // unparsed nonterminal: do not consume token
            parse_loop();
            // success
            std::cout << "parsed stat loop\n";
            return;
        // FIRST(assign)
        case TK_KW_LET:
            // unparsed nonterminal: do not consume token
            parse_assign();
            // success
            std::cout << "parsed stat assign\n";
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
                    std::cout << "parsed in\n";
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
                std::cout << "parsed out\n";
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
                    std::cout << "parsed if\n";
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
                    std::cout << "parsed loop\n";
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
                        std::cout << "parsed assign\n";
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
            std::cout << "parsed RO lt\n";
            return;
        case TK_OP_GT:
            ++m_token_current;
            parse_RO_gt();
            // success
            std::cout << "parsed RO gt\n";
            return;
        case TK_OP_EQ:
            ++m_token_current;
            parse_RO_eq();
            // success
            std::cout << "parsed RO eq\n";
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
            std::cout << "parsed RO_lt lt\n";
            return;
        default:
            // epsilon
            std::cout << "epsilon on RO_lt\n";
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
            std::cout << "parsed RO_gt gt\n";
            return;
        default:
            // epsilon
            std::cout << "epsilon on RO_gt\n";
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
            std::cout << "parsed RO_eq eq\n";
            return;
        default:
            // epsilon
            std::cout << "epsilon on RO_eq\n";
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

        std::cout << "successfully parsed :)\n";

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
