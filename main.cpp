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

    // Construct character iterators on the input stream
    auto& input = *input_ptr;
    std::istreambuf_iterator<char> input_begin(input);
    std::istreambuf_iterator<char> input_end {};

    // Construct scanner object on input stream
    p2::scanner<decltype(input_begin)> scanner(input_begin, input_end);

    //for (auto&& token : scanner)
    //{
    //    std::cout << token.content << "\n";
    //}

    // Construct token iterators on scanner
    //p2::token_iterator token_begin(scanner);
    //p2::token_iterator token_end {};

    // Construct parser object on scanned token stream
    p2::parser<decltype(input_begin)> parser(scanner.begin(), scanner.end());

    parser.parse(); // FIXME

    return 0;
}
