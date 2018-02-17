/*
 * Tyler Filla
 * CS 4280 - P0
 */

#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "tree.h"

/**
 * Do a traversal using the given traversal callable and output.
 *
 * @param traversal The traversal callable
 * @param output The output
 *
 * @tparam TraversalT The traversal callable type
 */
template<typename TraversalT>
static void do_traversal(TraversalT traversal, std::ostream& output)
{
    unsigned int last_length = 0;
    unsigned int length_count = 0;

    traversal([&](const std::string& word, unsigned int num_length, unsigned int depth)
    {
        if (word.length() != last_length)
        {
            // If not first line, end previous line
            if (last_length != 0)
            {
                output << "\n";
            }

            // Add two-space indents according to depth
            for (unsigned int d = 0; d < depth; ++d)
            {
                output << "  ";
            }

            // Add length prefix
            output << word.length() << " ";

            // Clear length counter
            length_count = 0;
        }

        // Output word itself
        output << word;

        // Output spaces between words, but not after the last word
        if (length_count < num_length - 1)
        {
            output << " ";
        }

        last_length = word.length();
        length_count++;
    });

    // End the last line of the output
    output << "\n";
}

int main(int argc, char* argv[])
{
    // Require that the user supply either zero or one argument
    if (argc > 2)
    {
        std::cerr << "error: invalid arguments\n";
        return 1;
    }

    // The input stream to read from
    // By default, standard input will be used
    std::istream* input_ptr = &std::cin;

    // The name of the input source
    // This will be used as the basenames for the result files
    std::string input_name = "out";

    // If user arguments are provided
    if (argc > 1)
    {
        input_name = argv[1];

        // Build full filename
        auto full_name = input_name + ".sp18";

        // Open file for read
        // HACK: This will get cleaned up on termination
        input_ptr = new std::ifstream(full_name);

        // Make sure file opened correctly
        if (!*input_ptr)
        {
            std::cerr << "error: could not open file " << full_name << "\n";
            return 1;
        }
    }

    auto& input = *input_ptr;

    // An array of all input words
    std::vector<std::string> input_words;

    // Scan words from input
    // Words are delimited by whitespace
    while (true)
    {
        std::string word;
        input >> word;

        if (input.eof())
            break;

        input_words.push_back(std::move(word));
    }

    // An input file with no words is an error
    if (input_words.empty())
    {
        std::cerr << "error: input file contains no words\n";
        return 1;
    }

    // Build tree with input words
    p0::tree word_tree(input_words.begin(), input_words.end());

    // Try to open preorder result file
    std::ofstream output_preorder(input_name + ".preorder");
    if (!output_preorder)
    {
        std::cerr << "error: could not open preorder result file " << input_name << ".preorder\n";
        return 1;
    }

    // Try to open inorder result file
    std::ofstream output_inorder(input_name + ".inorder");
    if (!output_inorder)
    {
        std::cerr << "error: could not open inorder result file " << input_name << ".inorder\n";
        return 1;
    }

    // Try to open postorder result file
    std::ofstream output_postorder(input_name + ".postorder");
    if (!output_postorder)
    {
        std::cerr << "error: could not open postorder result file " << input_name << ".postorder\n";
        return 1;
    }

    {
        using namespace std::placeholders;
        using traversal = typename std::function<void(const std::string&, unsigned int, unsigned int)>;

        // Perform preorder, inorder, and postorder traversals
        do_traversal(std::bind(&p0::tree::traverse_preorder<traversal>, &word_tree, _1), output_preorder);
        do_traversal(std::bind(&p0::tree::traverse_inorder<traversal>, &word_tree, _1), output_inorder);
        do_traversal(std::bind(&p0::tree::traverse_postorder<traversal>, &word_tree, _1), output_postorder);
    }

    return 0;
}
