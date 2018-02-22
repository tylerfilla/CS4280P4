/*
 * Tyler Filla
 * CS 4280
 * Project 1
 */

#ifndef P1_SCANNER_H
#define P1_SCANNER_H

#include "token.h"

namespace p1
{

/**
 * The scanner object.
 */
template<typename InputIteratorT>
class scanner
{
    /**
     * An iterator to the beginning of the input.
     */
    InputIteratorT m_iter_begin;

    /**
     * An iterator to the end of the input.
     */
    InputIteratorT m_iter_end;

    /**
     * An iterator to the current position in the input.
     */
    InputIteratorT m_iter_current;

public:
    scanner(InputIteratorT p_iter_begin, InputIteratorT p_iter_end)
            : m_iter_begin(p_iter_begin)
            , m_iter_end(p_iter_end)
            , m_iter_current(p_iter_begin)
    {
    }

    /**
     * Scan the next token from the input.
     * @return The next token
     */
    token next_token()
    {
        for (auto i = m_iter_current; i != m_iter_end; ++i)
        {
            // FIXME
        }

        // FIXME
        return {};
    }
};

} // namespace p1

#endif // #ifndef P1_SCANNER_H
