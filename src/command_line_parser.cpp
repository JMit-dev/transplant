#include "command_line_parser.hpp"
#include "path_manager.hpp"
#include <cstdio>
#include <cstdlib>

namespace transplant {

void CommandLineParser::print_usage(const char* program_name, int exit_code) {
    std::fprintf(stderr, "USAGE: %s %s\n", program_name,
        "[-h] -s|-d [-c] [-p DIR]\n"
        "   -h       Help: displays this help menu.\n"
        "   -s       Serialize: traverse tree of files, output serialized data.\n"
        "   -d       Deserialize: read serialized data, reconstruct tree of files.\n"
        "            Optional additional parameter for both -s and -d:\n"
        "               -p DIR       DIR is a pathname that specifies the source directory\n"
        "                            for serialization or the target directory for deserialization.\n"
        "                            If this parameter is not present, the pathname `.`\n"
        "                            (referring to the current working directory) is assumed.\n"
        "            Optional additional parameter for -d:\n"
        "               -c           ``clobber'': the program will overwrite existing files,\n"
        "                            rather than terminating with an error, and it will ignore\n"
        "                            errors that result when attempts is made to create directories\n"
        "                            that already exist.\n");
    std::exit(exit_code);
}

int CommandLineParser::parse(int argc, char** argv, ProgramOptions& options) {
    options.flags = 0;
    options.path = nullptr;

    if (argc < 2) {
        return -1;
    }

    char** args = argv + 1;

    if (**args != '-') {
        return -1;
    }

    if (*(*args + 1) == 'h') {
        options.flags |= 0x1;
        return 0;
    }

    bool d_flag = false;

    if (*(*args + 1) == 's') {
        options.flags |= 0x2;
    } else if (*(*args + 1) == 'd') {
        options.flags |= 0x4;
        d_flag = true;
    } else {
        return -1;
    }

    bool c_flag = false;
    bool p_flag = false;

    args++;
    while (*args) {
        if (**args != '-') {
            return -1;
        }

        if (*(*args + 1) == 'c' && !c_flag) {
            c_flag = true;
            if (!d_flag) {
                return -1;
            }
            options.flags |= 0x8;
        } else if (*(*args + 1) == 'p' && !p_flag) {
            p_flag = true;
            args++;
            if (!*args || **args == '-') {
                return -1;
            }
            options.path = *args;
        } else {
            return -1;
        }
        args++;
    }

    if (!p_flag) {
        options.path = ".";
    }

    return 0;
}

} // namespace transplant
