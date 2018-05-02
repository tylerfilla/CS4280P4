/*
 * Tyler Filla
 * CS 4280
 * Project 4
 */

#ifndef P4_CODEGEN_H
#define P4_CODEGEN_H

#include <sstream>
#include <string>
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
     * program, such as an arithmetic instruction or a variable reference. Some
     * bookkeeping fragments also exist, such as variable scope indicators. This
     * technique creates a flat view of the generated program, rather than a
     * tree view, which makes storage and label allocation/renaming easier.
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
        /** The declared name of the label. */
        std::string name;
    };

    /**
     * A variable declaration.
     */
    struct frag_decl_var : frag
    {
        /** The declared name of the variable. */
        std::string name;

        /** The initial value of the variable. */
        int value;
    };

    /**
     * Label reference fragment.
     */
    struct frag_ref_label : frag
    {
        /** The source program label name. */
        std::string name;
    };

    /**
     * Variable reference fragment.
     */
    struct frag_ref_var : frag
    {
        /** The source program variable name. */
        std::string name;
    };

    /**
     * Instruction fragment. BR. Unconditional branch.
     */
    struct frag_ins_br : frag
    {
        /** The branch target. */
        frag_ref_label* target;
    };

    /**
     * Instruction fragment. BRNEG. Branch if negative.
     */
    struct frag_ins_brneg : frag
    {
        /** The branch target. */
        frag_ref_label* target;
    };

    /**
     * Instruction fragment. BRZNEG. Branch if zero or negative.
     */
    struct frag_ins_brzneg : frag
    {
        /** The branch target. */
        frag_ref_label* target;
    };

    /**
     * Instruction fragment. BRPOS. Branch if positive.
     */
    struct frag_ins_brpos : frag
    {
        /** The branch target. */
        frag_ref_label* target;
    };

    /**
     * Instruction fragment. BRZPOS. Branch if zero or positive.
     */
    struct frag_ins_brzpos : frag
    {
        /** The branch target. */
        frag_ref_label* target;
    };

    /**
     * Instruction fragment. BRZERO. Branch if zero.
     */
    struct frag_ins_brzero : frag
    {
        /** The branch target. */
        frag_ref_label* target;
    };

    /**
     * Instruction fragment. COPY. Copy variable content.
     */
    struct frag_ins_copy : frag
    {
        /** The destination variable operand. */
        frag_ref_var* dest;

        /** The source variable operand. */
        frag_ref_var* src;
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
    struct frag_ins_add_var : frag
    {
        /** The variable rhs. */
        frag_ref_var* rhs;
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
    struct frag_ins_sub_var : frag
    {
        /** The variable rhs. */
        frag_ref_var* rhs;
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
    struct frag_ins_div_var : frag
    {
        /** The variable rhs. */
        frag_ref_var* rhs;
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
    struct frag_ins_mult_var : frag
    {
        /** The variable rhs. */
        frag_ref_var* rhs;
    };

    /**
     * Instruction fragment. READ. Read to variable destination.
     */
    struct frag_ins_read : frag
    {
        /** The variable destination. */
        frag_ref_var* dest;
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
    struct frag_ins_write_var : frag
    {
        /** The variable source. */
        frag_ref_var* src;
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
    struct frag_ins_store : frag
    {
        /** The variable destination. */
        frag_ref_var* dest;
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
    struct frag_ins_load_var : frag
    {
        /** The variable source. */
        frag_ref_var* src;
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
     * A fragment indicating the variable scope narrows.
     */
    struct frag_scope_enter : frag
    {
    };

    /**
     * A fragment indicating the variable scope widens.
     */
    struct frag_scope_leave : frag
    {
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

    /** The type of the last examined relational operator. */
    ro_type m_ro_type;

public:
    codegen();

    codegen(const codegen& rhs) = delete;

    ~codegen() = default;

    codegen& operator=(const codegen& rhs) = delete;

private:
    void stage_1_traverse(tree::node* root);
    void do_stage_1();
    void do_stage_2();
    void do_stage_3();

    /**
     * Compose the final output.
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
