#ifndef TEST_ADAPTER_HPP
#define TEST_ADAPTER_HPP

// This header provides C-compatible interfaces for the tests
#include "path_manager.hpp"
#include "binary_io.hpp"
#include "command_line_parser.hpp"
#include <cstdio>

// Global instances for test compatibility
extern transplant::PathManager* g_test_path_manager;
extern transplant::BinaryIO* g_test_io;
extern transplant::ProgramOptions g_test_options;

// C-compatible wrapper functions
extern "C" {

int path_init(char* name);
int path_push(char* name);
int path_pop();

int validargs(int argc, char** argv);

void write_u32_be(uint32_t value, std::FILE* out);
void write_u64_be(uint64_t value, std::FILE* out);
uint32_t read_u32_be(std::FILE* in);
uint64_t read_u64_be(std::FILE* in);
void write_header(std::FILE* out, uint8_t type, uint32_t depth, uint64_t size);
int read_header(std::FILE* in, transplant::RecordHeader* header);
int io_error();

}

// Global variables for test compatibility
extern int g_global_options;
extern char g_name_buf[NAME_MAX];
extern char g_path_buf[PATH_MAX];
extern int g_path_length;

#endif
