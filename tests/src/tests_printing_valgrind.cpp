#include "gtest/gtest.h"
#include "tests_aux.h"

extern char pargs[300];

class printing_valgrind_sf_TestSuite : public testing::Test { 
	void SetUp() override {
        system("rm -rf ./tests/actual_outputs");
        system("rm -f /tmp/exp_single_line.txt and /tmp/act_single_line.txt");
		mkdir("./tests/actual_outputs/", 0700);
	}
};

TEST_F(printing_valgrind_sf_TestSuite, print_short_message1_2) {
    const char *input_file = "./tests/images/desert.sbu";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(pargs, "-i %s -r \"seawolves\",\"./tests/fonts/font1.txt\",1,100,150 -o %s", input_file, actual_output_file);
    expect_no_valgrind_errors(run_using_valgrind(pargs));
}

TEST_F(printing_valgrind_sf_TestSuite, print_overflow_message1_2) {
    const char *input_file = "./tests/images/desert.sbu";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(pargs, "-i %s -o %s -r \"new YORK state\",\"./tests/fonts/font3.txt\",1,10,220", input_file, actual_output_file);
    expect_no_valgrind_errors(run_using_valgrind(pargs));
}
