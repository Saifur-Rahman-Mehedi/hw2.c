#include "gtest/gtest.h"
#include "tests_aux.h"

extern char pargs[300];

class copy_paste_valgrind_sf_TestSuite : public testing::Test { 
	void SetUp() override {
        system("rm -rf ./tests/actual_outputs");
        system("rm -f /tmp/exp_single_line.txt and /tmp/act_single_line.txt");
		mkdir("./tests/actual_outputs/", 0700);
	}
};

TEST_F(copy_paste_valgrind_sf_TestSuite, copy_paste_cactus_sbu) {
    const char *input_file = "./tests/images/desert.sbu";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(pargs, "-i %s -p 90,60 -c 90,10,50,100 -o %s", input_file, actual_output_file);
    expect_no_valgrind_errors(run_using_valgrind(pargs));	
}

TEST_F(copy_paste_valgrind_sf_TestSuite, copy_paste_stony2_1) {
    const char *input_file = "./tests/images/stony.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(pargs, "-o %s -c 170,100,90,180 -p 0,0 -i %s", actual_output_file, input_file);
    expect_no_valgrind_errors(run_using_valgrind(pargs));
}

TEST_F(copy_paste_valgrind_sf_TestSuite, overflow_corner) {
    const char *input_file = "./tests/images/seawolf.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(pargs, "-o %s -c 36,60,50,50 -p 50,80 -i %s", actual_output_file, input_file);
    expect_no_valgrind_errors(run_using_valgrind(pargs));
}

TEST_F(copy_paste_valgrind_sf_TestSuite, overlapping1) {
    const char *input_file = "./tests/images/stony.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(pargs, "-i %s -p 22,32 -o %s -c 20,30,120,140", input_file, actual_output_file);
    expect_no_valgrind_errors(run_using_valgrind(pargs));
}