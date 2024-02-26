#include "tests_aux.h"

char cmd[300];
char pargs[300];

bool file_exists(const char *path) {
    return (access(path, F_OK) == 0);
}

void expect_no_valgrind_errors(int status) {
    EXPECT_NE(37, WEXITSTATUS(status));
}

int run_using_valgrind(const char *args) {
    sprintf(cmd, "valgrind --quiet -s --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --error-exitcode=37 ./build/hw2_main %s", args);
	return system(cmd);
}

int run_using_system(const char *args) {
    assert(file_exists("./build/hw2_main"));
    (void)sprintf(cmd, "./build/hw2_main %s", args);
	return system(cmd);
}

void check_image_file_contents(const char *expected_file, const char *actual_file) {
    assert(file_exists(expected_file));
    if (file_exists(actual_file)) {
        system("/usr/bin/bash -c \"rm -f /tmp/exp.txt /tmp/exp_single_line.txt /tmp/act.txt /tmp/act_single_line.txt\"");
        sprintf(cmd, "cp %s /tmp/exp.txt; cp %s /tmp/act.txt", expected_file, actual_file);
        system(cmd);
        system("/usr/bin/bash -c \"chmod 700 ./tests/src/cleanup.sh; ./tests/src/cleanup.sh\"");
        sprintf(cmd, "/usr/bin/bash -c \"diff -q /tmp/exp_single_line.txt /tmp/act_single_line.txt\"");
        int status = system(cmd);
        if (WEXITSTATUS(status) != 0) {
            INFO("Expected image and actual image differ. Insert a breakpoint and review the files named");
            INFO("/tmp/exp_single_line.txt and /tmp/act_single_line.txt");
        }
        EXPECT_EQ(0, WEXITSTATUS(status)); 
    }
    else {
        INFO("Output file does not exist.");
        FAIL();
    }
}