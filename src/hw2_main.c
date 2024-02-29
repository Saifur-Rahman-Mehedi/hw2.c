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
    FILE *file = fopen(path, "w");
    if (file == NULL) return false;
    fclose(file);
    return true;
}

bool validate_c_argument(const char *arg) {
    int values[4];
    return sscanf(arg, "%d,%d,%d,%d", &values[0], &values[1], &values[2], &values[3]) == 4 &&
           values[2] > 0 && values[3] > 0; // width and height must be positive
}

bool validate_p_argument(const char *arg) {
    int values[2];
    return sscanf(arg, "%d,%d", &values[0], &values[1]) == 2; // Only needs to parse two integers
}

bool validate_r_argument(const char *arg) {
    char message[256], fontPath[256];
    int values[3];
    return sscanf(arg, "%255[^,],%255[^,],%d,%d,%d", message, fontPath, &values[0], &values[1], &values[2]) == 5 &&
           values[0] > 0; // fontSize must be positive
}

int validate_args(int argc, char *argv[]) {
    bool i_flag = false, o_flag = false, c_flag = false, p_flag = false, r_flag = false;
    char *input_file = NULL, *output_file = NULL;

    int opt, error = 0;
    while ((opt = getopt(argc, argv, ":i:o:c:p:r:")) != -1) {
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
                if (!validate_c_argument(optarg)) return C_ARGUMENT_INVALID;
                break;
            case 'p':
                if (p_flag) return DUPLICATE_ARGUMENT;
                p_flag = true;
                if (!c_flag) return C_ARGUMENT_MISSING;
                if (!validate_p_argument(optarg)) return P_ARGUMENT_INVALID;
                break;
            case 'r':
                if (r_flag) return DUPLICATE_ARGUMENT;
                r_flag = true;
                if (!validate_r_argument(optarg)) return R_ARGUMENT_INVALID;
                break;
            case ':':
                return MISSING_ARGUMENT;
            case '?':
                return UNRECOGNIZED_ARGUMENT;
        }
    }

    if (!i_flag || !o_flag) return MISSING_ARGUMENT;
    if (!file_exists(input_file)) return INPUT_FILE_MISSING;
    if (!file_writable(output_file)) return OUTPUT_FILE_UNWRITABLE;

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
