/*
 * Tyler Filla
 * CS 4280
 * Project 1
 */

#ifndef P1_SCANNER_H
#define P1_SCANNER_H

#include <iostream>
#include <string>

#include "token.h"
#include "scanner_table.gen.h"

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

    /**
     * The current state of the scanner.
     */
    int m_state;

    /**
     * The in-progress content of the current token.
     */
    std::string m_running_content;

    /**
     * The line on which the current token begins.
     */
    unsigned int m_token_line;

    /**
     * The column at which the current token begins.
     */
    unsigned int m_token_column;

    /**
     * The current line.
     */
    unsigned int m_current_line;

    /**
     * The current column.
     */
    unsigned int m_current_column;

    /**
     * Translate an input character from ASCII to a local, more compact alphabet. This decouples the state machine from
     * ASCII, which reduces the size of the transition table, makes it easier to expand the alphabet in the future, and
     * provides a quick, initial validation of the input characters before they hit the state machine.
     *
     * @param ch The input character
     * @return The output character
     */
    int intake(char ch)
    {
        // Group: lowercase letters
        if (ch >= 'a' && ch <= 'z')
            return ch - 'a';

        // Group: uppercase letters
        if (ch >= 'A' && ch <= 'Z')
            return ch - 'A' + 26;

        // Group: decimal digits
        if (ch >= '0' && ch <= '9')
            return ch - '0' + 26 + 26;

        // Miscellaneous
        int base = 26 + 26 + 10;
        switch (ch)
        {
        case '=':
            return base;
        case '<':
            return base + 1;
        case '>':
            return base + 2;
        case ':':
            return base + 3;
        case '+':
            return base + 4;
        case '-':
            return base + 5;
        case '*':
            return base + 6;
        case '/':
            return base + 7;
        case '%':
            return base + 8;
        case '.':
            return base + 9;
        case '(':
            return base + 10;
        case ')':
            return base + 11;
        case ',':
            return base + 12;
        case '{':
            return base + 13;
        case '}':
            return base + 14;
        case ';':
            return base + 15;
        case '[':
            return base + 16;
        case ']':
            return base + 17;
        case '&':
            return base + 18;
        case ' ':
            // Space
            return base + 19;
        case '\t':
            // Horizontal tab
            return base + 20;
        case '\r':
            // Carriage return (CR)
            return base + 21;
        case '\n':
            // Linefeed (LF)
            return base + 22;
        default:
            // Illegal character (not in input alphabet)
            return -1;
        }
    }

    token accept_token(int action)
    {
        token tk {};

        tk.type = static_cast<token_type>(action & ~SCANNER_TABLE_ACCEPT_MASK);
        tk.content = m_running_content.substr(0, m_running_content.length() - 1);
        tk.line_begin = m_token_line;
        tk.line_end = m_current_line;
        tk.column_begin = m_token_column;
        tk.column_end = (m_current_column - 1 < 1) ? 1 : m_current_column - 1;

        m_state = 0;
        m_running_content.clear();
        m_token_line = m_current_line;
        m_token_column = m_current_column;

        return tk;
    }

public:
    scanner(InputIteratorT p_iter_begin, InputIteratorT p_iter_end)
            : m_iter_begin(p_iter_begin)
            , m_iter_end(p_iter_end)
            , m_iter_current(p_iter_begin)
            , m_state(0)
            , m_running_content {}
            , m_token_line(1)
            , m_token_column(1)
            , m_current_line(1)
            , m_current_column(1)
    {
    }

    scanner(const scanner& rhs) = delete;

    ~scanner() = default;

    scanner& operator=(const scanner& rhs) = delete;

    /**
     * Scan the next token from the input.
     * @return The next token
     */
    token next_token()
    {
        m_running_content.clear();
        m_token_line = m_current_line;
        m_token_column = m_current_column;

        // Continue from current position
        for (; m_iter_current != m_iter_end; ++m_iter_current)
        {
            // Get input character
            char c = *m_iter_current;

            if (c == '\n')
            {
                m_current_line++;
                m_current_column = 1;
            }
            else
            {
                m_current_column++;
            }

            // Add character to running content
            m_running_content += c;

            // Translate character to input alphabet
            // This is only useful for internal processing, not diagnostic messages
            int c_in = intake(c);

            // If character is illegal
            if (c_in == -1)
            {
                // TODO: Add real diagnostic reporting with "Scanner Error: " prefix as instructed
                std::cerr << "illegal character: " << c << "\n";
                return {};
            }

            // Look up the action for this character in the current state
            int action = SCANNER_TABLE[m_state][c_in];

            // If character is unexpected
            if (action == -1)
            {
                // TODO: Diagnostic message
                std::cerr << "unexpected character: " << c << "\n";
                return {};
            }

            if ((action & SCANNER_TABLE_ACCEPT_MASK) != 0)
            {
                auto tk = accept_token(action);

                if (c == '\n')
                {
                    m_current_line--;
                }
                else
                {
                    m_current_column--;
                }

                return tk;
            }
            else
            {
                // Go to the new state
                m_state = action;

                // Clear running content when dropping to ground state
                if (m_state == 0)
                {
                    m_running_content.clear();
                    m_token_line = m_current_line;
                    m_token_column = m_current_column;
                }
            }
        }

        // If we're still not in the ground state after trying to read a token
        // We must have encountered EOF at this point
        if (m_state > 0)
        {
            // Get the action for a virtual trailing newline
            // This is a hack to squeeze out the last token in a file
            int action = SCANNER_TABLE[m_state][intake('\n')];

            if (action == -1)
            {
                // TODO: Real diagnostics
                std::cerr << "internal scanner error: cannot close state with virtual linefeed\n";
                return {};
            }

            if ((action & SCANNER_TABLE_ACCEPT_MASK) != 0)
            {
                m_running_content += *m_iter_current;
                m_current_column++;
                return accept_token(action);
            }
            else
            {
                // TODO: Diag
                std::cerr << "premature end-of-file\n";
                return {};
            }
        }

        return {};
    }
};

} // namespace p1

#endif // #ifndef P1_SCANNER_H
