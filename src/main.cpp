#include "command_line_parser.hpp"
#include "path_manager.hpp"
#include "binary_io.hpp"
#include "serializer.hpp"
#include "deserializer.hpp"
#include <cstdlib>

int main(int argc, char** argv) {
    using namespace transplant;

    CommandLineParser parser;
    ProgramOptions options;

    int ret = parser.parse(argc, argv, options);

    if (ret) {
        CommandLineParser::print_usage(*argv, EXIT_FAILURE);
    }

    if (options.is_help()) {
        CommandLineParser::print_usage(*argv, EXIT_SUCCESS);
    }

    PathManager path_manager;
    if (path_manager.init(options.path)) {
        return EXIT_FAILURE;
    }

    BinaryIO io;

    if (options.is_serialize()) {
        Serializer serializer(path_manager, io);
        if (serializer.serialize()) {
            return EXIT_FAILURE;
        }
    } else if (options.is_deserialize()) {
        Deserializer deserializer(path_manager, io, options.is_clobber());
        if (deserializer.deserialize()) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
