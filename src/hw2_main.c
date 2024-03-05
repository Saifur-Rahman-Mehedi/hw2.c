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

typedef struct {
    int width, height;        
    unsigned char* data;      
} Image;

Image* loadImage(const char *path) {
    FILE* file = fopen(path, "rb"); 
    if (file == NULL) {
        perror("Failed to open file");
        return NULL;
    }

    char header[3];
    if (fscanf(file, "%2s", header) != 1 || strncmp(header, "P6", 2) != 0) {
        fprintf(stderr, "Unsupported or invalid image format\n");
        fclose(file);
        return NULL;
    }

    int width, height, maxval;
    if (fscanf(file, "%d %d %d", &width, &height, &maxval) != 3) {
        fprintf(stderr, "Invalid image size or maxval\n");
        fclose(file);
        return NULL;
    }

    while (fgetc(file) != '\n'); 

    if (maxval != 255) {
        fprintf(stderr, "Unsupported maxval\n");
        fclose(file);
        return NULL;
    }

    Image* image = malloc(sizeof(Image));
    if (image == NULL) {
        perror("Failed to allocate memory for image");
        fclose(file);
        return NULL;
    }

    image->width = width;
    image->height = height;
    image->data = malloc(3 * width * height);
    if (image->data == NULL) {
        perror("Failed to allocate memory for pixel data");
        free(image);
        fclose(file);
        return NULL;
    }

    if (fread(image->data, 3, width * height, file) != width * height) {
        fprintf(stderr, "Failed to read pixel data\n");
        free(image->data);
        free(image);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return image;
}

bool saveImage(const char *path, const Image *image) {
    FILE* file = fopen(path, "wb");
    if (file == NULL) {
        perror("Failed to open output file");
        return false;
    }

    fprintf(file, "P6\n%d %d\n255\n", image->width, image->height);

    size_t written = fwrite(image->data, 3, image->width * image->height, file);
    if (written != image->width * image->height) {
        fprintf(stderr, "Failed to write pixel data\n");
        fclose(file);
        return false;
    }

    fclose(file);
    return true;
}

Image* copyRegion(const Image* src, int row, int col, int width, int height) {
    if (!src || !src->data || row < 0 || col < 0 || width <= 0 || height <= 0 || 
        row + height > src->height || col + width > src->width) {
        printf("Invalid region specified.\n");
        return NULL;
    }

    Image* region = malloc(sizeof(Image));
    if (!region) {
        perror("Failed to allocate memory for region");
        return NULL;
    }
    
    region->width = width;
    region->height = height;
    region->data = malloc(3 * width * height);
    if (!region->data) {
        perror("Failed to allocate memory for region data");
        free(region);
        return NULL;
    }

    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            int srcIndex = ((row + r) * src->width + (col + c)) * 3;
            int dstIndex = (r * width + c) * 3;
            memcpy(region->data + dstIndex, src->data + srcIndex, 3);
        }
    }

    return region;
}

int main(int argc, char *argv[]) {
    bool i_flag = false, o_flag = false, c_flag = false, p_flag = false, r_flag = false;
    char *input_file = NULL, *output_file = NULL;
    int opt, error = 0;

    Image* loadedImage = NULL;


    while ((opt = getopt(argc, argv, ":i:o:c:p:r:")) != -1) {
        switch (opt) {
            case 'i':
                if (i_flag) error = DUPLICATE_ARGUMENT;
                else {
                    i_flag = true;
                    input_file = optarg;
                    loadedImage = loadImage(input_file);
                    if (!loadedImage) {
                        error = INPUT_FILE_MISSING;
                    }
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
                if (p_flag) error = DUPLICATE_ARGUMENT;
                else if (!c_flag) error = C_ARGUMENT_MISSING;
                else if (!validate_p_argument(optarg)) error = P_ARGUMENT_INVALID;
                else p_flag = true;
                break;
            case 'r':
                if (r_flag) error = DUPLICATE_ARGUMENT;
                else if (!c_flag) error = C_ARGUMENT_MISSING;
                else if (!validate_r_argument(optarg)) error = R_ARGUMENT_INVALID;
                else r_flag = true;
                break;
            case ':':
                error = MISSING_ARGUMENT;
                break;
            case '?':
            default:
                error = UNRECOGNIZED_ARGUMENT;
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
    if (loadedImage) {
            free(loadedImage->data);
            free(loadedImage);
    }

if (o_flag && loadedImage) {
    if (!saveImage(output_file, loadedImage)) {
        fprintf(stderr, "Error saving the image to the output file\n");
    } else {
        printf("Image saved successfully to %s\n", output_file);
    }
}

if (loadedImage) {
    free(loadedImage->data);
    free(loadedImage);
}

    return 0;
}
