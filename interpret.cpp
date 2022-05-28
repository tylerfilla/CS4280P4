#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

struct Instruction {
    virtual ~Instruction() = default;

    /**
     * Execute the instruction.
     */
    virtual void execute() = 0;
};

struct InstructionNoop : Instruction {
    void execute() final;
};

struct InstructionStop : Instruction {
    void execute() final;
};

struct InstructionLoadImmediate : Instruction {
    int value;

    explicit InstructionLoadImmediate(int value)
            : value(value) {
    }

    void execute() final;
};

struct InstructionLoadGlobal : Instruction {
    std::string variableName;

    explicit InstructionLoadGlobal(std::string variableName)
            : variableName(std::move(variableName)) {
    }

    void execute() final;
};

struct InstructionStore : Instruction {
    std::string variableName;

    explicit InstructionStore(std::string variableName)
            : variableName(std::move(variableName)) {
    }

    void execute() final;
};

struct InstructionCopy : Instruction {
    std::string srcVariableName;
    std::string dstVariableName;

    InstructionCopy(std::string srcVariableName, std::string dstVariableName)
            : srcVariableName(std::move(srcVariableName)),
              dstVariableName(std::move(dstVariableName)) {
    }

    void execute() final;
};

struct InstructionPush : Instruction {
    void execute() final;
};

struct InstructionPop : Instruction {
    void execute() final;
};

struct InstructionLocalRead : Instruction {
    int variableOffset;

    explicit InstructionLocalRead(int variableOffset)
            : variableOffset(variableOffset) {
    }

    void execute() final;
};

struct InstructionLocalWrite : Instruction {
    int variableOffset;

    explicit InstructionLocalWrite(int variableOffset)
            : variableOffset(variableOffset) {
    }

    void execute() final;
};

struct InstructionBranch : Instruction {
    std::string labelName;

    explicit InstructionBranch(std::string labelName)
            : labelName(std::move(labelName)) {
    }

    void execute() final;
};

struct InstructionBranchIfNegative : Instruction {
    std::string labelName;

    explicit InstructionBranchIfNegative(std::string labelName)
            : labelName(std::move(labelName)) {
    }

    void execute() final;
};

struct InstructionBranchIfZeroOrNegative : Instruction {
    std::string labelName;

    explicit InstructionBranchIfZeroOrNegative(std::string labelName)
            : labelName(std::move(labelName)) {
    }

    void execute() final;
};

struct InstructionBranchIfPositive : Instruction {
    std::string labelName;

    explicit InstructionBranchIfPositive(std::string labelName)
            : labelName(std::move(labelName)) {
    }

    void execute() final;
};

struct InstructionBranchIfZeroOrPositive : Instruction {
    std::string labelName;

    explicit InstructionBranchIfZeroOrPositive(std::string labelName)
            : labelName(std::move(labelName)) {
    }

    void execute() final;
};

struct InstructionAddImmediate : Instruction {
    int value;

    explicit InstructionAddImmediate(int value)
            : value(value) {
    }

    void execute() final;
};

struct InstructionAddGlobal : Instruction {
    std::string variableName;

    explicit InstructionAddGlobal(std::string variableName)
            : variableName(std::move(variableName)) {
    }

    void execute() final;
};

struct InstructionSubtractImmediate : Instruction {
    int value;

    explicit InstructionSubtractImmediate(int value)
            : value(value) {
    }

    void execute() final;
};

struct InstructionSubtractGlobal : Instruction {
    std::string variableName;

    explicit InstructionSubtractGlobal(std::string variableName)
            : variableName(std::move(variableName)) {
    }

    void execute() final;
};

struct InstructionMultiplyImmediate : Instruction {
    int value;

    explicit InstructionMultiplyImmediate(int value)
            : value(value) {
    }

    void execute() final;
};

struct InstructionMultiplyGlobal : Instruction {
    std::string variableName;

    explicit InstructionMultiplyGlobal(std::string variableName)
            : variableName(std::move(variableName)) {
    }

    void execute() final;
};

struct InstructionDivideImmediate : Instruction {
    int value;

    explicit InstructionDivideImmediate(int value)
            : value(value) {
    }

    void execute() final;
};

struct InstructionDivideGlobal : Instruction {
    std::string variableName;

    explicit InstructionDivideGlobal(std::string variableName)
            : variableName(std::move(variableName)) {
    }

    void execute() final;
};

struct InstructionRead : Instruction {
    std::string variableName;

    explicit InstructionRead(std::string variableName)
            : variableName(std::move(variableName)) {
    }

    void execute() final;
};

struct InstructionWriteImmediate : Instruction {
    int value;

    explicit InstructionWriteImmediate(int value)
            : value(value) {
    }

    void execute() final;
};

struct InstructionWriteGlobal : Instruction {
    std::string variableName;

    explicit InstructionWriteGlobal(std::string variableName)
            : variableName(std::move(variableName)) {
    }

    void execute() final;
};

/**
 * The program source code.
 */
static std::string source;

/**
 * A single register used for computation.
 */
static int accumulator;

/**
 * The data stack used for anonymous local variables.
 */
static std::vector<int> locals;

/**
 * The data map used for named global variables.
 */
static std::map<std::string, int> globals;

/**
 * A map to keep track of label targets in the loaded program.
 */
static std::map<std::string, int> labels;

/**
 * The loaded program instructions.
 */
static std::vector<std::unique_ptr<Instruction>> program;

/**
 * The current instruction pointer.
 */
static decltype(program.begin()) instruction;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid arguments!\n";
        std::exit(1);
    }

    // Load the program source code
    {
        // Try to open the file
        std::ifstream ifs(argv[1]);
        if (!ifs) {
            std::cerr << "Failed to open file!\n";
            std::exit(1);
        }

        // Load the file content into memory
        std::stringstream ss;
        ss << ifs.rdbuf();
        source = ss.str();
    }

    // Preload with a no-op instruction as a hack
    program.push_back(std::make_unique<InstructionNoop>());

    // Parse the source code
    std::string line;
    bool stopped = false;
    for (char c: source) {
        if (c == '\n' || c == '\r') {
            static const char DIGITS[] = "0123456789";
            static const char WHITESPACE[] = " \n\r\t";

            // Trim whitespace from the line
            auto lineTrim1 = line.find_first_not_of(WHITESPACE);
            if (lineTrim1 == std::string::npos) {
                lineTrim1 = 0;
            }
            auto lineTrim2 = line.find_last_not_of(WHITESPACE);
            if (lineTrim2 == std::string::npos) {
                lineTrim2 = line.length() - 1;
            }
            line = line.substr(lineTrim1, lineTrim2 - lineTrim1 + 1);

            // If the line still has content
            if (!line.empty()) {
                // Treat the line like an input stream
                // This makes it easy to ignore whitespace and also to parse integers
                std::istringstream ss(line);

                // Read the first word from the line
                // This must be the name of a label, an instruction, or a global variable
                std::string name;
                ss >> name;

                // If we've stopped reading code
                if (stopped) {
                    // Initialize the name as a global variable
                    int value;
                    ss >> value;
                    globals[name] = value;
                } else {
                    // If it ends in a colon, it's declaring a label
                    if (name.find_first_of(':') == name.size() - 1) {
                        // Trim off the colon to get the bare label name
                        name = name.substr(0, name.size() - 1);

                        // Point the label at the current program index
                        labels[name] = (int) program.size() - 1;

                        // Read the next word from the line
                        // At this point, an instruction name must be loaded
                        ss >> name;
                    }

                    // Handle the instruction
                    if (name == "NOOP") {
                        // Instruction to do nothing
                        program.push_back(std::make_unique<InstructionNoop>());
                    } else if (name == "STOP") {
                        // Instruction to explicitly stop the program
                        program.push_back(std::make_unique<InstructionStop>());

                        // The stop instruction also serves the syntactical purpose of marking the end of the code
                        stopped = true;
                    } else if (name == "LOAD") {
                        // Read the argument
                        std::string arg;
                        ss >> arg;

                        // If the argument only contains digits
                        if (arg.find_first_not_of(DIGITS) == std::string::npos) {
                            // Instruction to load an immediate value
                            program.push_back(std::make_unique<InstructionLoadImmediate>(std::atoi(arg.c_str())));
                        } else {
                            // Instruction to load a global variable
                            program.push_back(std::make_unique<InstructionLoadGlobal>(arg));
                        }
                    } else if (name == "STORE") {
                        // Read the argument
                        std::string arg;
                        ss >> arg;

                        // Instruction to store a global variable
                        program.push_back(std::make_unique<InstructionStore>(arg));
                    } else if (name == "COPY") {
                        // Read the arguments
                        std::string arg1;
                        std::string arg2;
                        ss >> arg1 >> arg2;

                        // Instruction to copy a global variable into another global variable
                        program.push_back(std::make_unique<InstructionCopy>(arg1, arg2));
                    } else if (name == "PUSH") {
                        // Instruction to push a zero onto the stack
                        program.push_back(std::make_unique<InstructionPush>());
                    } else if (name == "POP") {
                        // Instruction to delete the top value off of the stack
                        program.push_back(std::make_unique<InstructionPop>());
                    } else if (name == "STACKR") {
                        int offset;
                        ss >> offset;

                        // Instruction to read from a variable on the stack at an offset
                        program.push_back(std::make_unique<InstructionLocalRead>(offset));
                    } else if (name == "STACKW") {
                        int offset;
                        ss >> offset;

                        // Instruction to write to a variable on the stack at an offset
                        program.push_back(std::make_unique<InstructionLocalWrite>(offset));
                    } else if (name == "BR") {
                        // Read the argument
                        std::string arg;
                        ss >> arg;

                        // Instruction to jump to a label
                        program.push_back(std::make_unique<InstructionBranch>(arg));
                    } else if (name == "BRNEG") {
                        // Read the argument
                        std::string arg;
                        ss >> arg;

                        // Instruction to jump to a label if the accumulator is negative
                        program.push_back(std::make_unique<InstructionBranchIfNegative>(arg));
                    } else if (name == "BRZNEG") {
                        // Read the argument
                        std::string arg;
                        ss >> arg;

                        // Instruction to jump to a label if the accumulator is zero or negative
                        program.push_back(std::make_unique<InstructionBranchIfZeroOrNegative>(arg));
                    } else if (name == "BRPOS") {
                        // Read the argument
                        std::string arg;
                        ss >> arg;

                        // Instruction to jump to a label if the accumulator is positive
                        program.push_back(std::make_unique<InstructionBranchIfPositive>(arg));
                    } else if (name == "BRZPOS") {
                        // Read the argument
                        std::string arg;
                        ss >> arg;

                        // Instruction to jump to a label if the accumulator is zero or positive
                        program.push_back(std::make_unique<InstructionBranchIfZeroOrPositive>(arg));
                    } else if (name == "ADD") {
                        // Read the argument
                        std::string arg;
                        ss >> arg;

                        // If the argument only contains digits
                        if (arg.find_first_not_of(DIGITS) == std::string::npos) {
                            // Instruction to add an immediate value
                            program.push_back(std::make_unique<InstructionAddImmediate>(std::atoi(arg.c_str())));
                        } else {
                            // Instruction to add a global variable
                            program.push_back(std::make_unique<InstructionAddGlobal>(arg));
                        }
                    } else if (name == "SUB") {
                        // Read the argument
                        std::string arg;
                        ss >> arg;

                        // If the argument only contains digits
                        if (arg.find_first_not_of(DIGITS) == std::string::npos) {
                            // Instruction to subtract an immediate value
                            program.push_back(std::make_unique<InstructionSubtractImmediate>(std::atoi(arg.c_str())));
                        } else {
                            // Instruction to subtract a global variable
                            program.push_back(std::make_unique<InstructionSubtractGlobal>(arg));
                        }
                    } else if (name == "MUL") {
                        // Read the argument
                        std::string arg;
                        ss >> arg;

                        // If the argument only contains digits
                        if (arg.find_first_not_of(DIGITS) == std::string::npos) {
                            // Instruction to multiply an immediate value
                            program.push_back(std::make_unique<InstructionMultiplyImmediate>(std::atoi(arg.c_str())));
                        } else {
                            // Instruction to multiply a global variable
                            program.push_back(std::make_unique<InstructionMultiplyGlobal>(arg));
                        }
                    } else if (name == "DIV") {
                        // Read the argument
                        std::string arg;
                        ss >> arg;

                        // If the argument only contains digits
                        if (arg.find_first_not_of(DIGITS) == std::string::npos) {
                            // Instruction to divide an immediate value
                            program.push_back(std::make_unique<InstructionDivideImmediate>(std::atoi(arg.c_str())));
                        } else {
                            // Instruction to divide a global variable
                            program.push_back(std::make_unique<InstructionDivideGlobal>(arg));
                        }
                    } else if (name == "READ") {
                        std::string destination;
                        ss >> destination;

                        // Instruction to prompt a value from the user
                        program.push_back(std::make_unique<InstructionRead>(destination));
                    } else if (name == "WRITE") {
                        // Read the argument
                        std::string arg;
                        ss >> arg;

                        // If the argument only contains digits
                        if (arg.find_first_not_of(DIGITS) == std::string::npos) {
                            // Instruction to print an immediate value to the user
                            program.push_back(std::make_unique<InstructionWriteImmediate>(std::atoi(arg.c_str())));
                        } else {
                            // Instruction to print a global variable to the user
                            program.push_back(std::make_unique<InstructionWriteGlobal>(arg));
                        }
                    } else {
                        std::cerr << "Unknown instruction: " << name << "\n";
                        std::exit(1);
                    }
                }
            }

            // Clear the line
            line.clear();
        } else {
            // Add to the current line
            line += c;
        }
    }

    // Load the first instruction
    instruction = program.begin();

    // While the program hasn't ended
    while (instruction != program.end()) {
        // Advance to the next instruction
        ++instruction;

        // Execute the instruction
        instruction->get()->execute();
    }

    // Return whatever remains in the accumulator register
    return accumulator;
}

void InstructionNoop::execute() {
}

void InstructionStop::execute() {
    instruction = program.end();
}

void InstructionLoadImmediate::execute() {
    accumulator = value;
}

void InstructionLoadGlobal::execute() {
    auto iterator = globals.find(variableName);
    if (iterator == globals.end()) {
        std::cerr << "Tried to load nonexistent variable: " << variableName << "\n";
        std::exit(1);
    }
    accumulator = iterator->second;
}

void InstructionStore::execute() {
    auto iterator = globals.find(variableName);
    if (iterator == globals.end()) {
        std::cerr << "Tried to store nonexistent variable: " << variableName << "\n";
        std::exit(1);
    }
    iterator->second = accumulator;
}

void InstructionCopy::execute() {
    auto iterator1 = globals.find(srcVariableName);
    if (iterator1 == globals.end()) {
        std::cerr << "Tried to copy from nonexistent variable: " << srcVariableName << "\n";
        std::exit(1);
    }
    auto iterator2 = globals.find(dstVariableName);
    if (iterator2 == globals.end()) {
        std::cerr << "Tried to copy to nonexistent variable: " << dstVariableName << "\n";
        std::exit(1);
    }
    iterator1->second = iterator2->second;
}

void InstructionLocalRead::execute() {
    auto index = locals.size() - variableOffset - 1;
    if (index >= locals.size()) {
        std::cerr << "Out of bounds local read: " << index << "\n";
        std::exit(1);
    }
    accumulator = locals[index];
}

void InstructionLocalWrite::execute() {
    auto index = locals.size() - variableOffset - 1;
    if (index >= locals.size()) {
        std::cerr << "Out of bounds local write: " << index << "\n";
        std::exit(1);
    }
    locals[index] = accumulator;
}

void InstructionPush::execute() {
    locals.push_back(0);
}

void InstructionPop::execute() {
    locals.pop_back();
}

void InstructionBranch::execute() {
    auto iterator = labels.find(labelName);
    if (iterator == labels.end()) {
        std::cerr << "Tried to jump to nonexistent label: " << labelName << "\n";
        std::exit(1);
    }
    instruction = program.begin() + iterator->second;
}

void InstructionBranchIfNegative::execute() {
    if (accumulator < 0) {
        auto iterator = labels.find(labelName);
        if (iterator == labels.end()) {
            std::cerr << "Tried to jump to nonexistent label: " << labelName << "\n";
            std::exit(1);
        }
        instruction = program.begin() + iterator->second;
    }
}

void InstructionBranchIfZeroOrNegative::execute() {
    if (accumulator <= 0) {
        auto iterator = labels.find(labelName);
        if (iterator == labels.end()) {
            std::cerr << "Tried to jump to nonexistent label: " << labelName << "\n";
            std::exit(1);
        }
        instruction = program.begin() + iterator->second;
    }
}

void InstructionBranchIfPositive::execute() {
    if (accumulator > 0) {
        auto iterator = labels.find(labelName);
        if (iterator == labels.end()) {
            std::cerr << "Tried to jump to nonexistent label: " << labelName << "\n";
            std::exit(1);
        }
        instruction = program.begin() + iterator->second;
    }
}

void InstructionBranchIfZeroOrPositive::execute() {
    if (accumulator >= 0) {
        auto iterator = labels.find(labelName);
        if (iterator == labels.end()) {
            std::cerr << "Tried to jump to nonexistent label: " << labelName << "\n";
            std::exit(1);
        }
        instruction = program.begin() + iterator->second;
    }
}

void InstructionAddImmediate::execute() {
    accumulator += value;
}

void InstructionAddGlobal::execute() {
    auto iterator = globals.find(variableName);
    if (iterator == globals.end()) {
        std::cerr << "Tried to add by nonexistent variable: " << variableName << "\n";
        std::exit(1);
    }
    accumulator += iterator->second;
}

void InstructionSubtractImmediate::execute() {
    accumulator -= value;
}

void InstructionSubtractGlobal::execute() {
    auto iterator = globals.find(variableName);
    if (iterator == globals.end()) {
        std::cerr << "Tried to subtract by nonexistent variable: " << variableName << "\n";
        std::exit(1);
    }
    accumulator -= iterator->second;
}

void InstructionMultiplyImmediate::execute() {
    accumulator *= value;
}

void InstructionMultiplyGlobal::execute() {
    auto iterator = globals.find(variableName);
    if (iterator == globals.end()) {
        std::cerr << "Tried to multiply by nonexistent variable: " << variableName << "\n";
        std::exit(1);
    }
    accumulator *= iterator->second;
}

void InstructionDivideImmediate::execute() {
    accumulator /= value;
}

void InstructionDivideGlobal::execute() {
    auto iterator = globals.find(variableName);
    if (iterator == globals.end()) {
        std::cerr << "Tried to divide by nonexistent variable: " << variableName << "\n";
        std::exit(1);
    }
    accumulator /= iterator->second;
}

void InstructionRead::execute() {
    auto iterator = globals.find(variableName);
    if (iterator == globals.end()) {
        std::cerr << "Tried to read into nonexistent variable: " << variableName << "\n";
        std::exit(1);
    }

    int value;
    std::cout << "Input: ";
    std::cin >> value;

    iterator->second = value;
}

void InstructionWriteImmediate::execute() {
    std::cout << "Output: " << value << "\n";
}

void InstructionWriteGlobal::execute() {
    auto iterator = globals.find(variableName);
    if (iterator == globals.end()) {
        std::cerr << "Tried to print nonexistent variable: " << variableName << "\n";
        std::exit(1);
    }
    std::cout << "Output: " << iterator->second << "\n";
}
