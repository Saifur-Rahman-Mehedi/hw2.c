#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "gtest/gtest.h"
#include "tests_aux.h"
#include "hw2.h"

class validate_args_TestSuite : public testing::Test { 
	void SetUp() override {
        system("rm -rf ./tests/actual_outputs");
		system("rm -f /tmp/exp_single_line.txt and /tmp/act_single_line.txt");
		mkdir("./tests/actual_outputs/", 0700);
	}
};

// Swapping the order of the -i and -o arguments should not matter
TEST_F(validate_args_TestSuite, acceptable_args) {
	int status = run_using_system("-o ./tests/actual_outputs/result.sbu -i ./tests/images/seawolf.sbu");
	EXPECT_EQ(0, WEXITSTATUS(status));
}

// -o argument is missing
TEST_F(validate_args_TestSuite, missing_o_arg) {
	int status = run_using_system("-i ./tests/images/seawolf.ppm -r \"I love SBU\",\"fonts/font3.txt\",1,4,2");
	EXPECT_EQ(MISSING_ARGUMENT, WEXITSTATUS(status));
}

// -i argument is missing
TEST_F(validate_args_TestSuite, missing_i_arg) {
	int status = run_using_system("-o ./tests/actual_output/result.ppm -r \"I love SBU\",\"fonts/font3.txt\",1,4,2");
	EXPECT_EQ(MISSING_ARGUMENT, WEXITSTATUS(status));
}

// Unrecognized argument and parameter are provided.
TEST_F(validate_args_TestSuite, unrecog_arg01) {
	int status = run_using_system("-i ./tests/images/seawolf.ppm -j 77 -o ./tests/actual_outputs/result.ppm");
	EXPECT_EQ(UNRECOGNIZED_ARGUMENT, WEXITSTATUS(status));
}

// UNRECOGNIZED_ARGUMENT takes precedence over DUPLICATE_ARGUMENT
TEST_F(validate_args_TestSuite, unrecog_arg02) {
	int status = run_using_system("-i ./tests/images/seawolf.ppm -o ./tests/actual_outputs/result1.ppm -k 100 -o ./tests/actual_outputs/result2.ppm");
	EXPECT_EQ(UNRECOGNIZED_ARGUMENT, WEXITSTATUS(status));
}

// -o argument provided twice
TEST_F(validate_args_TestSuite, duplicate_arg01) {
	int status = run_using_system("-i ./tests/images/seawolf.ppm -o ./tests/actual_outputs/result1.ppm -o ./tests/actual_outputs/result2.ppm");
	EXPECT_EQ(DUPLICATE_ARGUMENT, WEXITSTATUS(status));
}

// -i argument provided twice
TEST_F(validate_args_TestSuite, duplicate_arg02) {
	int status = run_using_system("-i ./tests/images/seawolf.ppm -o ./tests/actual_outputs/result1.ppm -i ./tests/images/seawolf.ppm");
	EXPECT_EQ(DUPLICATE_ARGUMENT, WEXITSTATUS(status));
}

// Input file is missing
TEST_F(validate_args_TestSuite, input_missing) {
	int status = run_using_system("-i ./tests/images/garbage.ppm -o ./tests/actual_outputs/result1.ppm");
	EXPECT_EQ(INPUT_FILE_MISSING, WEXITSTATUS(status));
}

// DUPLICATE_ARGUMENT takes precedence over INPUT_FILE_MISSING
TEST_F(validate_args_TestSuite, duplicate_arg03) {
	int status = run_using_system("-i ./tests/images/garbage.ppm -o ./tests/actual_outputs/result1.ppm -i ./tests/images/garbage.ppm");
	EXPECT_EQ(DUPLICATE_ARGUMENT, WEXITSTATUS(status));
}

// Output file cannot be written
TEST_F(validate_args_TestSuite, unwriteable_file01) {
	int status = run_using_system("-i ./tests/images/seawolf.ppm -o /result1.ppm");
	EXPECT_EQ(OUTPUT_FILE_UNWRITABLE, WEXITSTATUS(status));
}

// DUPLICATE_ARGUMENT takes precedence over OUTPUT_FILE_UNWRITABLE 
TEST_F(validate_args_TestSuite, duplicate_arg04) {
	int status = run_using_system("-o /result1.ppm -i ./tests/images/seawolf.ppm -o /result1.ppm");
	EXPECT_EQ(DUPLICATE_ARGUMENT, WEXITSTATUS(status));
}

// -o argument is missing its parameter
TEST_F(validate_args_TestSuite, missing_parameter01) {
	int status = run_using_system("-i ./tests/images/seawolf.ppm -o");
	EXPECT_EQ(MISSING_ARGUMENT, WEXITSTATUS(status));
}

// -p argument is missing its parameter
TEST_F(validate_args_TestSuite, missing_parameter02) {
	int status = run_using_system("-i ./tests/images/seawolf.ppm -p -o /result1.ppm");
	EXPECT_EQ(MISSING_ARGUMENT, WEXITSTATUS(status));
}

// -r argument is missing its parameter
TEST_F(validate_args_TestSuite, missing_parameter03) {
	int status = run_using_system("-i ./tests/images/seawolf.ppm -r -o ./tests/actual_outputs/result1.ppm");
	EXPECT_EQ(MISSING_ARGUMENT, WEXITSTATUS(status));
}

// -i argument is missing its parameter
TEST_F(validate_args_TestSuite, missing_parameter04) {
	int status = run_using_system("-i -p 10,20 -o ./tests/actual_outputs/result1.ppm");
	EXPECT_EQ(MISSING_ARGUMENT, WEXITSTATUS(status));
}

// -p argument is given without -c parameter
TEST_F(validate_args_TestSuite, missing_c_arg) {
	int status = run_using_system("-i ./tests/images/seawolf.ppm -p 10,20 -o ./tests/actual_outputs/result1.ppm");
	EXPECT_EQ(C_ARGUMENT_MISSING, WEXITSTATUS(status));
}

// MISSING_ARGUMENT takes precedence over C_ARGUMENT_MISSING
TEST_F(validate_args_TestSuite, missing_parameter05) {
	int status = run_using_system("-i ./tests/images/seawolf.ppm -p -o ./tests/actual_outputs/result1.ppm");
	EXPECT_EQ(MISSING_ARGUMENT, WEXITSTATUS(status));
}

// MISSING_ARGUMENT takes precedence over C_ARGUMENT_MISSING
TEST_F(validate_args_TestSuite, missing_parameter06) {
	int status = run_using_system("-r -i ./tests/images/seawolf.ppm -p 10,20 -o ./tests/actual_outputs/result1.ppm");
	EXPECT_EQ(MISSING_ARGUMENT, WEXITSTATUS(status));
}

// MISSING_ARGUMENT takes precedence over C_ARGUMENT_MISSING
TEST_F(validate_args_TestSuite, missing_parameter07) {
	int status = run_using_system("-i ./tests/images/seawolf.ppm -p 10,20 -o ./tests/actual_outputs/result1.ppm -r");
	EXPECT_EQ(MISSING_ARGUMENT, WEXITSTATUS(status));
}

// MISSING_ARGUMENT takes precedence over C_ARGUMENT_MISSING
TEST_F(validate_args_TestSuite, missing_parameter08) {
	int status = run_using_system("-i ./tests/images/seawolf.ppm -p 10,20 -o");
	EXPECT_EQ(MISSING_ARGUMENT, WEXITSTATUS(status));
}

// -c argument is missing an input value.
TEST_F(validate_args_TestSuite, c_argument_invalid01) {
	int status = run_using_system("-i ./tests/images/seawolf.ppm -o ./tests/actual_outputs/result1.ppm -c 12,15,20");
	EXPECT_EQ(C_ARGUMENT_INVALID, WEXITSTATUS(status));
}

// -c argument is missing an input value.
TEST_F(validate_args_TestSuite, c_argument_invalid02) {
	int status = run_using_system("-i ./tests/images/seawolf.ppm -o ./tests/actual_outputs/result1.ppm -c 12,15,20,");
	EXPECT_EQ(C_ARGUMENT_INVALID, WEXITSTATUS(status));
}

// -c argument has too many input values.
TEST_F(validate_args_TestSuite, c_argument_invalid03) {
	int status = run_using_system("-i ./tests/images/seawolf.ppm -o ./tests/actual_outputs/result1.ppm -c 12,15,20,30,5");
	EXPECT_EQ(C_ARGUMENT_INVALID, WEXITSTATUS(status));
}

// -p argument is missing an input value.
TEST_F(validate_args_TestSuite, p_argument_invalid01) {
	int status = run_using_system("-i ./tests/images/seawolf.ppm -p 10, -o ./tests/actual_outputs/result1.ppm -c 10,20,30,15");
	EXPECT_EQ(P_ARGUMENT_INVALID, WEXITSTATUS(status));
}

// -r argument is given an invalid font file.
TEST_F(validate_args_TestSuite, r_argument_invalid01) {
	int status = run_using_system("-i ./tests/images/seawolf.ppm -p 10,20 -r \"hello\",\"./tests/fonts/fonts200.txt\",1,10,15 -o ./tests/actual_outputs/result1.ppm -c 10,20,30,15");
	EXPECT_EQ(R_ARGUMENT_INVALID, WEXITSTATUS(status));
}

// -r argument is missing an input value.
TEST_F(validate_args_TestSuite, r_argument_invalid02) {
	int status = run_using_system("-i ./tests/images/seawolf.ppm -p 10,20 -r \"hello\",\"./tests/fonts/font1.txt\",10,15 -o ./tests/actual_outputs/result1.ppm -c 10,20,30,15");
	EXPECT_EQ(R_ARGUMENT_INVALID, WEXITSTATUS(status));
}

// -r argument is has too many input values.
TEST_F(validate_args_TestSuite, r_argument_invalid03) {
	int status = run_using_system("-i ./tests/images/seawolf.ppm -p 10,20 -r \"hello\",\"./tests/fonts/font1.txt\",1,10,15,\"oops\" -o ./tests/actual_outputs/result1.ppm -c 10,20,30,15");
	EXPECT_EQ(R_ARGUMENT_INVALID, WEXITSTATUS(status));
}

// MISSING_ARGUMENT takes precedence over R_ARGUMENT_INVALID
TEST_F(validate_args_TestSuite, missing_parameter09) {
	int status = run_using_system("-p 10,20 -r \"hello\",\"./tests/fonts/fonts200.txt\",10,15 -o ./tests/actual_outputs/result1.ppm -c 10,20,30,15");
	EXPECT_EQ(MISSING_ARGUMENT, WEXITSTATUS(status));
}

// MISSING_ARGUMENT takes precedence over R_ARGUMENT_INVALID
TEST_F(validate_args_TestSuite, missing_parameter10) {
	int status = run_using_system("-i ./tests/images/seawolf.ppm -p 10,20 -r \"hello\",\"./tests/fonts/fonts200.txt\",10,15 -o ./tests/actual_outputs/result1.ppm");
	EXPECT_EQ(C_ARGUMENT_MISSING, WEXITSTATUS(status));
}