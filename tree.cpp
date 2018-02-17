/*
 * Tyler Filla
 * CS4280 - P0
 */

#include <iostream> // FIXME
#include "tree.h"

p0::tree::tree() : m_root(nullptr)
{
    m_root = new p0::tree::node {};
}

p0::tree::~tree()
{
    visit_node_postorder(m_root, [](p0::tree::node* node) { delete node; });
}

void p0::tree::add_word(const std::string& word)
{
    std::cout << "adding word " << word << "\n";

    // If root is empty, add word to it
    if (m_root->m_words.size() == 0)
    {
        m_root->m_length = word.length();
        m_root->m_words.insert(word);
        return;
    }

    // Search for a position in the tree for the word
    // Insert the word at this position
    search(m_root, word.length(), [&](p0::tree::node* node, p0::tree::node* parent, bool side)
    {
        std::cout << "callback; node: " << node << ", parent: " << parent << ", side: " << side << "\n";

        // If no node for this word was found
        if (node == nullptr)
        {
            std::cout << "hit null, so making new node for " << word.length() << "\n";

            // Create new node for the word
            auto node = new p0::tree::node {};
            node->m_parent = parent;
            node->m_length = word.length();
            node->m_words.insert(word);

            // Add node to tree
            if (side)
            {
                std::cout << "placed on right\n";
                parent->m_child_right = node;
            }
            else
            {
                std::cout << "placed on left\n";
                parent->m_child_left = node;
            }
        }
        else
        {
            std::cout << "found node " << node->m_length << "\n";

            // A node was found, so just add the word to it
            if (node->m_length == 0 || node->m_length == word.length())
            {
                node->m_length = word.length();
                node->m_words.insert(word);
            }
        }
    });
}
