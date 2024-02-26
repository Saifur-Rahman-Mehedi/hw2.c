#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <string>
#include "gtest/gtest.h"

#define INFO(MSG) do{std::cerr << "[          ] [ INFO ] " << (MSG) << std::endl;}while(0)

bool file_exists(const char *path);
void expect_no_valgrind_errors(int status);
void check_image_file_contents(const char *expected_file, const char *actual_file);
int run_using_system(const char *args);
int run_using_valgrind(const char *args);
