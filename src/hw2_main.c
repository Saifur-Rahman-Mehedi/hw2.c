#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

extern char *optarg;
extern int optopt;

#define MISSING_ARGUMENT 1
#define UNRECOGNIZED_ARGUMENT 2
#define DUPLICATE_ARGUMENT 3
#define INPUT_FILE_MISSING 4
#define OUTPUT_FILE_UNWRITABLE 5
#define C_ARGUMENT_MISSING 6
#define C_ARGUMENT_INVALID 7
#define P_ARGUMENT_INVALID 8
#define R_ARGUMENT_INVALID 9

typedef struct {
    unsigned char r, g, b;
} RGBPixel;

typedef struct {
    int width, height;
    RGBPixel *pixels;
} Image;


bool load_ppm(const char *filename, Image *image) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file");
        return false;
    }
    
    char magicNumber[3];
    if (!fgets(magicNumber, sizeof(magicNumber), file) || strcmp(magicNumber, "P3\n") != 0) {
        fprintf(stderr, "Invalid PPM file format.\n");
        fclose(file);
        return false;
    }

    if (fscanf(file, "%d %d", &image->width, &image->height) != 2) {
        fprintf(stderr, "Failed to read image dimensions.\n");
        fclose(file);
        return false;
    }

    int maxColorValue;
    if (fscanf(file, "%d", &maxColorValue) != 1 || maxColorValue != 255) {
        fprintf(stderr, "Invalid or unsupported max color value.\n");
        fclose(file);
        return false;
    }

    image->pixels = (RGBPixel*)malloc(image->width * image->height * sizeof(RGBPixel));
    if (image->pixels == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        fclose(file);
        return false;
    }

    for (int i = 0; i < image->width * image->height; i++) {
        int r, g, b;
        if (fscanf(file, "%d %d %d", &r, &g, &b) != 3) {
            fprintf(stderr, "Error reading pixel data.\n");
            free(image->pixels);
            fclose(file);
            return false;
        }
        image->pixels[i].r = (unsigned char)r;
        image->pixels[i].g = (unsigned char)g;
        image->pixels[i].b = (unsigned char)b;
    }

    fclose(file);
    return true;
}


bool load_sbu(const char *filename, Image *image) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file");
        return false;
    }

    char header[4];
    if (fscanf(file, "%3s", header) < 1 || strcmp(header, "SBU") != 0) {
        fprintf(stderr, "Invalid SBU file format.\n");
        fclose(file);
        return false;
    }

    if (fscanf(file, "%d %d", &image->width, &image->height) != 2) {
        fprintf(stderr, "Failed to read image dimensions.\n");
        fclose(file);
        return false;
    }

    int entries;
    if (fscanf(file, "%d", &entries) != 1) {
        fprintf(stderr, "Failed to read the number of color table entries.\n");
        fclose(file);
        return false;
    }

    while (fgetc(file) != '\n');

    RGBPixel *colorTable = malloc(entries * sizeof(RGBPixel));
    if (!colorTable) {
        fprintf(stderr, "Unable to allocate memory for color table.\n");
        fclose(file);
        return false;
    }

    for (int i = 0; i < entries; i++) {
        if (fscanf(file, "%hhu %hhu %hhu", &colorTable[i].r, &colorTable[i].g, &colorTable[i].b) != 3) {
            fprintf(stderr, "Failed to read color table entry %d.\n", i);
            free(colorTable);
            fclose(file);
            return false;
        }
    }

    image->pixels = malloc(image->width * image->height * sizeof(RGBPixel));
    if (!image->pixels) {
        fprintf(stderr, "Unable to allocate memory for pixels.\n");
        free(colorTable);
        fclose(file);
        return false;
    }

    int pix = 0;
    while (pix < image->width * image->height && !feof(file)) {
        char ch = fgetc(file);
        if (ch == '*') { 
            int count, index;
            fscanf(file, "%d %d", &count, &index);
            for (int i = 0; i < count; i++, pix++) {
                if (pix < image->width * image->height && index < entries) {
                    image->pixels[pix] = colorTable[index];
                }
            }
        } else if (ch >= '0' && ch <= '9') { 
            ungetc(ch, file); 
            int index;
            fscanf(file, "%d", &index);
            if (pix < image->width * image->height && index < entries) {
                image->pixels[pix++] = colorTable[index];
            }
        }
    }

    free(colorTable);
    fclose(file);
    return true;
}
bool compare_rgb_pixels(RGBPixel a, RGBPixel b) {
    return a.r == b.r && a.g == b.g && a.b == b.b;
}

int calculate_color_palette(Image *image, RGBPixel **palette, int *paletteSize) {
    RGBPixel *tempPalette = malloc(image->width * image->height * sizeof(RGBPixel));
    *paletteSize = 0;

    for (int i = 0; i < image->width * image->height; i++) {
        bool found = false;
        for (int j = 0; j < *paletteSize; j++) {
            if (compare_rgb_pixels(image->pixels[i], tempPalette[j])) {
                found = true;
                break;
            }
        }
        if (!found) {
            tempPalette[(*paletteSize)++] = image->pixels[i];
        }
    }

    *palette = malloc(*paletteSize * sizeof(RGBPixel));
    memcpy(*palette, tempPalette, *paletteSize * sizeof(RGBPixel));
    free(tempPalette);

    return *paletteSize; 
}



int find_palette_index(RGBPixel *palette, int paletteSize, RGBPixel pixel) {
    for (int i = 0; i < paletteSize; i++) {
        if (compare_rgb_pixels(pixel, palette[i])) {
            return i;
        }
    }
    return -1; 
}

bool save_ppm(const char *filename, Image *image) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Unable to open file for writing");
        return false;
    }

    fprintf(file, "P3\n%d %d\n255\n", image->width, image->height);

    for (int i = 0; i < image->width * image->height; i++) {
        fprintf(file, "%d %d %d\t", image->pixels[i].r, image->pixels[i].g, image->pixels[i].b);
        if ((i + 1) % image->width == 0) { 
            fprintf(file, "\n");
        }
    }

    fclose(file);
    return true;

}

bool save_sbu(const char *filename, Image *image) {
    FILE *file = fopen(filename, "wb"); 
    if (!file) {
        perror("Unable to open file for writing");
        return false;
    }

    fprintf(file, "SBU");
    fwrite(&image->width, sizeof(int), 1, file);
    fwrite(&image->height, sizeof(int), 1, file);

    int paletteSize = 0; 
    RGBPixel *palette = NULL;
    calculate_color_palette(image, &palette, &paletteSize); 

    fwrite(&paletteSize, sizeof(int), 1, file);
    for (int i = 0; i < paletteSize; i++) {
        fwrite(&palette[i], sizeof(RGBPixel), 1, file);
    }

    for (int i = 0; i < image->width * image->height; i++) {
        unsigned char index = find_palette_index(palette, paletteSize, image->pixels[i]);
        fwrite(&index, sizeof(unsigned char), 1, file);
    }

    fclose(file);
    free(palette);

    return true;
}

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
            case ':':
                if (optopt == 'i' || optopt == 'o' || optopt == 'c' || optopt == 'p' || optopt == 'r') {
                    error = MISSING_ARGUMENT;
                }
                break;
            case '?':
            default:
                error = UNRECOGNIZED_ARGUMENT;
                break;
        }
        if (error) break;
    }


if (!i_flag || !o_flag) {
    fprintf(stderr, "Error: Missing required arguments.\n");
    return MISSING_ARGUMENT;
}

if (!error && input_file && !file_exists(input_file)) {
    fprintf(stderr, "Error: Input file does not exist.\n");
    return INPUT_FILE_MISSING;
}

if (!error && output_file && !file_writable(output_file)) {
    fprintf(stderr, "Error: Output file is not writable.\n");
    return OUTPUT_FILE_UNWRITABLE;
}

if (error) {
    fprintf(stderr, "Error: %d\n", error);
    return error;
}

if (i_flag) {
    char *extension = strrchr(input_file, '.');
    if (extension != NULL) {
        Image image = {0}; 
        bool load_success = false; 
        
        if (strcmp(extension, ".ppm") == 0) {
            load_success = load_ppm(input_file, &image);
        } else if (strcmp(extension, ".sbu") == 0) {
            load_success = load_sbu(input_file, &image);
        } else {
            fprintf(stderr, "Unsupported file format.\n");
        }
        
        if (!load_success) {
            return 1; 
        }

        free(image.pixels); 
    } else {
        fprintf(stderr, "Invalid file path.\n");
        return 1; 
    }
}

    Image image;
    bool load_success = false, save_success = false;

    char *in_extension = strrchr(input_file, '.');
    if (strcmp(in_extension, ".ppm") == 0) {
        load_success = load_ppm(input_file, &image);
    } else if (strcmp(in_extension, ".sbu") == 0) {
        load_success = load_sbu(input_file, &image);
    } else {
        fprintf(stderr, "Unsupported input file format.\n");
    }

    if (!load_success) {
        fprintf(stderr, "Failed to load the input file.\n");
        return 1;
    }

    char *out_extension = strrchr(output_file, '.');
    if (strcmp(out_extension, ".ppm") == 0) {
        save_success = save_ppm(output_file, &image);
    } else if (strcmp(out_extension, ".sbu") == 0) {
        save_success = save_sbu(output_file, &image);
    } else {
        fprintf(stderr, "Unsupported output file format.\n");
    }

    if (!save_success) {
        fprintf(stderr, "Failed to save the output file.\n");
        free(image.pixels); 
        return 1;
    }

    free(image.pixels);
return 0; 

}