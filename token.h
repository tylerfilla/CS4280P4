/*
 * Tyler Filla
 * CS 4280
 * Project 1
 */

#ifndef P1_TOKEN_H
#define P1_TOKEN_H

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
    unsigned int begin_column;
    unsigned int begin_line;
    unsigned int end_column;
    unsigned int end_line;
};

} // namespace p1

#endif // #ifndef P1_TOKEN_H
