/*
 * Tyler Filla
 * CS 4280
 * Project 3
 */

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

#include "analyzer.h"
#include "parser.h"
#include "scanner.h"
#include "token.h"
#include "tree.h"

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
    p3::scanner<input_iterator> scanner(input_iterator(input), input_iterator {});
    p3::parser<input_iterator>* parser;

    // Try to parse the input
    try
    {
        parser = new p3::parser<input_iterator>(scanner.scan_begin(), scanner.scan_end());
        parser->parse();
    }
    catch (const p3::scanner_error& e)
    {
        std::cerr << "an error has occurred while scanning the input\n";
        std::cerr << "error: " << e.really_what() << "\n";
        return 1;
    }
    catch (const p3::parser_error& e)
    {
        std::cerr << "an error has occurred while parsing the input\n";
        std::cerr << "error: " << e.really_what() << "\n";
        return 1;
    }

    // Get the parse tree
    auto tree = parser->get_tree();

    // Create analyzer for parse tree
    p3::analyzer analyzer(tree);

    // Try to analyze the tree
    try
    {
        analyzer.run();
    }
    catch (const p3::analyzer_error& e)
    {
        std::cerr << "an error has occurred while analyzing the input\n";
        std::cerr << "error: " << e.really_what() << "\n";
        return 1;
    }

    return 0;
}
