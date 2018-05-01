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
     * An intermediate piece of generated code.
     */
    struct gen
    {
        virtual ~gen() = default;

        virtual std::string get_desc() const
        { return "<unknown>"; }
    };

    /**
     * An intermediate label reference.
     */
    struct label
    {
        /** The user-given name of the label. */
        std::string name;

        /** The target generated code element. */
        gen* target;
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
     * A generated label declaration.
     */
    struct gen_decl_label : gen
    {
        /** The originating label object. */
        label* obj;

        /** The generated label name. */
        std::string name;

        ~gen_decl_label() final = default;

        std::string get_desc() const final
        { return std::string("label: ") + name; }
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

        /** The initial value. */
        int value;

        ~gen_decl_var() final = default;

        std::string get_desc() const final
        { return std::string("var: ") + name; }
    };

    /**
     * A generated BR instruction.
     */
    struct gen_instr_br : gen
    {
        /** The branch target. */
        label* target;

        ~gen_instr_br() final = default;

        std::string get_desc() const final
        { return "br"; }
    };

    /**
     * A generated BRNEG instruction.
     */
    struct gen_instr_brneg : gen
    {
        /** The branch target. */
        label* target;

        ~gen_instr_brneg() final = default;

        std::string get_desc() const final
        { return "brneg"; }
    };

    /**
     * A generated BRZNEG instruction.
     */
    struct gen_instr_brzneg : gen
    {
        /** The branch target. */
        label* target;

        ~gen_instr_brzneg() final = default;

        std::string get_desc() const final
        { return "brzneg"; }
    };

    /**
     * A generated BRPOS instruction.
     */
    struct gen_instr_brpos : gen
    {
        /** The branch target. */
        label* target;

        ~gen_instr_brpos() final = default;

        std::string get_desc() const final
        { return "brpos"; }
    };

    /**
     * A generated BRZPOS instruction.
     */
    struct gen_instr_brzpos : gen
    {
        /** The branch target. */
        label* target;

        ~gen_instr_brzpos() final = default;

        std::string get_desc() const final
        { return "brzpos"; }
    };

    /**
     * A generated BRZERO instruction.
     */
    struct gen_instr_brzero : gen
    {
        /** The branch target. */
        label* target;

        ~gen_instr_brzero() final = default;

        std::string get_desc() const final
        { return "brzero"; }
    };

    /**
     * A generated COPY instruction.
     */
    struct gen_instr_copy : gen
    {
        /** The source variable. */
        var* src;

        /** The destination variable. */
        var* dest;

        ~gen_instr_copy() final = default;

        std::string get_desc() const final
        { return "copy"; }
    };

    /**
     * A generated variable ADD instruction.
     */
    struct gen_instr_add_var : gen
    {
        /** The operand variable. */
        var* rhs;

        ~gen_instr_add_var() final = default;

        std::string get_desc() const final
        { return "variable add"; }
    };

    /**
     * A generated immediate ADD instruction.
     */
    struct gen_instr_add_imm : gen
    {
        /** The immediate operand value. */
        int rhs;

        ~gen_instr_add_imm() final = default;

        std::string get_desc() const final
        { return "immediate add"; }
    };

    /**
     * A generated variable SUB instruction.
     */
    struct gen_instr_sub_var : gen
    {
        /** The operand variable. */
        var* rhs;

        ~gen_instr_sub_var() final = default;

        std::string get_desc() const final
        { return "variable sub"; }
    };

    /**
     * A generated immediate SUB instruction.
     */
    struct gen_instr_sub_imm : gen
    {
        /** The immediate operand value. */
        int rhs;

        ~gen_instr_sub_imm() final = default;

        std::string get_desc() const final
        { return "immediate sub"; }
    };

    /**
     * A generated variable DIV instruction.
     */
    struct gen_instr_div_var : gen
    {
        /** The operand variable. */
        var* rhs;

        ~gen_instr_div_var() final = default;

        std::string get_desc() const final
        { return "variable div"; }
    };

    /**
     * A generated immediate DIV instruction.
     */
    struct gen_instr_div_imm : gen
    {
        /** The immediate operand value. */
        int rhs;

        ~gen_instr_div_imm() final = default;

        std::string get_desc() const final
        { return "immediate div"; }
    };

    /**
     * A generated variable MULT instruction.
     */
    struct gen_instr_mult_var : gen
    {
        /** The operand variable. */
        var* rhs;

        ~gen_instr_mult_var() final = default;

        std::string get_desc() const final
        { return "variable mult"; }
    };

    /**
     * A generated immediate MULT instruction.
     */
    struct gen_instr_mult_imm : gen
    {
        /** The immediate operand value. */
        int rhs;

        ~gen_instr_mult_imm() final = default;

        std::string get_desc() const final
        { return "immediate mult"; }
    };

    /**
     * A generated READ instruction.
     */
    struct gen_instr_read : gen
    {
        /** The destination variable. */
        var* dest;

        ~gen_instr_read() final = default;

        std::string get_desc() const final
        { return "read"; }
    };

    /**
     * A generated variable WRITE instruction.
     */
    struct gen_instr_write_var : gen
    {
        /** The source variable. */
        var* src;

        ~gen_instr_write_var() final = default;

        std::string get_desc() const final
        { return "write variable"; }
    };

    /**
     * A generated immediate WRITE instruction.
     */
    struct gen_instr_write_imm : gen
    {
        /** The immediate value. */
        int value;

        ~gen_instr_write_imm() final = default;

        std::string get_desc() const final
        { return "write immediate"; }
    };

    /**
     * A generated STOP instruction.
     */
    struct gen_instr_stop : virtual gen
    {
        ~gen_instr_stop() final = default;

        std::string get_desc() const final
        { return "stop"; }
    };

    /**
     * A generated STORE instruction.
     */
    struct gen_instr_store : gen
    {
        /** The destination variable. */
        var* dest;

        ~gen_instr_store() final = default;

        std::string get_desc() const final
        { return "store"; }
    };

    /**
     * A generated variable LOAD instruction.
     */
    struct gen_instr_load_var : gen
    {
        /** The source variable. */
        var* src;

        ~gen_instr_load_var() final = default;

        std::string get_desc() const final
        { return "variable load"; }
    };

    /**
     * A generated immediate LOAD instruction.
     */
    struct gen_instr_load_imm : gen
    {
        /** The immediate value. */
        int value;

        ~gen_instr_load_imm() final = default;

        std::string get_desc() const final
        { return "immediate load"; }
    };

    /**
     * A generated NOOP instruction.
     */
    struct gen_instr_noop : gen
    {
        ~gen_instr_noop() final = default;

        std::string get_desc() const final
        { return "noop"; }
    };

    /**
     * A generated PUSH instruction.
     */
    struct gen_instr_push : gen
    {
        ~gen_instr_push() final = default;

        std::string get_desc() const final
        { return "push"; }
    };

    /**
     * A generated POP instruction.
     */
    struct gen_instr_pop : gen
    {
        ~gen_instr_pop() final = default;

        std::string get_desc() const final
        { return "pop"; }
    };

    /**
     * A generated STACKW instruction.
     */
    struct gen_instr_stackw : gen
    {
        /** The stack offset. */
        int offset;

        ~gen_instr_stackw() final = default;

        std::string get_desc() const final
        { return std::string("stackw"); }
    };

    /**
     * A generated STACKR instruction.
     */
    struct gen_instr_stackr : gen
    {
        /** The stack offset. */
        int offset;

        ~gen_instr_stackr() final = default;

        std::string get_desc() const final
        { return std::string("stackr"); }
    };

    /**
     * A relational operator type.
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

    /** Intermediate relational operator type. */
    ro_type m_ro_type;

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
