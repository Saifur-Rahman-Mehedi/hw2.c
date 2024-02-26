#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string>
#include "gtest/gtest.h"
#include "tests_aux.h"
#include "hw2.h"

using namespace std;

class image_operations_TestSuite : public testing::Test { 
	void SetUp() override {
        system("rm -rf ./tests/actual_outputs");
        system("rm -f /tmp/exp_single_line.txt and /tmp/act_single_line.txt");
		mkdir("./tests/actual_outputs/", 0700);
	}
};

extern char cmd[300];

// Load PPM image and copy it to PPM
TEST_F(image_operations_TestSuite, load_ppm_save_ppm) {
    const char *input_file = "./tests/images/seawolf.ppm";
    const char *expected_output_file = "./tests/images/seawolf.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -i %s -o %s", input_file, actual_output_file);
    INFO(cmd);
	int status = run_using_system(cmd);
	EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// Load PPM image and convert it to SBU
TEST_F(image_operations_TestSuite, load_ppm_save_sbu) {
    const char *input_file = "./tests/images/seawolf.ppm";
    const char *expected_output_file = "./tests/images/seawolf.sbu";
    const char *actual_output_file = "./tests/actual_outputs/result.sbu";
    sprintf(cmd, "./build/hw2_main -o %s -i %s", actual_output_file, input_file);
    INFO(cmd);
	int status = run_using_system(cmd);
	EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// Load SBU image and copy it to SBU
TEST_F(image_operations_TestSuite, load_sbu_save_sbu) {
    const char *input_file = "./tests/images/desert.sbu";
    const char *expected_output_file = "./tests/images/desert.sbu";
    const char *actual_output_file = "./tests/actual_outputs/result.sbu";
    sprintf(cmd, "./build/hw2_main -i %s -o %s", input_file, actual_output_file);
    INFO(cmd);
	int status = run_using_system(cmd);
	EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// Load SBU image and convert it to PPM
TEST_F(image_operations_TestSuite, load_sbu_save_ppm) {
    const char *input_file = "./tests/images/desert.sbu";
    const char *expected_output_file = "./tests/images/desert.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -o %s -i %s", actual_output_file, input_file);
    INFO(cmd);
	int status = run_using_system(cmd);
	EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}
