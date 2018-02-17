/*
 * Tyler Filla
 * CS 4280 - P0
 */

#ifndef P0_TREE_H
#define P0_TREE_H

#include <set>
#include <string>

namespace p0
{

/**
 * A binary search tree of strings, grouped by their lengths.
 */
class tree
{
    struct node
    {
        /**
         * The parent of this node.
         */
        node* m_parent;

        /**
         * The left child of this node.
         */
        node* m_child_left;

        /**
         * The right child of this node.
         */
        node* m_child_right;

        /**
         * The length of words on this node.
         */
        unsigned int m_length;

        /**
         * The words on this node.
         */
        std::set<std::string> m_words;
    };

    /**
     * The root node of the tree.
     */
    node* m_root;

    /**
     * Visit all nodes in the subtree rooted by the given node using an inorder traversal.
     *
     * @param p_node The subtree root node
     * @param visitor The visitor callable
     *
     * @tparam VisitorT The type of the visitor callable
     */
    template<typename VisitorT>
    void visit_node_inorder(node* p_node, VisitorT visitor)
    {
        if (p_node == nullptr)
            return;

        visit_node_inorder(p_node->m_child_left, visitor);
        visitor(p_node);
        visit_node_inorder(p_node->m_child_right, visitor);
    }

    /**
     * Visit all nodes in the subtree rooted by the given node using a preorder traversal.
     *
     * @param p_node The subtree root node
     * @param visitor The visitor callable
     *
     * @tparam VisitorT The type of the visitor callable
     */
    template<typename VisitorT>
    void visit_node_preorder(node* p_node, VisitorT visitor)
    {
        if (p_node == nullptr)
            return;

        visitor(p_node);
        visit_node_preorder(p_node->m_child_left, visitor);
        visit_node_preorder(p_node->m_child_right, visitor);
    }

    /**
     * Visit all nodes in the subtree rooted by the given node using a postorder traversal.
     *
     * @param p_node The subtree root node
     * @param visitor The visitor callable
     *
     * @tparam VisitorT The type of the visitor callable
     */
    template<typename VisitorT>
    void visit_node_postorder(node* p_node, VisitorT visitor)
    {
        if (p_node == nullptr)
            return;

        visit_node_preorder(p_node->m_child_left, visitor);
        visit_node_preorder(p_node->m_child_right, visitor);
        visitor(p_node);
    }

    /**
     * @hide
     */
    template<typename CallbackT>
    void __search(node* p_node, unsigned int length, node* parent, bool side, CallbackT callback)
    {
        if (p_node == nullptr)
        {
            // Could not find the node
            // Report back enough information to add a node here, if desired
            callback(nullptr, parent, side);
        }
        else
        {
            if (length == p_node->m_length)
            {
                // Node found
                callback(p_node, parent, side);
            }
            else if (length < p_node->m_length)
            {
                // Look to the left
                __search(p_node->m_child_left, length, p_node, false, callback);
            }
            else if (length > p_node->m_length)
            {
                // Look to the right
                __search(p_node->m_child_right, length, p_node, true, callback);
            }
        }
    }

    /**
     * Perform a binary search for the node with the given string length in the subtree rooted by the given node. If
     * the search finds the desired node, the given callback callable is called with a pointer to the node, a pointer
     * to the node's parent, and a flag indicating which side (left or right) the node is relative to its parent.
     *
     * @param p_node The subtree root node
     * @param length The length to search for
     * @param callback The callback callable
     *
     * @tparam CallbackT The type of the callback callable
     */
    template<typename CallbackT>
    void search(node* p_node, unsigned int length, CallbackT callback)
    { __search(p_node, length, nullptr, false, callback); }

public:
    tree();

    template<typename IteratorT>
    tree(IteratorT begin, IteratorT end) : tree()
    { add_words(begin, end); }

    ~tree();

    /**
     * Add a word to the tree.
     * @param word The new word
     */
    void add_word(const std::string& word);

    /**
     * Add a range of words to the tree.
     *
     * @param begin A begin iterator for the range
     * @param end An end iterator for the range
     *
     * @tparam IteratorT The range iterator type
     */
    template<typename IteratorT>
    void add_words(IteratorT begin, IteratorT end)
    {
        for (auto i = begin; i != end; ++i)
        { add_word(*i); }
    }

    /**
     * Perform an inorder traversal of the tree.
     *
     * @param visitor The visitor callable
     * @tparam VisitorT The type of the visitor callable
     */
    template<typename VisitorT>
    void traverse_inorder(VisitorT visitor)
    {
        visit_node_inorder(m_root, [&](node* p_node)
        {
            for (auto&& word : p_node->m_words)
            { visitor(word); }
        });
    }

    /**
     * Perform a preorder traversal of the tree.
     *
     * @param visitor The visitor callable
     * @tparam VisitorT The type of the visitor callable
     */
    template<typename VisitorT>
    void traverse_preorder(VisitorT visitor)
    {
        visit_node_preorder(m_root, [&](node* p_node)
        {
            for (auto&& word : p_node->m_words)
            { visitor(word); }
        });
    }

    /**
     * Perform a postorder traversal of the tree.
     *
     * @param visitor The visitor callable
     * @tparam VisitorT The type of the visitor callable
     */
    template<typename VisitorT>
    void traverse_postorder(VisitorT visitor)
    {
        visit_node_postorder(m_root, [&](node* p_node)
        {
            for (auto&& word : p_node->m_words)
            { visitor(word); }
        });
    }
};

} // namespace p0

#endif // #ifndef P0_TREE_H
