/*
 * Tyler Filla
 * CS 4280
 * Project 4
 */

#ifndef P4_CODEGEN_H
#define P4_CODEGEN_H

#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "tree.h"

namespace p4
{

/**
 * A code generator for the target language.
 */
class codegen
{
    /**
     * A generated program fragment. This represents a piece of the final
     * program, such as an arithmetic instruction or a label declaration. This
     * technique creates a flat view of the generated program, rather than a
     * tree view, which makes optimization and variable/label renaming easy.
     */
    struct frag
    {
        virtual ~frag() = default;
    };

    /**
     * A label declaration.
     */
    struct frag_decl_label : frag
    {
        /** The label name. */
        std::string name;
    };

    /**
     * A global variable declaration.
     */
    struct frag_decl_gvar : frag
    {
        /** The variable name. */
        std::string name;

        /** The initial value. */
        int value;
    };

    /**
     * Instruction fragment. BR. Unconditional branch.
     */
    struct frag_ins_br : frag
    {
        /** The branch target. */
        int target;
    };

    /**
     * Instruction fragment. BRNEG. Branch if negative.
     */
    struct frag_ins_brneg : frag
    {
        /** The branch target. */
        int target;
    };

    /**
     * Instruction fragment. BRZNEG. Branch if zero or negative.
     */
    struct frag_ins_brzneg : frag
    {
        /** The branch target. */
        int target;
    };

    /**
     * Instruction fragment. BRPOS. Branch if positive.
     */
    struct frag_ins_brpos : frag
    {
        /** The branch target. */
        int target;
    };

    /**
     * Instruction fragment. BRZPOS. Branch if zero or positive.
     */
    struct frag_ins_brzpos : frag
    {
        /** The branch target. */
        int target;
    };

    /**
     * Instruction fragment. BRZERO. Branch if zero.
     */
    struct frag_ins_brzero : frag
    {
        /** The branch target. */
        int target;
    };

    /**
     * Instruction fragment. COPY. Copy variable content.
     */
    struct frag_ins_copy : frag
    {
        /** The destination variable operand. */
        int dest;

        /** The source variable operand. */
        int src;
    };

    /**
     * Instruction fragment. ADD (imm). Add immediate rhs.
     */
    struct frag_ins_add_imm : frag
    {
        /** The immediate rhs. */
        int rhs;
    };

    /**
     * Instruction fragment. ADD (var). Add variable rhs.
     */
    struct frag_ins_add_gvar : frag
    {
        /** The variable rhs. */
        int rhs;
    };

    /**
     * Instruction fragment. SUB (imm). Subtract immediate rhs.
     */
    struct frag_ins_sub_imm : frag
    {
        /** The immediate rhs. */
        int rhs;
    };

    /**
     * Instruction fragment. SUB (var). Subtract variable rhs.
     */
    struct frag_ins_sub_gvar : frag
    {
        /** The variable rhs. */
        int rhs;
    };

    /**
     * Instruction fragment. DIV (imm) Divide by immediate rhs.
     */
    struct frag_ins_div_imm : frag
    {
        /** The immediate rhs. */
        int rhs;
    };

    /**
     * Instruction fragment. DIV (var). Divide by variable rhs.
     */
    struct frag_ins_div_gvar : frag
    {
        /** The variable rhs. */
        int rhs;
    };

    /**
     * Instruction fragment. MULT (imm). Multiply by immediate rhs.
     */
    struct frag_ins_mult_imm : frag
    {
        /** The immediate rhs. */
        int rhs;
    };

    /**
     * Instruction fragment. MULT (var). Multiply by variable rhs.
     */
    struct frag_ins_mult_gvar : frag
    {
        /** The variable rhs. */
        int rhs;
    };

    /**
     * Instruction fragment. READ. Read to variable destination.
     */
    struct frag_ins_read_gvar : frag
    {
        /** The variable destination. */
        int dest;
    };

    /**
     * Instruction fragment. WRITE (imm). Write from immediate source.
     */
    struct frag_ins_write_imm : frag
    {
        /** The immediate source. */
        int src;
    };

    /**
     * Instruction fragment. WRITE (var). Write from variable source.
     */
    struct frag_ins_write_gvar : frag
    {
        /** The variable source. */
        int src;
    };

    /**
     * Instruction fragment. STOP. Stop the program.
     */
    struct frag_ins_stop : frag
    {
    };

    /**
     * Instruction fragment. STORE. Store from ACC to variable destination.
     */
    struct frag_ins_store_gvar : frag
    {
        /** The variable destination. */
        int dest;
    };

    /**
     * Instruction fragment. LOAD (imm). Load to ACC from immediate source.
     */
    struct frag_ins_load_imm : frag
    {
        /** The immediate source. */
        int src;
    };

    /**
     * Instruction fragment. LOAD (var). Load to ACC from variable source.
     */
    struct frag_ins_load_gvar : frag
    {
        /** The variable source. */
        int src;
    };

    /**
     * Instruction fragment. NOOP. Do nothing.
     */
    struct frag_ins_noop : frag
    {
    };

    /**
     * Instruction fragment. PUSH. Push from ACC onto the stack.
     */
    struct frag_ins_push : frag
    {
    };

    /**
     * Instruction fragment. POP. Remove the top of the stack.
     */
    struct frag_ins_pop : frag
    {
    };

    /**
     * Instruction fragment. STACKW. Read stack by offset.
     */
    struct frag_ins_stackw : frag
    {
        /** The stack offset. */
        int offset;
    };

    /**
     * Instruction fragment. STACKR. Read stack by offset.
     */
    struct frag_ins_stackr : frag
    {
        /** The stack offset. */
        int offset;
    };

    /**
     * The type of a relational operator.
     */
    enum class ro_type
    {
        NONE = 0,
        LT,
        LTE,
        GT,
        GTE,
        EQ,
        NEQ,
    };

    /** The parse tree. */
    tree::node* m_tree;

    /** The final output. */
    std::string m_output;

    /** The intemediate output builder. */
    std::ostringstream m_output_ss;

    /** The intermediate program fragments. */
    std::vector<frag*> m_frags;

    /** A map of global variables and their initial values. */
    std::map<int, std::pair<std::string, int>> m_globals;

    /** The intermediate variable names. */
    std::vector<std::string> m_vars;

    /** The intermediate scope boundaries. */
    std::vector<int> m_scopes;

    /** The number to assign to the next global variable. */
    int m_next_global;

    /** The reserved-so-far labels. */
    std::map<int, std::string> m_labels;

    /** The number to assign to the next label. */
    int m_next_label;

    /** The type of the last examined relational operator. */
    ro_type m_ro_type;

public:
    codegen();

    codegen(const codegen& rhs) = delete;

    ~codegen() = default;

    codegen& operator=(const codegen& rhs) = delete;

private:
    /**
     * Enter a scope.
     */
    void push_scope();

    /**
     * Leave a scope.
     */
    void pop_scope();

    /**
     * Mangle a user variable so that it doesn't collide with any keywords,
     * temporaries, or other fixtures in the target language.
     *
     * @param name The variable name
     * @return The mangled varible name
     */
    std::string mangle_user_variable(std::string name);

    /**
     * Find a variable's in-scope location by name.
     *
     * @param name The variable name
     * @return The variable location
     */
    int locate_variable(std::string name);

    /**
     * Declare a temporary global variable. Some generated instructions are not
     * compatible with local variables, so this closes the gap.
     *
     * @param name The variable name (default "")
     * @param value The initial value (default 0)
     * @return The variable location
     */
    int make_temp_gvar(std::string name = "", int value = 0);

    /**
     * Reserve a new label.
     *
     * @param name The label name
     * @return The label number
     */
    int reserve_label(std::string name = "");

    /**
     * Append a label declaration fragment for the given label.
     *
     * @param label The label number
     */
    void place_label(int label);

    /**
     * Do a code generation traversal. This contributes to the array of abstract
     * code fragments, which can then be easily translated into an executable
     * string form.
     *
     * @param root The tree root
     */
    void traverse(tree::node* root);

    /**
     * Optimize the generated code fragments.
     */
    void optimize();

    /**
     * Perform final output composition. This builds the executable string.
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
     * @return The final output
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
