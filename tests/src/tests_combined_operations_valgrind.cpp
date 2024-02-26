#include "gtest/gtest.h"
#include "tests_aux.h"

extern char pargs[300];

class combined_valgrind_sf_TestSuite : public testing::Test { 
	void SetUp() override {
        system("rm -rf ./tests/actual_outputs");
        system("rm -f /tmp/exp_single_line.txt and /tmp/act_single_line.txt");
		mkdir("./tests/actual_outputs/", 0700);
	}
};

TEST_F(combined_valgrind_sf_TestSuite, combined1) {
    const char *input_file = "./tests/images/stony.sbu";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(pargs, "-c 125,130,150,40 -p 85,130 -i %s -o %s -r \"Go STONY BROOK\",\"./tests/fonts/font1.txt\",2,50,5", input_file, actual_output_file);
    expect_no_valgrind_errors(run_using_valgrind(pargs));	
}

TEST_F(combined_valgrind_sf_TestSuite, combined2) {
    const char *input_file = "./tests/images/stony.sbu";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(pargs, "-c 125,130,150,40 -i %s -p 85,130 -o %s -r \"Go STONY BROOK\",\"./tests/fonts/font4.txt\",2,100,10", input_file, actual_output_file);
    expect_no_valgrind_errors(run_using_valgrind(pargs));	
}