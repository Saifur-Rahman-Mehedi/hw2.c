#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

extern char *optarg;

#define MISSING_ARGUMENT 1
#define UNRECOGNIZED_ARGUMENT 2
#define DUPLICATE_ARGUMENT 3
#define INPUT_FILE_MISSING 4
#define OUTPUT_FILE_UNWRITABLE 5
#define C_ARGUMENT_MISSING 6
#define C_ARGUMENT_INVALID 7
#define P_ARGUMENT_INVALID 8
#define R_ARGUMENT_INVALID 9

bool file_exists(const char *path) {
    return access(path, F_OK) == 0;
}

bool file_writable(const char *path) {
    char tempFilePath[256];
    snprintf(tempFilePath, sizeof(tempFilePath), "%s.tmp", path);
    FILE *file = fopen(tempFilePath, "w");
    if (file == NULL) return false;
    fclose(file);
    remove(tempFilePath);
    return true;
}

bool validate_c_argument(const char *arg) {
    int row, col, width, height;
    return sscanf(arg, "%d,%d,%d,%d", &row, &col, &width, &height) == 4 &&
           row >= 0 && col >= 0 && width > 0 && height > 0;
}

bool validate_p_argument(const char *arg) {
    int row, col;
    return sscanf(arg, "%d,%d", &row, &col) == 2 &&
           row >= 0 && col >= 0;
}

bool validate_r_argument(const char *arg) {
    char message[256] = {0}, fontPath[256] = {0};
    int fontSize, row, col;
    int result = sscanf(arg, "%255[^,],%255[^,],%d,%d,%d", message, fontPath, &fontSize, &row, &col);

    if (result != 5) return false;
    if (fontSize < 1 || fontSize > 10) return false;
    if (row < 0 || col < 0) return false;
    if (!file_exists(fontPath)) return false;

    return true;
}

int main(int argc, char *argv[]) {
    bool i_flag = false, o_flag = false, c_flag = false, p_flag = false, r_flag = false;
    char *input_file = NULL, *output_file = NULL;
    int opt, error = 0;

    while ((opt = getopt(argc, argv, ":i:o:c:p:r:")) != -1) {
        switch (opt) {
            case 'i':
                if (i_flag) error = DUPLICATE_ARGUMENT;
                else {
                    i_flag = true;
                    input_file = optarg;
                }
                break;
            case 'o':
                if (o_flag) error = DUPLICATE_ARGUMENT;
                else {
                    o_flag = true;
                    output_file = optarg;
                }
                break;
            case 'c':
                if (c_flag) error = DUPLICATE_ARGUMENT;
                else {
                    c_flag = true;
                    if (!validate_c_argument(optarg)) error = C_ARGUMENT_INVALID;
                }
                break;
            case 'p':
                if (!c_flag) error = C_ARGUMENT_MISSING;
                else if (p_flag) error = DUPLICATE_ARGUMENT;
                else if (!validate_p_argument(optarg)) error = P_ARGUMENT_INVALID;
                else p_flag = true;
                break;
            case 'r':
                if (!c_flag) error = C_ARGUMENT_MISSING;
                else if (r_flag) error = DUPLICATE_ARGUMENT;
                else if (!validate_r_argument(optarg)) error = R_ARGUMENT_INVALID;
                else r_flag = true;
                break;
            case '?':
                error = UNRECOGNIZED_ARGUMENT;
                break;
            case ':':
                error = MISSING_ARGUMENT;
                break;
        }
        if (error) break;
    }

    if (!i_flag || !o_flag) error = MISSING_ARGUMENT;
    if (!error && !file_exists(input_file)) error = INPUT_FILE_MISSING;
    if (!error && !file_writable(output_file)) error = OUTPUT_FILE_UNWRITABLE;

    if (error) {
        fprintf(stderr, "Error: %d\n", error);
        return error;
    }

    printf("All arguments validated successfully.\n");
    return 0;
}