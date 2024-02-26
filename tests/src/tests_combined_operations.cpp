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

// Copy & paste operations plus text rendering
// Copied region does not overlap the text.
TEST_F(image_operations_TestSuite, combined1) {
    const char *input_file = "./tests/images/stony.sbu";
    const char *expected_output_file = "./tests/expected_outputs/combined1.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -c 125,130,150,40 -p 85,130 -i %s -o %s -r \"Go STONY BROOK\",\"./tests/fonts/font1.txt\",2,50,5", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// Copy & paste operations plus text rendering. Copied region overlaps the text.
TEST_F(image_operations_TestSuite, combined2) {
    const char *input_file = "./tests/images/stony.sbu";
    const char *expected_output_file = "./tests/expected_outputs/combined2.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -c 125,130,150,40 -i %s -p 85,130 -o %s -r \"Go STONY BROOK\",\"./tests/fonts/font4.txt\",2,100,10", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// Copy & paste operations plus text rendering
// Copied region does not overlap the text.
TEST_F(image_operations_TestSuite, combined3) {
    const char *input_file = "./tests/images/stony.sbu";
    const char *expected_output_file = "./tests/expected_outputs/combined3.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -c 125,130,150,40 -p 85,130 -i %s -o %s -r \"NEw york state\",\"./tests/fonts/font3.txt\",5,50,5", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}
