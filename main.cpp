/*
 * Tyler Filla
 * CS 4280 - P0
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "tree.h"

static std::istream& open_input(int argc, char* argv[])
{
    // The input stream to read from
    // By default, standard input will be used
    std::istream* input = &std::cin;

    // If user arguments are provided
    if (argc > 1)
    {
        // Build full filename
        auto full_name = std::string(argv[1]) + ".sp18";

        // Open file for read
        // HACK: This will get cleaned up on termination
        input = new std::ifstream(full_name);

        // Make sure file opened correctly
        if (!*input)
        {
            std::cerr << "could not open file " << full_name << "\n";
            std::exit(1);
        }
    }

    return *input;
}

int main(int argc, char* argv[])
{
    // Require that the user supply either zero or one argument
    if (argc > 2)
    {
        std::cerr << "too many arguments\n";
        return 1;
    }

    // Get the input file
    auto& input = open_input(argc, argv);

    // An array of all input words
    std::vector<std::string> input_words;

    // Scan words from input
    while (true)
    {
        std::string word;
        input >> word;

        if (input.eof())
            break;

        input_words.push_back(std::move(word));
    }

    // Build tree with input words
    p0::tree tree;
    tree.add_words(input_words.cbegin(), input_words.cend());

    tree.traverse_inorder([](const std::string& word)
    {
        std::cout << ">" << word << "\n";
    });

    return 0;
}
