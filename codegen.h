/*
 * Tyler Filla
 * CS 4280
 * Project 4
 */

#ifndef P4_CODEGEN_H
#define P4_CODEGEN_H

#include <functional>
#include <sstream>
#include <string>
#include <vector>

#include "tree.h"

namespace p4
{

/**
 * A code generator for the target VM from a source parse tree.
 */
class codegen
{
    /**
     * A label object.
     */
    struct label
    {
        /** The user-given name of the label. */
        std::string name;
    };

    /**
     * A variable object.
     */
    struct var
    {
        /** The user-given name of the variable. */
        std::string name;
    };

    /**
     * A generated code element.
     */
    struct gen
    {
    };

    /**
     * A generated label declaration.
     */
    struct gen_decl_label : gen
    {
        /** The originating label object. */
        label* obj;

        /** The generated label name. */
        std::string name;
    };

    /**
     * A generated variable declaration.
     */
    struct gen_decl_var : gen
    {
        /** The originating variable object. */
        var* obj;

        /** The generated variable name. */
        std::string name;
    };

    /** The parse tree. */
    tree::node* m_tree;

    /** The generated output code. */
    std::string m_output;

    /** Incomplete generated output. */
    std::ostringstream m_output_ss;

    /** Intermediate label objects. */
    std::vector<label*> m_labels;

    /** Intermediate variable objects. */
    std::vector<var*> m_vars;

    /** Intermediate generated pieces of code. */
    std::vector<gen*> m_gens;

public:
    codegen();

    codegen(const codegen& rhs) = delete;

    ~codegen() = default;

    codegen& operator=(const codegen& rhs) = delete;

private:
    /**
     * Do a parse tree traversal with preorder and postorder callbacks.
     *
     * @param root The traversal root
     * @param cb_pre The preorder callback
     * @param cb_post The postorder callback
     */
    void traverse(tree::node* root, std::function<void(tree::node*)> cb_pre, std::function<void(tree::node*)> cb_post);

    /**
     * Do first pass. This generates instructions and prescribes temporary
     * variables, but leaves variable and label references alone.
     */
    void do_pass_1();

    /**
     * Do second pass. This processes variable references.
     */
    void do_pass_2();

    /**
     * Do third pass. This processes label references.
     */
    void do_pass_3();

    /**
     * Compose the final generated code string.
     */
    void compose();

public:
    /**
     * @return The parse tree
     */
    tree::node* get_tree() const
    { return m_tree; }

    /**
     * @param p_tree The parse tree
     */
    void set_tree(tree::node* p_tree)
    { m_tree = p_tree; }

    /**
     * @return The generated output code
     */
    std::string get_output() const
    { return m_output; }

    /**
     * Run the code generator.
     */
    void run();
};

} // namespace p4

#endif // #ifndef P4_CODEGEN_H
