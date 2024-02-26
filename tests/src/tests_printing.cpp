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

// Print a short message that fits entirely in the image
TEST_F(image_operations_TestSuite, print_short_message1_1) {
    const char *input_file = "./tests/images/desert.ppm";
    const char *expected_output_file = "./tests/expected_outputs/desert_short_message1_1.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -i %s -o %s -r \"seawolves\",\"./tests/fonts/font1.txt\",1,100,150", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// Print a short message that fits entirely in the image
TEST_F(image_operations_TestSuite, print_short_message1_2) {
    const char *input_file = "./tests/images/desert.sbu";
    const char *expected_output_file = "./tests/expected_outputs/desert_short_message1_2.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -i %s -r \"seawolves\",\"./tests/fonts/font1.txt\",1,100,150 -o %s", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// Print a short message that fits entirely in the image
TEST_F(image_operations_TestSuite, print_short_message2_1) {
    const char *input_file = "./tests/images/desert.ppm";
    const char *expected_output_file = "./tests/expected_outputs/desert_short_message2_1.sbu";
    const char *actual_output_file = "./tests/actual_outputs/result.sbu";
    sprintf(cmd, "./build/hw2_main -o %s -r \"stONY brOOK desert\",\"./tests/fonts/font2.txt\",1,60,45 -i %s", actual_output_file, input_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// Print a short message that fits entirely in the image
TEST_F(image_operations_TestSuite, print_short_message2_2) {
    const char *input_file = "./tests/images/desert.sbu";
    const char *expected_output_file = "./tests/expected_outputs/desert_short_message2_2.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -i %s -o %s -r \"stONY brOOK\",\"./tests/fonts/font2.txt\",1,60,100", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// Print a message that overflows the right side of the image
TEST_F(image_operations_TestSuite, print_overflow_message1_1) {
    const char *input_file = "./tests/images/desert.ppm";
    const char *expected_output_file = "./tests/expected_outputs/desert_overflow_message1_1.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -i %s -o %s -r \"new YORK state\",\"./tests/fonts/font3.txt\",1,10,220", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// Print a message that overflows the right side of the image
TEST_F(image_operations_TestSuite, print_overflow_message1_2) {
    const char *input_file = "./tests/images/desert.sbu";
    const char *expected_output_file = "./tests/expected_outputs/desert_overflow_message1_2.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -i %s -o %s -r \"new YORK state\",\"./tests/fonts/font3.txt\",1,10,220", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// Print a message that overflows the right side of the image
TEST_F(image_operations_TestSuite, print_overflow_message2_1) {
    const char *input_file = "./tests/images/desert.ppm";
    const char *expected_output_file = "./tests/expected_outputs/desert_overflow_message2_1.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -i %s -o %s -r \"new YORK state\",\"./tests/fonts/font4.txt\",1,10,200", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// Print a message that overflows the right side of the image
TEST_F(image_operations_TestSuite, print_overflow_message2_2) {
    const char *input_file = "./tests/images/desert.sbu";
    const char *expected_output_file = "./tests/expected_outputs/desert_overflow_message2_2.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -i %s -o %s -r \"new YORK state\",\"./tests/fonts/font4.txt\",1,10,200", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// Print a message that overflows the right side of the image
TEST_F(image_operations_TestSuite, print_overflow_message3_1) {
    const char *input_file = "./tests/images/desert.ppm";
    const char *expected_output_file = "./tests/expected_outputs/desert_overflow_message3_1.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -i %s -o %s -r \"new YORK state\",\"./tests/fonts/font1.txt\",2,40,180", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// Print a message that overflows the right side of the image
TEST_F(image_operations_TestSuite, print_overflow_message3_2) {
    const char *input_file = "./tests/images/desert.sbu";
    const char *expected_output_file = "./tests/expected_outputs/desert_overflow_message3_2.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -i %s -o %s -r \"new YORK state\",\"./tests/fonts/font1.txt\",2,40,180", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// Print a message that overflows the right side of the image
TEST_F(image_operations_TestSuite, print_overflow_message4_1) {
    const char *input_file = "./tests/images/desert.ppm";
    const char *expected_output_file = "./tests/expected_outputs/desert_overflow_message4_1.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -i %s -o %s -r \"seawolves\",\"./tests/fonts/font2.txt\",3,10,180", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// Print a message that overflows the right side of the image
TEST_F(image_operations_TestSuite, print_overflow_message4_2) {
    const char *input_file = "./tests/images/desert.sbu";
    const char *expected_output_file = "./tests/expected_outputs/desert_overflow_message4_2.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -i %s -o %s -r \"seawolves\",\"./tests/fonts/font2.txt\",3,10,180", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}