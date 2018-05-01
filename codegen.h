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
     * An intermediate label reference.
     */
    struct label
    {
        /** The user-given name of the label. */
        std::string name;
    };

    /**
     * An intermediate variable reference.
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

    /**
     * A generated READ instruction.
     */
    struct gen_instr_read : gen
    {
        /** The destination variable. */
        var* dest;
    };

    /**
     * A generated variable WRITE instruction.
     */
    struct gen_instr_write_var : gen
    {
        /** The source variable. */
        var* src;
    };

    /**
     * A generated immediate WRITE instruction.
     */
    struct gen_instr_write_imm : gen
    {
        /** The immediate value. */
        int value;
    };

    /**
     * A generated STOP instruction.
     */
    struct gen_instr_stop : gen
    {
    };

    /**
     * A generated STORE instruction.
     */
    struct gen_instr_store : gen
    {
        /** The destination variable. */
        var* dest;
    };

    /** The parse tree. */
    tree::node* m_tree;

    /** The generated output code. */
    std::string m_output;

    /** Incomplete generated output. */
    std::ostringstream m_output_ss;

    /** Intermediate label references. */
    std::vector<label*> m_labels;

    /** Intermediate variable references. */
    std::vector<var*> m_vars;

    /** Intermediate generated code fragments. */
    std::vector<gen*> m_gens;

public:
    codegen();

    codegen(const codegen& rhs) = delete;

    ~codegen() = default;

    codegen& operator=(const codegen& rhs) = delete;

private:
    /**
     * Traverse the parse tree as part of stage 1.
     * @param root The traversal root
     */
    void stage_1_traverse(tree::node* root);

    /**
     * Do first stage. This generates instructions.
     */
    void do_stage_1();

    /**
     * Do second stage. This processes variable references.
     */
    void do_stage_2();

    /**
     * Do third stage. This processes label references.
     */
    void do_stage_3();

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
