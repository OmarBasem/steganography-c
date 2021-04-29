#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "common.h"

#define P3 "P3" // the constant P3 of every ppm image

// reads a file and returns a ppm struct
struct PPM *getPPM(FILE *f) {
    struct PPM *ppm = (struct PPM *) malloc(sizeof(struct PPM)); // allocate space of ppm struct pointer
    char ch = getc(f);
    int number;
    char **comments = (char **) malloc(sizeof(char *) * 2); // allocate space using malloc for comments
    int i = 0; // variable for pixel index
    int j = 0; // variable for character position of each comment
    int k = 0; // variable for the comment index in the comments pointer(array)
    ppm->p3 = P3; // first, write P3
    while (ch != '\n') { // skip the first line which is "P3"
        ch = getc(f);
    }
    int p = 2;
    ch = getc(f); // get the next char in file
    while (ch == '#') { // while there is '#', read that comment
        char *comment = (char *) malloc(100);
        while (ch != '\n') { // read till end of line
            comment[j] = ch;
            j++;
            ch = getc(f);
        }
        comments[k] = comment; // add the comment to the comments array
        ppm->commentsCount = k + 1; // increment comments count
        ch = getc(f); // read next char
        k++;
        j = 0; // put j back to zero to read next comment
    }
    ppm->comments = comments; // finally set the comments in the ppm struct

    fseek(f, -1, SEEK_CUR); // seek back after the last check for '#'

    fscanf(f, "%d", &ppm->cols); // read cols
    fscanf(f, "%d", &ppm->rows); // read rows
    fscanf(f, "%d", &ppm->maxColor); // read max color
    fscanf(f, "%d", &number); // read r value of first pixel
    struct color **rgb = (struct color **) malloc(sizeof(struct color *) * (ppm->rows * ppm->cols)); // allocate space for rgb structs array(pointer to pointer)
    while (i < (ppm->rows * ppm->cols)) { // rows * cols is the total number of pixels
        struct color *c = (struct color *) malloc(sizeof(struct color)); // allocate space for color struct
        c->r = number; // assign r value
        fscanf(f, "%d", &number); // read g value
        c->g = number; // assign g value
        fscanf(f, "%d", &number); // read b value
        c->b = number; // assign b value
        rgb[i] = c; // assign the pixel
        fscanf(f, "%d", &number); // read r value of next pixel
        i++;
    }
    ppm->rgb = rgb; // Finally, assign the rgb array in the ppm struct
    return ppm; // return the ppm
}

// function to display a ppm struct as text
void showPPM(struct PPM *im) {
    printf("%s\n", im->p3); // print "P3"
    int i = 0;
    while (i < im->commentsCount) { // print all the comments
        printf("%s\n", im->comments[i]);
        i++;
    }

    printf("%d %d\n", im->cols, im->rows); // print number of rows and cols
    printf("%d\n", im->maxColor); // print the max color
    i = 0;
    while (i < (im->cols * im->rows)) { // print all the pixels
        printf("%d %d %d\n", im->rgb[i]->r, im->rgb[i]->g, im->rgb[i]->b);
        i++;
    }
}

// function to read a ppm struct to a file
// same procedure as showPPM function above
void writePPM(struct PPM *im, FILE *f) {
    fprintf(f, "%s\n", "P3");
    int i;
    for (i = 0; i < im->commentsCount; i++) {
        fprintf(f, "%s\n", im->comments[i]);
    }
    fprintf(f, "%d\n", im->cols);
    fprintf(f, "%d\n", im->rows);
    fprintf(f, "%d\n", im->maxColor);
    for (i = 0; i < (im->rows * im->cols); i++) {
        fprintf(f, "%d %d %d\n", im->rgb[i]->r, im->rgb[i]->g, im->rgb[i]->b);
    }
}

// function to convert an int to binary
char *intToBinary(int num, int bits) {
    int i, j;
    char *binary = malloc(bits+1);
    binary[0] = '\0'; // null terminator
    for (i = bits-1; i >= 0; i--) {
        j = num >> i; // shift num right i places and i 0s to the left (equivalent to dividing by 2)
        if (j & 1) // bitwise AND operation, an odd number will result in 1, an even num will result in zero
            strcat(binary, "1");
        else
            strcat(binary, "0");
    }
    return binary;
}

// function to convert a string to binary
char *stringToBinary(char *s, unsigned int mSize) {
    if (s == NULL) // if the string is NULL return 0
        return 0;
    int i, j, k;
    char c;
    char *binary = malloc(mSize * 8 + 1); // each char is 8 bits and + 1 at the end for string null terminator
    binary[0] = '\0'; // null terminator
    for (i = 0; i < mSize; i++) { // iterate through every char in the string
        c = s[i]; // the ith character
        for (j = 7; j >= 0; j--) {
            k = 1 << j; // shift 1 left j places and add j 0s to the right (128, 64, 32,...)
            if (c & k) // bitwise AND operation ( char c ascii value and value of k)
                strcat(binary, "1");
            else
                strcat(binary, "0");
        }
    }
    return binary;
}

// function to convert binary to decimal
int binaryToDecimal(int num) {
    int decimal = 0, base = 1, remainder; // start at base 1
    while (num > 0) { // iterate while num bigger than 0
        remainder = num % 10; // get the remainder
        decimal = decimal + (remainder * base); // decimal is equal to curr decimal plus rem * current base
        num = num / 10; // divide num by 10
        base *=  2; // double the base
    }
    return decimal;
}

// function to convert binary to string
char *binaryToString(char *binary) {
    int i = 0, j, k = 0, l = 0;
    int z = 0;
    char ascii;
    char *c;
    char *message = (char *) malloc(100);
    while (i < strlen(binary)) { // iterate 8 bits at a time till the end of the binary string
        c = (char *) malloc(100);
        for (j = i; j < l + 8; j++) { // get the next 8 bits (which is l + 8)
            c[z] = binary[i];
            i++;
            z++;
        }
        ascii = binaryToDecimal(atoi(c)); // convert the binary to decimal
        message[k] = ascii; // assign decimal value to the char array which gives the character
        k++;
        l = i; // set l to current position of i
        z = 0; // set z back to zero
    }
    return message;
}