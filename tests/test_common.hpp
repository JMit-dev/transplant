#ifndef TEST_COMMON_HPP
#define TEST_COMMON_HPP

#include <cstdint>
#include <criterion/criterion.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

constexpr int TEST_TIMEOUT = 15;

constexpr const char* TEST_RSRC_DIR = "tests/rsrc";
constexpr const char* TEST_OUTPUT_DIR = "test_output";
constexpr const char* REF_BIN_DIR = "tests/rsrc/bin";
constexpr const char* STANDARD_LIMITS = "ulimit -t 10; ulimit -f 2000;";

#define QUOTE1(x) #x
#define QUOTE(x) QUOTE1(x)

#define NEWSTREAM(f, s, v) \
do { \
    if((v) != nullptr) \
        free(v); \
    (f) = open_memstream(&(v), &(s)); \
} while(0)

#define ASSERT_RETURN(exp_ret) do { \
    cr_assert_eq(ret, exp_ret, \
             "Wrong return value from function: expected %d, was %d\n", \
                 exp_ret, ret); \
  } while(0)

#define REDIRECT_STDOUT do { \
    std::FILE *ret = std::freopen(test_outfile, "w", stdout); \
    cr_assert(ret != nullptr, "Failed to redirect stdout to '%s'", test_outfile); \
  } while(0)

#define REDIRECT_STDERR do { \
    std::FILE *ret = std::freopen(test_errfile, "w", stderr); \
    cr_assert(ret != nullptr, "Failed to redirect stderr to '%s'", test_errfile); \
  } while(0)

#define REDIRECT_STDIN do { \
    std::FILE *ret = std::freopen(ref_infile, "r", stdin); \
    cr_assert(ret != nullptr, "Failed to redirect stdin from '%s'", ref_infile); \
  } while(0)

#define CREATE_DIRECTORY(name) do { \
    int ret = mkdir(name, 0777); \
    if(ret == -1) \
    cr_assert_fail("Unable to create directory: '%s'", name); \
  } while(0)

extern char *test_output_dir;
extern char *test_outfile;
extern char *test_errfile;
extern char *alt_outfile;

extern char *ref_dir;
extern char *ref_infile;
extern char *ref_outfile;
extern char *ref_errfile;

int setup_test(char *name);
int run_using_system(char *program_path, char *pre_cmd, char *valgrind_cmd,
                     char *program_options, char *limits);

void assert_normal_exit(int status);
void assert_error_exit(int status);
void assert_expected_status(int expected, int status);
void assert_signaled(int sig, int status);
void assert_no_valgrind_errors(int status);

void assert_files_match(char *ref, char *test, char *filter);
void assert_binaries_match(char *ref, char *test);
void assert_dirs_match(char *ref, char *test);

#endif
