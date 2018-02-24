/*
 * Tyler Filla
 * CS 4280
 * Project 1 - Table Generator
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace tg
{

struct unresolved_token
{
    std::string code;
};

struct token
{
    int id;
    std::string code;
};

struct unresolved_state
{
    std::string name;
};

struct state
{
    enum
    {
        VERB_ACCEPT,
        VERB_ACCEPT_UNRESOLVED,
        VERB_GOTO,
        VERB_GOTO_UNRESOLVED,
    };

    struct action
    {
        int verb;
        void* target;
    };

    int id;
    std::string name;
    std::map<int, action*> actions;
};

} // namespace tg

int main(int argc, char* argv[])
{
    std::cout << "READING table.txt\n";

    std::ifstream table_file("table.txt");

    std::string table_text;
    {
        std::stringstream ss;
        ss << table_file.rdbuf();
        table_text = std::move(ss.str());
    }

    std::map<std::string, tg::state*> states_by_name;
    std::map<std::string, tg::token*> tokens_by_code;

    std::string line;
    for (auto&& ch : table_text)
    {
        if (ch == '\n')
        {
            if (line.empty())
                continue;

            if (line[0] != '#')
            {
                bool in_state = false;
                bool out_state = false;
                bool in_filter = false;
                std::string state_name;
                std::string state_filter;

                for (auto&& line_ch : line)
                {
                    if (line_ch == ' ' && !in_state && !out_state)
                        continue;

                    if (line_ch != ' ' && !in_state && !out_state)
                    {
                        in_state = true;
                    }

                    if (line_ch == ' ' && in_state)
                    {
                        in_state = false;
                        out_state = true;
                    }

                    if (line_ch != ' ' && out_state)
                    {
                        in_filter = true;
                    }

                    if (in_state)
                    {
                        state_name += line_ch;
                    }

                    if (in_filter)
                    {
                        state_filter += line_ch;
                    }
                }

                bool in_filter_char = true;
                int filter_target_step = 0;
                bool in_filter_target = false;
                std::string filter_current_char;
                std::string filter_current_target;

                std::map<std::string, std::string> state_actions;

                for (auto i = state_filter.begin(); i != state_filter.end(); ++i)
                {
                    char filter_ch = *i;

                    if (filter_ch == '-' && filter_target_step == 0)
                    {
                        filter_target_step = 1;
                        continue;
                    }

                    if (filter_ch == '>' && filter_target_step == 1)
                    {
                        filter_target_step = 0;
                        in_filter_char = false;
                        in_filter_target = true;
                        continue;
                    }

                    if ((filter_ch == ',' || i + 1 == state_filter.end()) && in_filter_target)
                    {
                        if (i + 1 == state_filter.end())
                        {
                            filter_current_target += filter_ch;
                        }

                        in_filter_char = true;
                        filter_target_step = 0;
                        in_filter_target = false;

                        state_actions.emplace(std::move(filter_current_char), std::move(filter_current_target));

                        filter_current_char.clear();
                        filter_current_target.clear();
                        continue;
                    }

                    if (in_filter_char)
                    {
                        filter_current_char += filter_ch;
                    }

                    if (in_filter_target)
                    {
                        filter_current_target += filter_ch;
                    }
                }

                auto state = new tg::state;
                state->id = -1;
                state->name = state_name;

                std::set<int> used_chars;
                int all_letters[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                        'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                        'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };

                for (auto&& action : state_actions)
                {
                    std::string action_char = action.first;
                    std::string action_target = action.second;

                    std::set<int> chars;

                    if (action_char == "WS")
                    {
                        chars.insert(' ');  // space
                        chars.insert('\t'); // h tab
                        chars.insert('\v'); // v tab
                        chars.insert('\f'); // formfeed
                        chars.insert('\n'); // linefeed
                        chars.insert('\r'); // carriage return
                    }
                    else if (action_char == "LETTERS")
                    {
                        for (auto&& letter : all_letters)
                        {
                            chars.insert(letter);
                        }
                    }
                    else if (action_char == "DIGITS")
                    {
                        chars.insert('0');
                        chars.insert('1');
                        chars.insert('2');
                        chars.insert('3');
                        chars.insert('4');
                        chars.insert('5');
                        chars.insert('6');
                        chars.insert('7');
                        chars.insert('8');
                        chars.insert('9');
                    }
                    else if (action_char == "OTHERLETTERS")
                    {
                        for (auto&& letter : all_letters)
                        {
                            if (used_chars.find(letter) == used_chars.end())
                            {
                                chars.insert(letter);
                            }
                        }
                    }
                    else
                    {
                        chars.insert(action_char[0]);
                    }

                    used_chars.insert(chars.begin(), chars.end());

                    if (action_target[0] == '{' && action_target[action_target.length() - 1] == '}')
                    {
                        std::string token_code = action_target.substr(1, action_target.find("}") - 1);

                        if (tokens_by_code.find(token_code) == tokens_by_code.end())
                        {
                            auto token = new tg::token;
                            token->id = -1;
                            token->code = token_code;

                            tokens_by_code.emplace(token_code, token);
                        }

                        for (auto&& c : chars)
                        {
                            auto target_token = new tg::unresolved_token;
                            target_token->code = token_code;

                            auto action = new tg::state::action;
                            action->verb = tg::state::VERB_ACCEPT_UNRESOLVED;
                            action->target = target_token;

                            state->actions.emplace(c, action);
                        }
                    }
                    else
                    {
                        for (auto&& c : chars)
                        {
                            auto target_state = new tg::unresolved_state;
                            target_state->name = action_target;

                            auto action = new tg::state::action;
                            action->verb = tg::state::VERB_GOTO_UNRESOLVED;
                            action->target = target_state;

                            state->actions.emplace(c, action);
                        }
                    }
                }

                states_by_name.emplace(state_name, state);
            }

            line.clear();
        }
        else
        {
            line += ch;
        }
    }

    std::cout << "\nALLOCATING STATES (assigning numeric IDs to state names)\n";

    int state_counter = 0;
    for (auto&& state_item : states_by_name)
    {
        auto&& state = state_item.second;
        state->id = state_counter++;
        std::cout << "  " << state->id << " <- " << state->name << "\n";
    }

    std::cout << "\nALLOCATING TOKENS (assigning numeric IDs to token codes)\n";

    int token_counter = 1;
    for (auto&& token_item : tokens_by_code)
    {
        auto&& token = token_item.second;
        token->id = token_counter++;
        std::cout << "  " << token->id << " <- " << token->code << "\n";
    }

    std::cout << "\nRESOLVING REFERENCES (linking references to states and tokens)\n";

    for (auto&& state_item : states_by_name)
    {
        auto&& state = state_item.second;

        for (auto&& action_item : state->actions)
        {
            auto&& action = action_item.second;

            switch (action->verb)
            {
            case tg::state::VERB_ACCEPT_UNRESOLVED:
                {
                    auto token_ref = static_cast<tg::unresolved_token*>(action->target);
                    action->verb = tg::state::VERB_ACCEPT;
                    action->target = tokens_by_code.find(token_ref->code)->second;
                    delete token_ref;
                }
                break;
            case tg::state::VERB_GOTO_UNRESOLVED:
                {
                    auto state_ref = static_cast<tg::unresolved_state*>(action->target);
                    action->verb = tg::state::VERB_GOTO;
                    action->target = states_by_name.find(state_ref->name)->second;
                    delete state_ref;
                }
                break;
            }
        }
    }

    std::cout << "  " << states_by_name.size() << " states\n";
    std::cout << "  " << tokens_by_code.size() << " tokens\n";

    std::cout << "\nGENERATING TABLE\n";

    std::stringstream table_ss;

    table_ss << "/*\n";
    table_ss << " * Tyler Filla\n";
    table_ss << " * CS 4280\n";
    table_ss << " * Program 1 - Lexer Table\n";
    table_ss << " *\n";
    table_ss << " * This file was auto-generated by tablegen.\n";
    table_ss << " */\n\n";
    table_ss << "#ifndef P1_TABLE_GEN_H\n";
    table_ss << "#define P1_TABLE_GEN_H\n\n";

    table_ss << "enum\n";
    table_ss << "{\n";

    for (auto&& token_item : tokens_by_code)
    {
        auto&& token = token_item.second;
        table_ss << "    " << token->code << ",\n";
    }

    table_ss << "};\n\n";

    table_ss << "#define ACCEPT(token) (token & (1 << 30))\n\n";
    table_ss << "static const int LEXER_TABLE[" << states_by_name.size() << "][256] = {\n";

    for (auto&& state_item : states_by_name)
    {
        auto&& state = state_item.second;

#define ACTION_FLAG_ACCEPT (1 << 29)
#define ACTION_FLAG_REJECT (1 << 30)

        int actions[256];
        for (auto&& action : actions)
        {
            action = ACTION_FLAG_REJECT;
        }

        std::map<int, std::string> token_codes;

        std::cout << "  STATE " << state->id << " (" << state->name << ")\n";
        for (auto&& action_item : state->actions)
        {
            auto&& ch = action_item.first;
            auto&& action = action_item.second;

            std::cout << "    ACTION " << ch << " ";

            switch (ch)
            {
            case ' ':
                std::cout << "[SPACE]";
                break;
            case '\t':
                std::cout << "[HTAB]";
                break;
            case '\v':
                std::cout << "[VTAB]";
                break;
            case '\f':
                std::cout << "[FF]";
                break;
            case '\n':
                std::cout << "[LF]";
                break;
            case '\r':
                std::cout << "[CR]";
                break;
            default:
                std::cout << "'" << static_cast<char>(ch) << "'";
                break;
            }

            std::cout << " -> ";

            switch (action->verb)
            {
            case tg::state::VERB_ACCEPT:
                {
                    auto token = static_cast<tg::token*>(action->target);
                    std::cout << "ACCEPT TOKEN " << token->id << " (" << token->code << ")";
                    actions[ch] = ACTION_FLAG_ACCEPT | token->id;
                    token_codes[token->id] = token->code;
                }
                break;
            case tg::state::VERB_GOTO:
                {
                    auto state = static_cast<tg::state*>(action->target);
                    std::cout << "GOTO STATE " << state->id << " (" << state->name << ")";
                    actions[ch] = state->id;
                }
                break;
            }

            std::cout << "\n";
        }

        table_ss << "    {";
        for (int action : actions)
        {
            if ((action & ACTION_FLAG_ACCEPT) > 0)
            {
                table_ss << "ACCEPT(" << token_codes[action & ~ACTION_FLAG_ACCEPT] << "), ";
            }
            else if ((action & ACTION_FLAG_REJECT) > 0)
            {
                table_ss << "-1, ";
            }
            else
            {
                table_ss << action << ", ";
            }
        }
        table_ss << "},\n";

#undef ACTION_FLAG_ACCEPT
#undef ACTION_FLAG_REJECT
    }

    table_ss << "};\n\n";
    table_ss << "#undef ACCEPT\n\n";
    table_ss << "#endif // #ifndef P1_TABLE_GEN_H\n\n";

    std::cout << "\nWRITING table.gen.h\n";

    std::ofstream output_file("table.gen.h");
    output_file << table_ss.rdbuf();

    return 0;
}
