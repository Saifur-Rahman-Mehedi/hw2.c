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

bool file_exists(const char *path);
bool file_writable(const char *path);

bool validate_c_argument(const char *arg);
bool validate_p_argument(const char *arg);
bool validate_r_argument(const char *arg);

int main(int argc, char *argv[]) {
    int opt, error = 0;
    bool i_flag = false, o_flag = false, c_flag = false, p_flag = false, r_flag = false;
    char *input_file = NULL, *output_file = NULL;

    while ((opt = getopt(argc, argv, ":i:o:c:p:r:")) != -1 && error == 0) {
        switch (opt) {
            case 'i':
                if (i_flag) error = DUPLICATE_ARGUMENT;
                else {
                    i_flag = true;
                    input_file = optarg;
                    if (!file_exists(input_file)) error = INPUT_FILE_MISSING;
                }
                break;
            case 'o':
                if (o_flag) error = DUPLICATE_ARGUMENT;
                else {
                    o_flag = true;
                    output_file = optarg;
                    if (!file_writable(output_file)) error = OUTPUT_FILE_UNWRITABLE;
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
                else {
                    p_flag = true;
                    if (!validate_p_argument(optarg)) error = P_ARGUMENT_INVALID;
                }
                break;
            case 'r':
                if (!c_flag) error = C_ARGUMENT_MISSING;
                else if (r_flag) error = DUPLICATE_ARGUMENT;
                else {
                    r_flag = true;
                    if (!validate_r_argument(optarg)) error = R_ARGUMENT_INVALID;
                }
                break;
            case ':': 
                error = MISSING_ARGUMENT;
                break;
            case '?': 
            default:
                error = UNRECOGNIZED_ARGUMENT;
                break;
        }
    }

    if (!i_flag || !o_flag) error = MISSING_ARGUMENT;

    if (error != 0) {
        fprintf(stderr, "Error: %d\n", error);
        return error;
    }

    printf("All arguments validated successfully.\n");

    // proceess with loading the image, processing it, and saving the result
    // according to the validated command-line arguments.

    return 0;
}

bool file_exists(const char *path) {
    return access(path, F_OK) == 0;
}

bool file_writable(const char *path) {
    FILE *file = fopen(path, "ab"); 
    if (file == NULL) return false; 
    fclose(file); 
    return true; 
}

bool validate_c_argument(const char *arg) {
    int values[4]; 
    if (sscanf(arg, "%d,%d,%d,%d", &values[0], &values[1], &values[2], &values[3]) != 4)
        return false; 
    for (int i = 0; i < 4; ++i) {
        if (values[i] < 0) return false; 
    }
    return true;
}

bool validate_p_argument(const char *arg) {
    int row, col;
    if (sscanf(arg, "%d,%d", &row, &col) != 2)
        return false; 
    if (row < 0 || col < 0) return false; 
    return true;
}

bool validate_r_argument(const char *arg) {
    char message[256], fontPath[256];
    int fontSize, row, col;
    if (sscanf(arg, "%255[^,],%255[^,],%d,%d,%d", message, fontPath, &fontSize, &row, &col) != 5)
        return false; 
    if (fontSize < 1 || fontSize > 10 || row < 0 || col < 0) return false; 
    if (!file_exists(fontPath)) return false;
    return true;
}


