#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MISSING_ARGUMENT 1
#define UNRECOGNIZED_ARGUMENT 2
#define DUPLICATE_ARGUMENT 3

int main(int argc, char *argv[]) {
    int opt;
    int error = 0;
    int i_flag = 0, o_flag = 0;

    while ((opt = getopt(argc, argv, "i:o:")) != -1) {
        switch (opt) {
            case 'i':
                if (i_flag) {
                    error = DUPLICATE_ARGUMENT;
                } else {
                    i_flag = 1;
                }
                break;
            case 'o':
                if (o_flag) {
                    error = DUPLICATE_ARGUMENT;
                } else {
                    o_flag = 1;
                }
                break;
            case '?':
            default: 
                error = UNRECOGNIZED_ARGUMENT;
                break;
        }
        if (error) break; 
    }


    if (error) {
        printf("Error code: %d\n", error);
        return error;
    }

    printf("All arguments validated successfully.\n");
    return 0;
}
