#include "gtest/gtest.h"
#include "tests_aux.h"

extern char pargs[300];

class load_save_valgrind_sf_TestSuite : public testing::Test { 
	void SetUp() override {
        system("rm -rf ./tests/actual_outputs");
        system("rm -f /tmp/exp_single_line.txt and /tmp/act_single_line.txt");
		mkdir("./tests/actual_outputs/", 0700);
	}
};

TEST_F(load_save_valgrind_sf_TestSuite, load_ppm_save_ppm) {
    const char *input_file = "./tests/images/seawolf.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(pargs, "-i %s -o %s", input_file, actual_output_file);
	expect_no_valgrind_errors(run_using_valgrind(pargs));	
}

TEST_F(load_save_valgrind_sf_TestSuite, load_sbu_save_ppm) {
    const char *input_file = "./tests/images/desert.sbu";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(pargs, "-o %s -i %s", actual_output_file, input_file);
	expect_no_valgrind_errors(run_using_valgrind(pargs));	
}