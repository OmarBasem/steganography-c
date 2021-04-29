#ifndef TEST
#define TEST

#define P3 "P3" // the constant P3 of every ppm image

typedef struct color {
    int r;
    int g;
    int b;
} color;

typedef struct PPM {
    char *p3;
    int cols;
    int rows;
    int maxColor;
    struct color **rgb; // like an array of structs
    char **comments; // like an array of strings
    int commentsCount;
} PPM;

struct PPM *getPPM(FILE *f);
void showPPM(struct PPM *im);
void writePPM(struct PPM *im, FILE *f);
char *intToBinary(int num, int bits);
char *stringToBinary(char *s, unsigned int mSize);
int binaryToDecimal(int num);
char *binaryToString(char *binary);

#endif