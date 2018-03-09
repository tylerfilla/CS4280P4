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
     * An iterator to the current location in the input token stream.
     */
    token_iterator m_token_current;

    /**
     * An iterator to the end of the input token stream.
     */
    token_iterator m_token_end;

public:
    parser(token_iterator p_token_current, token_iterator p_token_end)
            : m_token_current(p_token_current)
            , m_token_end(p_token_end)
    {
    }

    ~parser() = default;

    int parse() // FIXME: Return tree
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
