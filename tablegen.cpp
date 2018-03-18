/*
 * Tyler Filla
 * CS 4280
 * Project 1 - Scanner Table Generator
 *
 * I (very roughly) commented around important parts of this program.
 */

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace tg
{

/**
 * A token that hasn't yet been resolved.
 */
struct unresolved_token
{
    std::string code;
};

/**
 * A token that has been resolved.
 */
struct token
{
    int id;
    std::string code;
};

/**
 * A state that hasn't yet been resolved.
 */
struct unresolved_state
{
    std::string name;
};

/**
 * A state that has been resolved.
 */
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

/**
 * A function that maps ASCII to just the alphabet used in the project. This
 * makes the table more compact.
 */
static int T(char ch)
{
    // Group: lowercase letters
    if (ch >= 'a' && ch <= 'z')
        return ch - 'a';

    // Group: uppercase letters
    if (ch >= 'A' && ch <= 'Z')
        return ch - 'A' + 26;

    // Group: decimal digits
    if (ch >= '0' && ch <= '9')
        return ch - '0' + 26 + 26;

    // Miscellaneous
    int base = 26 + 26 + 10;
    switch (ch)
    {
    case '=':
        return base;
    case '<':
        return base + 1;
    case '>':
        return base + 2;
    case ':':
        return base + 3;
    case '+':
        return base + 4;
    case '-':
        return base + 5;
    case '*':
        return base + 6;
    case '/':
        return base + 7;
    case '%':
        return base + 8;
    case '.':
        return base + 9;
    case '(':
        return base + 10;
    case ')':
        return base + 11;
    case ',':
        return base + 12;
    case '{':
        return base + 13;
    case '}':
        return base + 14;
    case ';':
        return base + 15;
    case '[':
        return base + 16;
    case ']':
        return base + 17;
    case '&':
        return base + 18;
    case ' ':   // space
        return base + 19;
    case '\t':  // horizontal tab
        return base + 20;
    case '\r':  // carriage return (CR)
        return base + 21;
    case '\n':  // linefeed (LF)
        return base + 22;
    default:
        // Illegal character (not in input alphabet)
        return -1;
    }
}

/**
 * The number of characters allowed in the alphabet for the project.
 */
static const int ALPHABET_SIZE = 85;

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "arguments invalid\n";
        return 1;
    }

    /*********************/
    /* THE READING PHASE */
    /*********************/

    // Open the table file
    std::ifstream table_file(argv[1]);

    // Read entire table file into string
    // We're not super worried about performance in this program
    std::string table_text;
    {
        std::stringstream ss;
        ss << table_file.rdbuf();
        table_text = std::move(ss.str());
    }

    // Maps that assign names to states and tokens
    // This will be useful for linking at the end
    // This is also what makes tablegen so useful (the ability to name these pesky things, rather than just number them)
    std::map<std::string, tg::state*> states_by_name;
    std::map<std::string, tg::token*> tokens_by_code;

    // The code for the EOF token, which always gets put at the top of the table
    // This was a hacky way to reduce table complexity, since EOF can happen anywhere
    std::string eof_token_code;

    /*********************/
    /* THE PARSING PHASE */
    /*********************/

    //
    // Goal: Parse the file contents and extract as much information as we can until we have to make another pass over the data.
    //

    // Process table line-by-line
    std::string line;
    for (auto&& ch : table_text)
    {
        if (ch == '\n')
        {
            // Skip empty lines
            if (line.empty())
                continue;

            // If not a comment line
            if (line[0] != '#')
            {
                // The first useful line defines the EOF token
                if (eof_token_code.empty())
                {
                    eof_token_code = line;
                    line.clear();
                    continue;
                }

                // Various data for handling tablegen's own parsing state
                bool in_state = false;
                bool out_state = false;
                bool in_filter = false;
                std::string state_name;
                std::string state_filter;

                //
                // Goal: Break the line into a state name and a filter part
                // The filter is the comma-separated zone that defines actions for character groups
                //

                // Process line character-by-character
                for (auto&& line_ch : line)
                {
                    // Toss out leading whitespace
                    if (line_ch == ' ' && !in_state && !out_state)
                        continue;

                    // If not reading a state name yet, start doing so after first non-space
                    if (line_ch != ' ' && !in_state && !out_state)
                    {
                        in_state = true;
                    }

                    // If reading a state name, stop doing so upon getting another space
                    if (line_ch == ' ' && in_state)
                    {
                        in_state = false;
                        out_state = true;
                    }

                    // If done with the state name and at least one space after it, start the filter part
                    if (line_ch != ' ' && out_state)
                    {
                        in_filter = true;
                    }

                    // If reading a state, append character to the running state name
                    if (in_state)
                    {
                        state_name += line_ch;
                    }

                    // If reading a filter part, append character to the running filter text
                    if (in_filter)
                    {
                        state_filter += line_ch;
                    }
                }

                //
                // Goal: Break the comma-separated filter part into individual actions
                //

                // More tablegen state values
                bool in_filter_char = true;
                int filter_target_step = 0;
                bool in_filter_target = false;
                std::string filter_current_char;
                std::string filter_current_target;

                // filter_target_step holds the current state for action parsing
                // A value of 0 means "inside character group" and 1 means "received a minus symbol"
                // This was originally going to be used for more states, but I decided not to
                // There is no technical reason it is not represented as a boolean value
                // This file is a very rough prototype (used in production!)

                // A map from character group to action and a vector to remember their order
                // The first string in each mapping is the character group text
                // The second string in each mapping is the target, which is something like "{TOKEN}" or "STATE"
                // The order of actions is needed to deal with the OTHER-whatever character groups
                std::map<std::string, std::string> state_actions;
                std::vector<std::string> action_order;

                // Iterate character-by-character over the filter part
                // We keep the iterator accessible in the for loop body to determine the last action
                // The last action is not followed by a comma, so it needs special care to not be forgotten
                for (auto i = state_filter.begin(); i != state_filter.end(); ++i)
                {
                    char filter_ch = *i;

                    // If just got a minus, but currently inside the character group
                    if (filter_ch == '-' && filter_target_step == 0)
                    {
                        // Set filter step to 1 (see above note)
                        filter_target_step = 1;
                        continue;
                    }

                    // If already got a minus and just got a greater-than symbol
                    // This makes a cute little arrow symbol that symbolizes action
                    if (filter_ch == '>' && filter_target_step == 1)
                    {
                        // Reset filter step to 0 (ready to read another character group)
                        // Not so fast, however, since we still have to read the target
                        // I don't think it is necessary to set it to zero this early, but it works
                        filter_target_step = 0;

                        // Prepare to read the action's target value
                        in_filter_char = false;
                        in_filter_target = true;
                        continue;
                    }

                    // If (just got a comma OR the last character in the filter part) AND we're reading a target
                    if ((filter_ch == ',' || i + 1 == state_filter.end()) && in_filter_target)
                    {
                        // The special case for not leaving off the last character (mentioned above somewhere)
                        if (i + 1 == state_filter.end())
                        {
                            filter_current_target += filter_ch;
                        }

                        // Prepare to read another action
                        in_filter_char = true;
                        filter_target_step = 0;
                        in_filter_target = false;

                        // Insert a mapping from character group to target AND remember its order in the list
                        state_actions.emplace(filter_current_char, std::move(filter_current_target));
                        action_order.push_back(std::move(filter_current_char));

                        // Re-validate values after moving their contents into the data structures above
                        filter_current_char.clear();
                        filter_current_target.clear();
                        continue;
                    }

                    // If in filter, put new character in filter string
                    if (in_filter_char)
                    {
                        filter_current_char += filter_ch;
                    }

                    // If in target, put new character in target string
                    // This will not capture the last character in the filter part (see above)
                    if (in_filter_target)
                    {
                        filter_current_target += filter_ch;
                    }
                }

                //
                // Goal: Create an instance of the state class above to represent the state from this line
                //

                // This is the canonical representation, in tablegen, of the state defined on this line of the file
                // We do not have a number to give to the state yet, so all references to this state will be called "unresolved" until then
                auto state = new tg::state;
                state->id = -1;
                state->name = state_name;

                //
                // Goal: Process the action's character group
                //

                // A set of all characters used in the character group
                // This is useful for handling the OTHER-whatever character groups
                std::set<int> used_chars;

                // HACK: All the supported letters
                int all_letters[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                        'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                        'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };

                // HACK: All the supported non-alphanumeric symbols
                int all_symbols[] = { '=', '<', '>', ':', '+', '-', '*', '/', '%', '.', '(', ')', ',', '{', '}', ';',
                        '[', ']', '&' };

                // HACK: All the supported characters in the alphabet for the project
                int all_chars[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                        'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                        'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1',
                        '2', '3', '4', '5', '6', '7', '8', '9', '=', '<', '>', ':', '+', '-', '*', '/', '%', '.', '(',
                        ')', ',', '{', '}', ';', '[', ']', '&', ' ', '\t', '\r', '\n' };

                // Iterate over character groups in the order in which they were presented in the table file
                // The action_order structure is the vector from above
                for (auto&& action_char : action_order)
                {
                    // And this is the map from above
                    // Here, we get the target for the action for the current character group
                    // Reminder: this string is of the form "{TOKEN}" or "STATE"
                    // Also reminder: we have created the state object
                    std::string action_target = state_actions[action_char];

                    // The list of characters represented by this particular character group
                    std::set<int> chars;

                    if (action_char == "WS")
                    {
                        // The WS group

                        chars.insert(' ');  // space
                        chars.insert('\t'); // h tab
                        chars.insert('\v'); // v tab
                        chars.insert('\f'); // formfeed
                        chars.insert('\n'); // linefeed
                        chars.insert('\r'); // carriage return
                    }
                    else if (action_char == "LETTERS")
                    {
                        // The LETTERS group

                        for (auto&& letter : all_letters)
                        {
                            chars.insert(letter);
                        }
                    }
                    else if (action_char == "DIGITS")
                    {
                        // The DIGITS group

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
                        // The OTHERLETTERS group

                        for (auto&& letter : all_letters)
                        {
                            if (used_chars.find(letter) == used_chars.end())
                            {
                                chars.insert(letter);
                            }
                        }
                    }
                    else if (action_char == "OTHERS")
                    {
                        // The OTHERS group

                        for (auto&& ch : all_chars)
                        {
                            if (used_chars.find(ch) == used_chars.end())
                            {
                                chars.insert(ch);
                            }
                        }
                    }
                    else if (action_char == "SYMBOLS")
                    {
                        // The SYMBOLS group
                        // This should really be called OTHERSYMBOLS, as it matches all symbols not yet used
                        // However, it is more often used to match all allowed symbols

                        for (auto&& ch : all_symbols)
                        {
                            if (used_chars.find(ch) == used_chars.end())
                            {
                                chars.insert(ch);
                            }
                        }
                    }
                    else if (action_char == "COMMA")
                    {
                        // The COMMA group

                        chars.insert(',');
                    }
                    else if (action_char == "MINUS")
                    {
                        // The MINUS group

                        chars.insert('-');
                    }
                    else
                    {
                        // Complain if the character group doesn't exist
                        // This helped tracked down a segfault at least once during the project

                        if (action_char.length() > 1)
                        {
                            std::cerr << "invalid character class: " << action_char << "\n";
                            return 1;
                        }

                        chars.insert(action_char[0]);
                    }

                    // Remember all character represented by this character group
                    // By storing them here, we can exclude them the next time we use an OTHER-whatever group
                    used_chars.insert(chars.begin(), chars.end());

                    //
                    // Goal: Process the action's target
                    //

                    // If the target is of the form "{TOKEN}" (meaning a token should be accepted)
                    if (action_target[0] == '{' && action_target[action_target.length() - 1] == '}')
                    {
                        // Get the code (usually its name prepended with TK_) of the token
                        std::string token_code = action_target.substr(1, action_target.find("}") - 1);

                        // If the token has never been seen before
                        if (tokens_by_code.find(token_code) == tokens_by_code.end())
                        {
                            // Create an instance of the token class for the token
                            // This is the canonical representation of the token in tablegen
                            auto token = new tg::token;
                            token->id = -1;
                            token->code = token_code;

                            // Store it in the token table
                            tokens_by_code.emplace(token_code, token);
                        }

                        // Iterate over all characters represented by the character group
                        // This is in the context of token matching
                        for (auto&& c : chars)
                        {
                            // Create unresolved token object to represent the token without actually knowing its number
                            // Later, we will worry about linking unresolved_token objects with token objects
                            auto target_token = new tg::unresolved_token;
                            target_token->code = token_code;

                            // Create action to represent accepting this token because of this character
                            // Of course, we don't yet have a canonical link to the token
                            auto action = new tg::state::action;
                            action->verb = tg::state::VERB_ACCEPT_UNRESOLVED; // The _UNRESOLVED suffix here means target points to a tg::unresolved_token object
                            action->target = target_token;

                            // Save it and move on
                            state->actions.emplace(T(c), action);
                        }
                    }
                    else
                    {
                        // Iterate over all characters represented by the character group
                        // This is in the context of state transitioning
                        for (auto&& c : chars)
                        {
                            // Create unresolved state object to represent the state without actually knowing its number
                            // Later, just like with tokens, numbers will be assigned and unresolved_state objects will link with state objects
                            auto target_state = new tg::unresolved_state;
                            target_state->name = action_target;

                            // Create action to represent transitioning to this state because of this character
                            // There is, like with tokens above, no link to the canonical state object
                            auto action = new tg::state::action;
                            action->verb = tg::state::VERB_GOTO_UNRESOLVED; // The _UNRESOLVED suffix here means target points to a tg::unresolved_state object
                            action->target = target_state;

                            // Save it an move on
                            state->actions.emplace(T(c), action);
                        }
                    }
                }

                //
                // We are done reading this state from the file.
                //

                // Store the state object
                // This carries with it references to all the actions, unresolved tokens, and unresolved states we created along the way
                // All token objects were stored in the other tokens_by_code map
                states_by_name.emplace(state_name, state);
            }

            line.clear();
        }
        else
        {
            line += ch;
        }
    }

    /*********************/
    /* THE LINKING PHASE */
    /*********************/

    //
    // Goal: Make a second pass over the data we have collected and assign numbers (IDs) to tokens and states.
    //

    // Assign IDs to states
    // Since we are iterating over a std::map, states will naturally be sorted alphabetically in the final table
    int state_counter = 0;
    for (auto&& state_item : states_by_name)
    {
        auto&& state = state_item.second;
        state->id = state_counter++;
    }

    // Assign IDs to tokens
    // We reserve the ID 0 and do not assign it normally
    int token_counter = 1;
    for (auto&& token_item : tokens_by_code)
    {
        auto&& token = token_item.second;
        token->id = token_counter++;
    }

    // Create the EOF token and give it ID 0
    auto eof_token = new tg::token;
    eof_token->id = 0;
    eof_token->code = eof_token_code;
    tokens_by_code.emplace(eof_token_code, eof_token);

    //
    // Goal: Resolve states and tokens.
    //
    // Make a third pass over the data and replace pointers to
    //     tg::unresolved_token -> the canonical tg::token
    //     tg::unresolved_state -> the canonical tg::state
    //
    // This could have been done before assigning IDs, but it doesn't matter.
    //

    // Iterate over all states
    for (auto&& state_item : states_by_name)
    {
        // Get individual state data
        auto&& state = state_item.second;

        // Iterate over all actions on the state
        for (auto&& action_item : state->actions)
        {
            // Get individual action data
            auto&& action = action_item.second;

            // Process the action if its verb has a _UNRESOLVED prefix
            // This resolves the token or state to which it refers
            // This will allow the code generation phase to know the ID to which the action refers
            switch (action->verb)
            {
            // tg::unresolved_token* -> tg::token*
            case tg::state::VERB_ACCEPT_UNRESOLVED:
                {
                    auto token_ref = static_cast<tg::unresolved_token*>(action->target);
                    action->verb = tg::state::VERB_ACCEPT;
                    action->target = tokens_by_code.find(token_ref->code)->second;
                    delete token_ref;
                }
                break;
            // tg::unresolved_state* -> tg::state*
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

    /*****************************/
    /* THE CODE GENERATION PHASE */
    /*****************************/

    //
    // Goal: Generate the C++ source code for the table.
    //

    std::stringstream table_ss;

    table_ss << "/*\n";
    table_ss << " * Tyler Filla\n";
    table_ss << " * CS 4280\n";
    table_ss << " * Program 1 - Scanner Table\n";
    table_ss << " *\n";
    table_ss << " * This file was generated by tablegen.\n";
    table_ss << " */\n\n";
    table_ss << "#ifndef P1_SCANNER_TABLE_GEN_H\n";
    table_ss << "#define P1_SCANNER_TABLE_GEN_H\n\n";

/*
    table_ss << "enum\n";
    table_ss << "{\n";

    for (auto&& token_item : tokens_by_code)
    {
        auto&& token = token_item.second;
        table_ss << "    " << token->code << ",\n";
    }

    table_ss << "};\n\n";
*/

    table_ss << "#define SCANNER_TABLE_ACCEPT_MASK (1 << 30)\n\n";
    table_ss << "namespace p1\n";
    table_ss << "{\n\n";
    table_ss << "#define ACCEPT(token) (token | SCANNER_TABLE_ACCEPT_MASK)\n\n";
    table_ss << "static const int SCANNER_TABLE[" << states_by_name.size() << "][" << ALPHABET_SIZE << "] = {\n";

    // Iterate over states
    for (auto&& state_item : states_by_name)
    {
        // Get individual state data
        auto&& state = state_item.second;

#define ACTION_FLAG_ACCEPT (1 << 29)
#define ACTION_FLAG_REJECT (1 << 30)

        // Initialize the array of actions to reject everything
        // The array is indexed by the character in the input alphabet (much like the generated table)
        int actions[ALPHABET_SIZE];
        for (auto&& action : actions)
        {
            action = ACTION_FLAG_REJECT;
        }

        // A map from token ID to token code
        // This reframes the tokens_by_code map above for efficiency (not that it really matters)
        std::map<int, std::string> token_codes;

        // Iterate over actions on the state
        // Remember, this is a map from character to action being iterated over
        for (auto&& action_item : state->actions)
        {
            // Get both character and corresponding action
            auto&& ch = action_item.first;
            auto&& action = action_item.second;

            switch (action->verb)
            {
            // If the action is to accept a token
            case tg::state::VERB_ACCEPT:
                {
                    // Store the token ID, masked with the accept bitmask, in the actions array
                    auto token = static_cast<tg::token*>(action->target);
                    actions[ch] = ACTION_FLAG_ACCEPT | token->id;

                    // Remember the token code by ID
                    token_codes[token->id] = token->code;
                }
                break;
            // If the action is to transition to a state
            case tg::state::VERB_GOTO:
                {
                    // Store the state ID directly in the actions array
                    auto state = static_cast<tg::state*>(action->target);
                    actions[ch] = state->id;
                }
                break;
            }
        }

        // Print the name of the state in a comment
        table_ss << "    /*" << state-> name << "*/ { ";

        // Generate the actions for the state as elements in this row of the table
        for (int action : actions)
        {
            if ((action & ACTION_FLAG_ACCEPT) > 0)
            {
                // If accept mask is present, wrap the corresponding token code in "ACCEPT(...)"
                table_ss << "ACCEPT(" << token_codes[action & ~ACTION_FLAG_ACCEPT] << "), ";
            }
            else if ((action & ACTION_FLAG_REJECT) > 0)
            {
                // Signify rejection with a -1 for visual distincation to aid human readers
                table_ss << "-1, ";
            }
            else
            {
                // Signify state transitions as just the ID of the state
                table_ss << action << ", ";
            }
        }
        table_ss << "},\n";

#undef ACTION_FLAG_ACCEPT
#undef ACTION_FLAG_REJECT
    }

    // Conclude the generated file content
    table_ss << "};\n\n";
    table_ss << "#undef ACCEPT\n\n";
    table_ss << "} // namespace p1\n\n";
    table_ss << "#endif // #ifndef P1_SCANNER_TABLE_GEN_H\n\n";

    //
    // Goal: Write the generated file content to the file.
    //

    std::ofstream output_file(argv[2]);
    output_file << table_ss.rdbuf();

    return 0;
}
