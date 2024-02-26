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

// Load a PPM image and perform a copy and paste operation
TEST_F(image_operations_TestSuite, copy_paste_cactus) {
    const char *input_file = "./tests/images/desert.ppm";
    const char *expected_output_file = "./tests/expected_outputs/cactus.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -c 90,10,50,100 -i %s -o %s -p 90,60", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// Load an SBU image and perform a copy and paste operation
TEST_F(image_operations_TestSuite, copy_paste_cactus_sbu) {
    const char *input_file = "./tests/images/desert.sbu";
    const char *expected_output_file = "./tests/expected_outputs/cactus.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -i %s -p 90,60 -c 90,10,50,100 -o %s", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// The copy region overflows right side of image, but not bottom. Paste entirely inside image.
TEST_F(image_operations_TestSuite, copy_paste_stony1_1) {
    const char *input_file = "./tests/images/stony.ppm";
    const char *expected_output_file = "./tests/expected_outputs/stony1_1.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -c 5,275,100,75 -p 10,20 -i %s -o %s", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// The copy region overflows right side of image, but not bottom. Paste entirely inside image.
TEST_F(image_operations_TestSuite, copy_paste_stony1_2) {
    const char *input_file = "./tests/images/stony.sbu";
    const char *expected_output_file = "./tests/expected_outputs/stony1_2.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -i %s -p 15,20 -c 75,200,300,10 -o %s", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// The copy region overflows bottom of image, but not right side. Paste entirely inside image.
TEST_F(image_operations_TestSuite, copy_paste_stony2_1) {
    const char *input_file = "./tests/images/stony.ppm";
    const char *expected_output_file = "./tests/expected_outputs/stony2_1.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -o %s -c 170,100,90,180 -p 0,0 -i %s", actual_output_file, input_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// The copy region overflows bottom of image, but not right side. Paste entirely inside image.
TEST_F(image_operations_TestSuite, copy_paste_stony2_2) {
    const char *input_file = "./tests/images/stony.sbu";
    const char *expected_output_file = "./tests/expected_outputs/stony2_2.sbu";
    const char *actual_output_file = "./tests/actual_outputs/result.sbu";
    sprintf(cmd, "./build/hw2_main -i %s -p 20,25 -o %s -c 75,200,50,175", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// The copy region overflows bottom and right side. Paste entirely inside image.
TEST_F(image_operations_TestSuite, copy_paste_stony3_1) {
    const char *input_file = "./tests/images/stony.ppm";
    const char *expected_output_file = "./tests/expected_outputs/stony3_1.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -i %s -c 120,235,100,150 -o %s -p 20,20", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// The copy region overflows bottom and right side. Paste entirely inside image.
TEST_F(image_operations_TestSuite, copy_paste_stony3_2) {
    const char *input_file = "./tests/images/stony.sbu";
    const char *expected_output_file = "./tests/expected_outputs/stony3_2.sbu";
    const char *actual_output_file = "./tests/actual_outputs/result.sbu";
    sprintf(cmd, "./build/hw2_main -c 120,235,100,150 -i %s -o %s -p 55,65", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// The copy region fits within the image, but the paste region overflows right side of the image.
TEST_F(image_operations_TestSuite, copy_paste_stony4_1) {
    const char *input_file = "./tests/images/stony.ppm";
    const char *expected_output_file = "./tests/expected_outputs/stony4_1.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -i %s -o %s -c 50,60,200,50 -p 50,170", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// The copy region fits within the image, but the paste region overflows right side of the image.
TEST_F(image_operations_TestSuite, copy_paste_stony4_2) {
    const char *input_file = "./tests/images/stony.sbu";
    const char *expected_output_file = "./tests/expected_outputs/stony4_2.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -i %s -c 25,150,185,125 -o %s -p 50,200", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// The copy region fits within the image, but the paste region overflows bottom of the image.
TEST_F(image_operations_TestSuite, copy_paste_stony5_1) {
    const char *input_file = "./tests/images/stony.ppm";
    const char *expected_output_file = "./tests/expected_outputs/stony5_1.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -o %s -c 100,100,150,50 -p 180,10 -i %s", actual_output_file, input_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// The copy region fits within the image, but the paste region overflows bottom of the image.
TEST_F(image_operations_TestSuite, copy_paste_stony5_2) {
    const char *input_file = "./tests/images/stony.sbu";
    const char *expected_output_file = "./tests/expected_outputs/stony5_2.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -o %s -c 100,100,150,50 -i %s -p 180,10", actual_output_file, input_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// The copy region fits within the image, but the paste region overflows right side and bottom of image.
TEST_F(image_operations_TestSuite, copy_paste_stony6_1) {
    const char *input_file = "./tests/images/stony.ppm";
    const char *expected_output_file = "./tests/expected_outputs/stony6_1.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -i %s -o %s -c 20,25,120,140 -p 100,200", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// The copy region fits within the image, but the paste region overflows right side and bottom of image.
TEST_F(image_operations_TestSuite, copy_paste_stony6_2) {
    const char *input_file = "./tests/images/stony.ppm";
    const char *expected_output_file = "./tests/expected_outputs/stony6_2.sbu";
    const char *actual_output_file = "./tests/actual_outputs/result.sbu";
    sprintf(cmd, "./build/hw2_main -p 100,200 -i %s -o %s -c 20,25,120,140", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}

// Copy and paste regions overlap each other
TEST_F(image_operations_TestSuite, overlapping1) {
    const char *input_file = "./tests/images/stony.ppm";
    const char *expected_output_file = "./tests/expected_outputs/overlapping1.ppm";
    const char *actual_output_file = "./tests/actual_outputs/result.ppm";
    sprintf(cmd, "./build/hw2_main -i %s -p 22,32 -o %s -c 20,30,120,140", input_file, actual_output_file);
    INFO(cmd);
    int status = run_using_system(cmd);
    EXPECT_EQ(0, WEXITSTATUS(status));
    check_image_file_contents(expected_output_file, actual_output_file);
}
