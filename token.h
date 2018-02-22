/*
 * Tyler Filla
 * CS 4280
 * Project 1
 */

#ifndef P1_TOKEN_H
#define P1_TOKEN_H

#include <string>

namespace p1
{

enum token_type
{
    TK_EOF = 0,
    TK_TYPE1,
    TK_TYPE2,
    TK_TYPE3
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

} // namespace p1

#endif // #ifndef P1_TOKEN_H
