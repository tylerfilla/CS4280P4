/*
 * Tyler Filla
 * CS 4280
 * Project 2
 */

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

#include "parser.h"
#include "scanner.h"

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

    // Prepare scanner and parser on input stream
    auto& input = *input_ptr;
    using input_iterator = typename std::istreambuf_iterator<char>;
    p2::scanner<input_iterator> scanner(input_iterator(input), input_iterator {});
    p2::parser<input_iterator> parser(scanner.scan_begin(), scanner.scan_end());

    // Parse the input
    auto tree = parser.parse();

    // TODO: Work with the parse tree

    return 0;
}
