#include <sys/stat.h>
#include "test_common.hpp"

char *test_output_dir = nullptr;
char *test_outfile = nullptr;
char *test_errfile = nullptr;
char *alt_outfile = nullptr;

char *ref_dir = nullptr;
char *ref_infile = nullptr;
char *ref_outfile = nullptr;
char *ref_errfile = nullptr;

int setup_test(char *name)
{
    std::FILE *f;
    size_t s;
    char *cmd = nullptr;

    NEWSTREAM(f, s, test_output_dir);
    std::fprintf(f, "%s/%s", TEST_OUTPUT_DIR, name);
    std::fclose(f);

    NEWSTREAM(f, s, test_outfile);
    std::fprintf(f, "%s/test.out", test_output_dir);
    std::fclose(f);

    NEWSTREAM(f, s, test_errfile);
    std::fprintf(f, "%s/test.err", test_output_dir);
    std::fclose(f);

    NEWSTREAM(f, s, alt_outfile);
    std::fprintf(f, "%s/test.alt", test_output_dir);
    std::fclose(f);

    NEWSTREAM(f, s, ref_dir);
    std::fprintf(f, "%s/%s", TEST_RSRC_DIR, name);
    std::fclose(f);

    NEWSTREAM(f, s, ref_infile);
    std::fprintf(f, "%s/ref.in", ref_dir);
    std::fclose(f);

    NEWSTREAM(f, s, ref_outfile);
    std::fprintf(f, "%s/ref.out", ref_dir);
    std::fclose(f);

    NEWSTREAM(f, s, ref_errfile);
    std::fprintf(f, "%s/ref.err", ref_dir);
    std::fclose(f);

    NEWSTREAM(f, s, cmd);
    f = open_memstream(&cmd, &s);
    std::fprintf(f, "chmod -fR u+w %s; rm -fr %s; mkdir -p %s",
            test_output_dir, test_output_dir, test_output_dir);
    std::fclose(f);

    int ret = std::system(cmd);
    free(cmd);
    return ret;
}

int run_using_system(char *program_path, char *pre_cmd, char *valgrind_cmd,
                     char *program_options, char *limits)
{
    std::FILE *f;
    size_t s;
    char *cmd = nullptr;
    struct stat sbuf;

    int staterr = stat(ref_infile, &sbuf);
    NEWSTREAM(f, s, cmd);
    std::fprintf(f, "%s%s%s %s %s < %s > %s 2> %s",
            limits, pre_cmd, valgrind_cmd,
            program_path, program_options,
            staterr ? "/dev/null" : ref_infile,
            test_outfile, test_errfile);
    std::fclose(f);
    std::fprintf(stderr, "run(%s)\n", cmd);
    int ret = std::system(cmd);
    free(cmd);
    return ret;
}

void assert_normal_exit(int status)
{
    cr_assert(!(status & 0x8000),
              "The program terminated with an unexpected signal (%d).\n",
              (status & 0x7f00) >> 8);
}

void assert_expected_status(int expected, int status)
{
    cr_assert(!(status & 0x8000),
              "The program terminated with an unexpected signal (%d).\n",
              (status & 0x7f00) >> 8);
    cr_assert_eq(
        WEXITSTATUS(status), expected,
        "The program did not exit with the expected status "
        "(expected 0x%x, was 0x%x).\n",
        expected, WEXITSTATUS(status));
}

void assert_signaled(int sig, int status)
{
    cr_assert(status & 0x8000,
              "The program did not terminate with a signal.\n");
    cr_assert((status & 0x7f00) >> 8 == sig,
              "The program did not terminate with the expected signal "
              "(expected %d, was %d).\n",
              sig, WTERMSIG(status));
}

void assert_no_valgrind_errors(int status)
{
    cr_assert_neq(WEXITSTATUS(status), 37,
                  "Valgrind reported errors -- see %s.err",
                  test_outfile);
}

void assert_files_match(char *ref, char *test, char *filter)
{
    std::FILE *f;
    size_t s;
    char *cmd = nullptr;

    NEWSTREAM(f, s, cmd);
    if (filter) {
        std::fprintf(f,
                "grep -v '%s' %s > %s.flt && "
                "grep -v '%s' %s > %s.flt && "
                "diff --ignore-tab-expansion --ignore-trailing-space "
                "--ignore-space-change --ignore-blank-lines "
                "%s.flt %s.flt",
                filter, test, test,
                filter, ref, ref,
                ref, test);
    } else {
        std::fprintf(f,
                "diff --ignore-tab-expansion --ignore-trailing-space "
                "--ignore-space-change --ignore-blank-lines "
                "%s %s",
                ref, test);
    }
    std::fclose(f);
    std::fprintf(stderr, "run(%s)\n", cmd);
    int err = std::system(cmd);
    free(cmd);
    cr_assert_eq(err, 0,
                 "The output was not what was expected (diff exited with "
                 "status %d).\n",
                 WEXITSTATUS(err));
}

void assert_binaries_match(char *ref, char *test)
{
    std::FILE *f;
    size_t s;
    char *cmd = nullptr;

    NEWSTREAM(f, s, cmd);
    std::fprintf(f, "cmp %s %s", ref, test);
    std::fclose(f);
    int err = std::system(cmd);
    free(cmd);
    cr_assert_eq(err, 0,
                 "The output was not what was expected (cmp exited with "
                 "status %d).\n",
                 WEXITSTATUS(err));
}

void assert_dirs_match(char *ref, char *test)
{
    std::FILE *f;
    size_t s;
    char *cmd = nullptr;

    NEWSTREAM(f, s, cmd);
    std::fprintf(f,
        "diff --recursive --ignore-tab-expansion --ignore-trailing-space "
        "--ignore-space-change --ignore-blank-lines "
        "%s %s",
        ref, test);
    std::fclose(f);
    std::fprintf(stderr, "run(%s)\n", cmd);
    int err = std::system(cmd);
    free(cmd);
    cr_assert_eq(err, 0,
                 "The output was not what was expected (diff exited with "
                 "status %d).\n",
                 WEXITSTATUS(err));
}
