#include "test_adapter.hpp"
#include <cstring>

// Global instances
transplant::PathManager* g_test_path_manager = nullptr;
transplant::BinaryIO* g_test_io = nullptr;
transplant::ProgramOptions g_test_options;

// Global variables for test compatibility
int g_global_options = 0;
char g_name_buf[NAME_MAX] = {0};
char g_path_buf[PATH_MAX] = {0};
int g_path_length = 0;

// Initialize global instances if needed
static void ensure_globals() {
    if (!g_test_path_manager) {
        g_test_path_manager = new transplant::PathManager();
    }
    if (!g_test_io) {
        g_test_io = new transplant::BinaryIO();
    }
}

extern "C" {

int path_init(char* name) {
    ensure_globals();
    int result = g_test_path_manager->init(name);
    // Sync with global variables
    g_path_length = g_test_path_manager->get_length();
    std::strcpy(g_path_buf, g_test_path_manager->get_path());
    return result;
}

int path_push(char* name) {
    ensure_globals();
    int result = g_test_path_manager->push(name);
    // Sync with global variables
    g_path_length = g_test_path_manager->get_length();
    std::strcpy(g_path_buf, g_test_path_manager->get_path());
    return result;
}

int path_pop() {
    ensure_globals();
    int result = g_test_path_manager->pop();
    // Sync with global variables
    g_path_length = g_test_path_manager->get_length();
    std::strcpy(g_path_buf, g_test_path_manager->get_path());
    return result;
}

int validargs(int argc, char** argv) {
    ensure_globals();
    transplant::CommandLineParser parser;
    int result = parser.parse(argc, argv, g_test_options);
    g_global_options = g_test_options.flags;

    if (result == 0 && g_test_options.path) {
        g_test_path_manager->init(g_test_options.path);
        g_path_length = g_test_path_manager->get_length();
        std::strcpy(g_path_buf, g_test_path_manager->get_path());
    }

    return result;
}

void write_u32_be(uint32_t value, std::FILE* out) {
    ensure_globals();
    g_test_io->reset_error();
    g_test_io->write_u32_be(value, out);
}

void write_u64_be(uint64_t value, std::FILE* out) {
    ensure_globals();
    g_test_io->reset_error();
    g_test_io->write_u64_be(value, out);
}

uint32_t read_u32_be(std::FILE* in) {
    ensure_globals();
    g_test_io->reset_error();
    return g_test_io->read_u32_be(in);
}

uint64_t read_u64_be(std::FILE* in) {
    ensure_globals();
    g_test_io->reset_error();
    return g_test_io->read_u64_be(in);
}

void write_header(std::FILE* out, uint8_t type, uint32_t depth, uint64_t size) {
    ensure_globals();
    g_test_io->reset_error();
    g_test_io->write_header(out, type, depth, size);
}

int read_header(std::FILE* in, transplant::RecordHeader* header) {
    ensure_globals();
    g_test_io->reset_error();
    return g_test_io->read_header(in, header);
}

int io_error() {
    ensure_globals();
    return g_test_io->has_error() ? 1 : 0;
}

}
