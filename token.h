/*
 * Tyler Filla
 * CS 4280
 * Project 3
 */

#ifndef P3_TOKEN_H
#define P3_TOKEN_H

#include <string>

namespace p3
{

enum token_type
{
    TK_EOF = 0,
    TK_IDENTIFIER,
    TK_INTEGER,
    TK_KW_IFF,
    TK_KW_ITER,
    TK_KW_LET,
    TK_KW_PRINT,
    TK_KW_PROGRAM,
    TK_KW_READ,
    TK_KW_RETURN,
    TK_KW_START,
    TK_KW_STOP,
    TK_KW_THEN,
    TK_KW_VAR,
    TK_KW_VOID,
    TK_OP_EQ,
    TK_OP_LT,
    TK_OP_GT,
    TK_OP_COLON,
    TK_OP_PLUS,
    TK_OP_MINUS,
    TK_OP_ASTERISK,
    TK_OP_SLASH,
    TK_OP_PERCENT,
    TK_OP_DOT,
    TK_OP_OPAREN,
    TK_OP_CPAREN,
    TK_OP_COMMA,
    TK_OP_OBRACE,
    TK_OP_CBRACE,
    TK_OP_SEMICOLON,
    TK_OP_OBRACKET,
    TK_OP_CBRACKET,
};

struct token
{
    token_type type;
    std::string content;
    unsigned int line_begin;
    unsigned int line_end;
    unsigned int column_begin;
    unsigned int column_end;
};

} // namespace p3

#endif // #ifndef P3_TOKEN_H
