/*
 * Tyler Filla
 * CS 4280
 * Project 1
 */

#ifndef P1_SCANNER_TESTER_H
#define P1_SCANNER_TESTER_H

#include <iostream>
#include "token.h"

namespace p1
{

template<typename InputIteratorT>
class scanner;

/**
 * A test harness for the scanner object.
 */
template<typename InputIteratorT>
class scanner_tester
{
    /**
     * A reference to the tested scanner.
     */
    scanner<InputIteratorT>& m_scanner;

public:
    scanner_tester(scanner<InputIteratorT>& p_scanner)
            : m_scanner(p_scanner)
    {
    }

    scanner_tester(const scanner_tester& rhs) = delete;

    ~scanner_tester() = default;

    scanner_tester& operator=(const scanner_tester& rhs) = delete;

    /**
     * Test the scanner.
     */
    void test_scanner()
    {
        while (true)
        {
            // Get the next token
            auto tk = m_scanner.next_token();

            // FIXME: Actually write gud code herr
            std::cout << tk.type << "\n";

            // Stop at EOF
            if (tk.type == TK_EOF)
                break;
        }
    }
};

} // namespace p1

#endif // #ifndef P1_SCANNER_TESTER_H
