#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h> 

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
    return access(path, F_OK | R_OK) == 0;
}

bool file_writable(const char *path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) return false;
    fclose(file);
    return true;
}

bool validate_c_argument(char *arg) {
    int row, col, width, height;
    if (sscanf(arg, "%d,%d,%d,%d", &row, &col, &width, &height) != 4) {
        return false;
    }
    return row >= 0 && col >= 0 && width > 0 && height > 0;
}

bool validate_p_argument(char *arg) {
    int row, col;
    if (sscanf(arg, "%d,%d", &row, &col) != 2) {
        return false;
    }
    return row >= 0 && col >= 0;
}

bool validate_r_argument(char *arg, bool c_flag) {
    if (!c_flag) {
        return false;
    }
    
    char message[256] = {0}, fontPath[256] = {0};
    int fontSize, row, col;
    if (sscanf(arg, "%255[^,],%255[^,],%d,%d,%d", message, fontPath, &fontSize, &row, &col) != 5) {
        return false;
    }
    
    if (fontSize < 1 || fontSize > 10 || row < 0 || col < 0 || !file_exists(fontPath)) {
        return false;
    }

    return true;
}

int validate_args(int argc, char *argv[]) {
    bool i_flag = false, o_flag = false, c_flag = false, p_flag = false, r_flag = false;
    char *input_file = NULL, *output_file = NULL;
    char c_arg[1024] = {0}, p_arg[1024] = {0};

    int opt;
    while ((opt = getopt(argc, argv, "i:o:c:p:r:")) != -1) {
        switch (opt) {
            case 'i':
                if (i_flag) return DUPLICATE_ARGUMENT;
                i_flag = true;
                input_file = optarg;
                break;
            case 'o':
                if (o_flag) return DUPLICATE_ARGUMENT;
                o_flag = true;
                output_file = optarg;
                break;
            case 'c':
                if (c_flag) return DUPLICATE_ARGUMENT;
                c_flag = true;
                strncpy(c_arg, optarg, sizeof(c_arg) - 1);
                if (!validate_c_argument(c_arg)) return C_ARGUMENT_INVALID;
                break;
            case 'p':
                if (!c_flag) return C_ARGUMENT_MISSING;
                if (p_flag) return DUPLICATE_ARGUMENT;
                p_flag = true;
                strncpy(p_arg, optarg, sizeof(p_arg) - 1);
                if (!validate_p_argument(p_arg)) return P_ARGUMENT_INVALID;
                break;
            case 'r':
                if (r_flag) return DUPLICATE_ARGUMENT;
                r_flag = true;
                if (!validate_r_argument(optarg, c_flag)) return R_ARGUMENT_INVALID;
                break;
            case ':':
                return MISSING_ARGUMENT;
            case '?':
            default:
                return UNRECOGNIZED_ARGUMENT;
        }
    }

    if (!i_flag || !o_flag) return MISSING_ARGUMENT;
    if (!file_exists(input_file)) return INPUT_FILE_MISSING;
    if (!file_writable(output_file)) return OUTPUT_FILE_UNWRITABLE;

    free(input_file);
    free(output_file);

    return 0;
}

int main(int argc, char *argv[]) {
    int validation_status = validate_args(argc, argv);
    if (validation_status != 0) {
        fprintf(stderr, "Error: %d\n", validation_status);
        return validation_status;
    }

    printf("All arguments validated successfully.\n");
    return 0;
}
