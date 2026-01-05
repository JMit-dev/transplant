#ifndef COMMAND_LINE_PARSER_HPP
#define COMMAND_LINE_PARSER_HPP

namespace transplant {

struct ProgramOptions {
    int flags = 0;
    const char* path = nullptr;

    bool is_help() const { return flags & 0x1; }
    bool is_serialize() const { return flags & 0x2; }
    bool is_deserialize() const { return flags & 0x4; }
    bool is_clobber() const { return flags & 0x8; }
};

class CommandLineParser {
public:
    CommandLineParser() = default;
    ~CommandLineParser() = default;

    // Parse and validate command line arguments
    int parse(int argc, char** argv, ProgramOptions& options);

    // Display usage message
    static void print_usage(const char* program_name, int exit_code);
};

} // namespace transplant

#endif
