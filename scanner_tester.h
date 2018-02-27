/*
 * Tyler Filla
 * CS 4280
 * Project 1
 */

#ifndef P1_SCANNER_TESTER_H
#define P1_SCANNER_TESTER_H

#include <iostream>
#include <string>

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

    std::string token_name(token_type tk)
    {
        switch (tk)
        {
        case TK_EOF:
            return "EOF";
        case TK_IDENTIFIER:
            return "IDENTIFIER";
        case TK_INTEGER:
            return "INTEGER";
        case TK_KW_IFF:
            return "KW_IFF";
        case TK_KW_ITER:
            return "KW_ITER";
        case TK_KW_LET:
            return "KW_LET";
        case TK_KW_PRINT:
            return "KW_PRINT";
        case TK_KW_PROGRAM:
            return "KW_PROGRAM";
        case TK_KW_READ:
            return "KW_READ";
        case TK_KW_RETURN:
            return "KW_RETURN";
        case TK_KW_START:
            return "KW_START";
        case TK_KW_STOP:
            return "KW_STOP";
        case TK_KW_THEN:
            return "KW_THEN";
        case TK_KW_VAR:
            return "KW_VAR";
        case TK_KW_VOID:
            return "KW_VOID";
        case TK_OP_EQ:
            return "OP_EQ";
        case TK_OP_LT:
            return "OP_LT";
        case TK_OP_GT:
            return "OP_GT";
        case TK_OP_COLON:
            return "OP_COLON";
        case TK_OP_PLUS:
            return "OP_PLUS";
        case TK_OP_MINUS:
            return "OP_MINUS";
        case TK_OP_ASTERISK:
            return "OP_ASTERISK";
        case TK_OP_SLASH:
            return "OP_SLASH";
        case TK_OP_PERCENT:
            return "OP_PERCENT";
        case TK_OP_DOT:
            return "OP_DOT";
        case TK_OP_OPAREN:
            return "OP_OPAREN";
        case TK_OP_CPAREN:
            return "OP_CPAREN";
        case TK_OP_COMMA:
            return "OP_COMMA";
        case TK_OP_OBRACE:
            return "OP_OBRACE";
        case TK_OP_CBRACE:
            return "OP_CBRACE";
        case TK_OP_SEMICOLON:
            return "OP_SEMICOLON";
        case TK_OP_OBRACKET:
            return "OP_OBRACKET";
        case TK_OP_CBRACKET:
            return "OP_CBRACKET";
        }
    }

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
            // Try to get the next token
            p1::token tk;
            try
            {
                tk = m_scanner.next_token();
            }
            catch (const p1::scanner_error& error)
            {
                std::cerr << error.really_what() << "\n";
                break;
            }

            // Print basic token info
            std::cout << token_name(tk.type);
            if (tk.content.empty())
            {
                std::cout << "\n";
            }
            else
            {
                std::cout << ": " << tk.content << "\n";
            }

            // Stop at EOF
            // This also avoids printing advanced info on the EOF token
            if (tk.type == TK_EOF)
                break;

            // Print advanced token info
            std::cout << "  begin: line " << tk.line_begin << ", col " << tk.column_begin << "\n";
            std::cout << "  end: line " << tk.line_end << ", col " << tk.column_end << "\n";
        }
    }
};

} // namespace p1

#endif // #ifndef P1_SCANNER_TESTER_H
