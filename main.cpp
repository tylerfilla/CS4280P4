/*
 * Tyler Filla
 * CS 4280
 * Project 2
 */

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

#include "parser.h"
#include "scanner.h"
#include "token.h"
#include "tree.h"

static std::ostream& operator<<(std::ostream& os, const p2::token& tk)
{
    switch (tk.type)
    {
    case p2::TK_EOF:
        return os << "eof";
    case p2::TK_IDENTIFIER:
        return os << std::string("identifier \"") + tk.content + std::string("\"");
    case p2::TK_INTEGER:
        return os << std::string("integer ") + tk.content;
    case p2::TK_KW_IFF:
    case p2::TK_KW_ITER:
    case p2::TK_KW_LET:
    case p2::TK_KW_PRINT:
    case p2::TK_KW_PROGRAM:
    case p2::TK_KW_READ:
    case p2::TK_KW_RETURN:
    case p2::TK_KW_START:
    case p2::TK_KW_STOP:
    case p2::TK_KW_THEN:
    case p2::TK_KW_VAR:
    case p2::TK_KW_VOID:
        return os << std::string("keyword \"") + tk.content + std::string("\"");
    case p2::TK_OP_EQ:
    case p2::TK_OP_LT:
    case p2::TK_OP_GT:
    case p2::TK_OP_COLON:
    case p2::TK_OP_PLUS:
    case p2::TK_OP_MINUS:
    case p2::TK_OP_ASTERISK:
    case p2::TK_OP_SLASH:
    case p2::TK_OP_PERCENT:
    case p2::TK_OP_DOT:
    case p2::TK_OP_OPAREN:
    case p2::TK_OP_CPAREN:
    case p2::TK_OP_COMMA:
    case p2::TK_OP_OBRACE:
    case p2::TK_OP_CBRACE:
    case p2::TK_OP_SEMICOLON:
    case p2::TK_OP_OBRACKET:
    case p2::TK_OP_CBRACKET:
        return os << std::string("operator ") + tk.content;
    }
}

static std::string stringify_node(p2::tree::node* node, int indent = 0, std::string prefix = "")
{
    std::ostringstream ss;
    ss << std::string(indent, ' ');
    ss << prefix;

    if (node)
    {
        ss << node->get_prod();
        ss << " ";
    }
    else
    {
        ss << "epsilon\n";
        return ss.str();
    }

    if (auto n = dynamic_cast<p2::tree::node_program*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_vars, indent + 2, "(nd_vars) ");
        ss << stringify_node(n->nd_block, indent + 2, "(nd_block) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_block*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_vars, indent + 2, "(nd_vars) ");
        ss << stringify_node(n->nd_stats, indent + 2, "(nd_stats) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_vars*>(node))
    {
        ss << "where tk_name = " << n->tk_name;
        ss << " ";
        ss << "and tk_value = " << n->tk_value;
        ss << "\n";
        ss << stringify_node(n->nd_mvars, indent + 2, "(nd_mvars) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_mvars_p1*>(node))
    {
        ss << "\n";
    }
    else if (auto n = dynamic_cast<p2::tree::node_mvars_p2*>(node))
    {
        ss << "where tk_identifier = " << n->tk_identifier;
        ss << "\n";
        ss << stringify_node(n->nd_mvars, indent + 2, "(nd_mvars) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_expr*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_M, indent + 2, "(nd_M) ");
        ss << stringify_node(n->nd_expr_2, indent + 2, "(nd_expr_2) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_expr_2_p1*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_rhs, indent + 2, "(nd_rhs) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_expr_2_p2*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_rhs, indent + 2, "(nd_rhs) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_expr_2_p3*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_rhs, indent + 2, "(nd_rhs) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_expr_2_p4*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_rhs, indent + 2, "(nd_rhs) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_M_p1*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_M, indent + 2, "(nd_M) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_M_p2*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_R, indent + 2, "(nd_R) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_R_p1*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_expr, indent + 2, "(nd_expr) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_R_p2*>(node))
    {
        ss << "where tk_identifier = " << n->tk_identifier;
        ss << "\n";
    }
    else if (auto n = dynamic_cast<p2::tree::node_R_p3*>(node))
    {
        ss << "where tk_integer = " << n->tk_integer;
        ss << "\n";
    }
    else if (auto n = dynamic_cast<p2::tree::node_stats*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_stat, indent + 2, "(nd_stat) ");
        ss << stringify_node(n->nd_mStat, indent + 2, "(nd_mStat) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_mStat*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_stat, indent + 2, "(nd_stat) ");
        ss << stringify_node(n->nd_mStat, indent + 2, "(nd_mStat) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_stat_p1*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_in, indent + 2, "(nd_in) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_stat_p2*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_out, indent + 2, "(nd_out) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_stat_p3*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_block, indent + 2, "(nd_block) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_stat_p4*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_if, indent + 2, "(nd_if) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_stat_p5*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_loop, indent + 2, "(nd_loop) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_stat_p6*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_assign, indent + 2, "(nd_assign) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_in*>(node))
    {
        ss << "where tk_identifier = " << n->tk_identifier;
        ss << "\n";
    }
    else if (auto n = dynamic_cast<p2::tree::node_out*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_expr, indent + 2, "(nd_expr) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_if*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_lhs, indent + 2, "(nd_lhs) ");
        ss << stringify_node(n->nd_operator, indent + 2, "(nd_operator) ");
        ss << stringify_node(n->nd_rhs, indent + 2, "(nd_rhs) ");
        ss << stringify_node(n->nd_body, indent + 2, "(nd_body) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_loop*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_lhs, indent + 2, "(nd_lhs) ");
        ss << stringify_node(n->nd_operator, indent + 2, "(nd_operator) ");
        ss << stringify_node(n->nd_rhs, indent + 2, "(nd_rhs) ");
        ss << stringify_node(n->nd_body, indent + 2, "(nd_body) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_assign*>(node))
    {
        ss << "where tk_name = " << n->tk_name;
        ss << "\n";
        ss << stringify_node(n->nd_value, indent + 2, "(nd_value) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_RO_p1*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_lt, indent + 2, "(nd_lt) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_RO_p2*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_gt, indent + 2, "(nd_gt) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_RO_p3*>(node))
    {
        ss << "\n";
        ss << stringify_node(n->nd_eq, indent + 2, "(nd_eq) ");
    }
    else if (auto n = dynamic_cast<p2::tree::node_RO_lt*>(node))
    {
        ss << "\n";
    }
    else if (auto n = dynamic_cast<p2::tree::node_RO_gt*>(node))
    {
        ss << "\n";
    }
    else if (auto n = dynamic_cast<p2::tree::node_RO_eq*>(node))
    {
        ss << "\n";
    }
    else
    {
        ss << "???\n";
    }

    return ss.str();
}

int main(int argc, char* argv[])
{
    // Read from standard input by default
    std::istream* input_ptr = &std::cin;

    // If the user provided arguments
    if (argc > 1)
    {
        // Get the basename from first user argument
        // Concatenate the *.sp18 file extension to build the file name
        auto file_name = std::string(argv[1]) + ".sp18";

        // Try to open the given file
        // HACK: This will get cleaned up on termination
        input_ptr = new std::ifstream(file_name, std::ios::binary);

        if (!*input_ptr)
        {
            std::cerr << "error: could not open file " << file_name << "\n";
            return 1;
        }
    }

    // Input stream
    auto& input = *input_ptr;

    // Scanner and parser
    using input_iterator = typename std::istreambuf_iterator<char>;
    p2::scanner<input_iterator> scanner(input_iterator(input), input_iterator {});
    p2::parser<input_iterator>* parser;

    // Try to parse the input
    try
    {
        parser = new p2::parser<input_iterator>(scanner.scan_begin(), scanner.scan_end());
        parser->parse();
    }
    catch (const p2::scanner_error& e)
    {
        std::cerr << "an error has occurred while scanning the input\n";
        std::cerr << "error: " << e.really_what() << "\n";
        return 1;
    }
    catch (const p2::parser_error& e)
    {
        std::cerr << "an error has occurred while parsing the input\n";
        std::cerr << "error: " << e.really_what() << "\n";
        return 1;
    }

    // Get the parse tree
    auto tree = parser->get_tree();

    // Print the parse tree
    std::cout << "(root) " << stringify_node(tree);

    return 0;
}
