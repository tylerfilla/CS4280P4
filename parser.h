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

template<typename InputIteratorT>
class parser
{
    using token_iterator = typename p2::scanner<InputIteratorT>::token_iterator;

    /**
     * A begin iterator over the input tokens.
     */
    token_iterator m_token_begin;

    /**
     * An end iterator over the input tokens.
     */
    token_iterator m_token_end;

    /**
     * An iterator to the current token.
     */
    token_iterator m_token_current;

public:
    parser(token_iterator p_token_begin, token_iterator p_token_end)
            : m_token_begin(p_token_begin)
            , m_token_end(p_token_end)
            , m_token_current(p_token_begin)
    {
    }

    ~parser() = default;

    void parse()
    {
        for (; m_token_current != m_token_end; m_token_current++)
        {
            std::cout << "Got a token: " << *m_token_current << "\n";
        }
    }
};

std::ostream& operator<<(std::ostream& stream, const token& rhs)
{
    stream << rhs.content;
    return stream;
}

} // namespace p2

#endif // #ifndef P2_PARSER_H
