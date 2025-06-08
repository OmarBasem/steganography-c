/*
 * This is a steganography file in C for PPM files, has the same functionalities as steg.c, but instead of hiding
 * a message, it hides an image. To encode, the user needs to enter 3  arguments after ‘e’, container image, secret
 * image and output image. To decode, the user need to enter 2 arguments after ‘d’, encoded image, and output
 * image (decoded image).
 * @author Omar Basem
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "common.h"


void encodeCR(struct PPM * containerIm, char * binary, int x, int * isUsed, double msb, int cr) {
    int i, j=-1, k, r, g, b, bitPos, randomPixel;
    char *rBinary, *gBinary, *bBinary;
    for (i = 0; i < x; i++) { // iterate for the total number of the secret image
        randomPixel = 0 + rand() % ((containerIm->rows * containerIm->cols)); // generate random picel
        while (isUsed[randomPixel] == 1) { // check if it has been used
            randomPixel = 0 + rand() % ((containerIm->rows * containerIm->cols));
        }
        isUsed[randomPixel] = 1; // mark pixel as used
        printf("random pixel %d\n", randomPixel);
        // get r g b values of container Image and secret image and convert to binary
        r = containerIm->rgb[randomPixel]->r;
        g = containerIm->rgb[randomPixel]->g;
        b = containerIm->rgb[randomPixel]->b;
        rBinary = intToBinary(r, 8);
        gBinary = intToBinary(g, 8);
        bBinary = intToBinary(b, 8);
        binary = intToBinary(cr, 32);
        bitPos = 8;
        for (k = 0; k < msb; k++) { // iterate for the number of most sig. bits provided
            if (j != 31) {
                rBinary[--bitPos] = binary[++j];
            }// encode secret image bits into the container image
        }
        containerIm->rgb[randomPixel]->r = binaryToDecimal(atoi(rBinary)); // back to decimal and assign
        // same for g and b values
        bitPos = 8;
        for (k = 0; k < msb; k++) {
            if (j != 31) {
                gBinary[--bitPos] = binary[++j];
            }
        }
        containerIm->rgb[randomPixel]->g = binaryToDecimal(atoi(gBinary));
        bitPos = 8;
        for (k = 0; k < msb; k++) {
            if (j != 31) {
                bBinary[--bitPos] = binary[++j];
            }
        }
        containerIm->rgb[randomPixel]->b = binaryToDecimal(atoi(bBinary));
    }
}

void decodeCR(struct PPM * encodedIm, char * binary, int x, int * isUsed, double msb) {
    int i, j=-1, k, r, g, b, bitPos, randomPixel;
    char *rBinary, *gBinary, *bBinary;
    for (i = 0; i < x; i++) { // iterate for the total number of the secret image
        randomPixel = 0 + rand() % ((encodedIm->rows * encodedIm->cols)); // generate random picel
        while (isUsed[randomPixel] == 1) { // check if it has been used
            randomPixel = 0 + rand() % ((encodedIm->rows * encodedIm->cols));
        }
        isUsed[randomPixel] = 1; // mark pixel as used
        r = encodedIm->rgb[randomPixel]->r;
        g = encodedIm->rgb[randomPixel]->g;
        b = encodedIm->rgb[randomPixel]->b;
        rBinary = intToBinary(r, 8);
        gBinary = intToBinary(g, 8);
        bBinary = intToBinary(b, 8);
        bitPos = 8;
        for (k = 0; k < msb; k++) { // iterate for the number of most sig. bits provided
            if (j != 31) {
                binary[++j] = rBinary[--bitPos];
            }
        }
        bitPos = 8;
        for (k = 0; k < msb; k++) {
            if (j != 31) {
                binary[++j] = gBinary[--bitPos];
            }
        }
        bitPos = 8;
        for (k = 0; k < msb; k++) {
            if (j != 31) {
                binary[++j] = bBinary[--bitPos];
            }
        }
    }
}

// method to encode image into another image
struct PPM *encode(struct PPM *containerIm, struct PPM *secretIm, unsigned int secret, double msb) {
    int *isUsed = (int *) malloc(sizeof(int) * (containerIm->rows * containerIm->cols)); // array to keep track of used pixels
    srand(secret);
    int i, j, k, r, g, b, rS, gS, bS;
    char *rBinary, *gBinary, *bBinary, *rBinaryS, *bBinaryS, *gBinaryS, *cols, * rows;
    int randomPixel;
    int bitPos;
    int x = ceil(32.0/(3.0*msb));
    encodeCR(containerIm, cols, x, isUsed, msb, secretIm -> cols); // encode cols
    encodeCR(containerIm, rows, x, isUsed, msb, secretIm -> rows); // encode rows
    // encode pixels
    for (i = 0; i < (secretIm->rows * secretIm->cols); i++) { // iterate for the total number of the secret image
        randomPixel = 0 + rand() % ((containerIm->rows * containerIm->cols)); // generate random picel
        while (isUsed[randomPixel] == 1) { // check if it has been used
            randomPixel = 0 + rand() % ((containerIm->rows * containerIm->cols));
        }
        isUsed[randomPixel] = 1; // mark pixel as used
        // get r g b values of container Image and secret image and convert to binary
        r = containerIm->rgb[randomPixel]->r;
        g = containerIm->rgb[randomPixel]->g;
        b = containerIm->rgb[randomPixel]->b;
        rS = secretIm->rgb[i]->r;
        gS = secretIm->rgb[i]->g;
        bS = secretIm->rgb[i]->b;
        rBinary = intToBinary(r,8);
        gBinary = intToBinary(g,8);
        bBinary = intToBinary(b,8);
        rBinaryS = intToBinary(rS,8);
        gBinaryS = intToBinary(gS,8);
        bBinaryS = intToBinary(bS,8);
        bitPos = 8, j = -1;
        for (k = 0; k < msb; k++) { // iterate for the number of most sig. bits provided
            rBinary[--bitPos] = rBinaryS[++j]; // encode secret image bits into the container image
        }
        containerIm->rgb[randomPixel]->r = binaryToDecimal(atoi(rBinary)); // back to decimal and assign
        // same for g and b values
        bitPos = 8, j = -1;
        for (k = 0; k < msb; k++) {
            gBinary[--bitPos] = gBinaryS[++j];
        }
        containerIm->rgb[randomPixel]->g = binaryToDecimal(atoi(gBinary));
        bitPos = 8, j = -1;
        for (k = 0; k < msb; k++) {
            bBinary[--bitPos] = bBinaryS[++j];
        }
        containerIm->rgb[randomPixel]->b = binaryToDecimal(atoi(bBinary));
    }
    return containerIm; // return the container image with the secret image encoded in it
}

// method to decode an image from an image
void decode(struct PPM *encodedIm, struct PPM *decodedIm, unsigned int secret, double msb) {
    int *isUsed = (int *) malloc(sizeof(int) * (encodedIm->rows * encodedIm->cols));
    srand(secret);
    int i, j=-1, k;
    int r, g, b;
    char *rBinary, *gBinary, *bBinary; // binary values of encoded image
    // allocate space for binary values of decoded image
    char *rB = (char *) malloc(9);
    char *gB = (char *) malloc(9);
    char *bB = (char *) malloc(9);
    char *rows = (char *) malloc(33);
    char *cols = (char *) malloc(33);
    int rowsNum, colsNum;
    int randomPixel;
    int bitPos;
    int x = ceil(32.0/(3.0*msb));
    decodeCR(encodedIm, cols, x, isUsed, msb); // decode cols
    decodeCR(encodedIm, rows, x, isUsed, msb); // decode rows
    colsNum = binaryToDecimal(atoi(cols));
    rowsNum = binaryToDecimal(atoi(rows));
    decodedIm -> rows = rowsNum;
    decodedIm -> cols = colsNum;
    // decode pixels
    for (i = 0; i < (rowsNum * colsNum); i++) {
        randomPixel = 0 + rand() % ((encodedIm->rows * encodedIm->cols));
        while (isUsed[randomPixel] == 1) {
            randomPixel = 0 + rand() % ((encodedIm->rows * encodedIm->cols));
        }
        isUsed[randomPixel] = 1;
        r = encodedIm->rgb[randomPixel]->r;
        g = encodedIm->rgb[randomPixel]->g;
        b = encodedIm->rgb[randomPixel]->b;
        rBinary = intToBinary(r,8);
        gBinary = intToBinary(g,8);
        bBinary = intToBinary(b,8);
        bitPos = 8, j = -1;
        for (k = 0; k < msb; k++) { // decode
            rB[++j] = rBinary[--bitPos];
        }
        for (k=0; k<8-msb; k++) // put the rest of bits till 8 as zero
            rB[++j] = '0';
        decodedIm->rgb[i]->r = binaryToDecimal(atoi(rB));
        bitPos = 8, j = -1;
        for (k = 0; k < msb; k++) {
            gB[++j] = gBinary[--bitPos];
        }
        for (k=0; k<8-msb; k++)
            gB[++j] = '0';
        decodedIm->rgb[i]->g = binaryToDecimal(atoi(gB));
        bitPos = 8, j = -1;
        for (k = 0; k < msb; k++) {
            bB[++j] = bBinary[--bitPos];
        }
        for (k=0; k<8-msb; k++)
            bB[++j] = '0';
        decodedIm->rgb[i]->b = binaryToDecimal(atoi(bB));
    }
}


int main(int argc, char **argv) {
    unsigned int secret;
    double msb;
    FILE *finContainer, *fin2, *fout;
    finContainer = fopen(argv[2], "r");
    fin2 = fopen(argv[2], "r");
    if (finContainer == NULL) {
        printf("can't open %s\n", argv[2]);
    }
    if (strcmp(argv[1], "e") == 0) { // encode
        FILE *finSecret;
        finSecret = fopen(argv[3], "r");
        if (finSecret == NULL) {
            printf("can't open %s\n", argv[3]);
        }
        fout = fopen(argv[4], "w");
        struct PPM *contIm = getPPM(finContainer); // read container ppm file into struct
        struct PPM *secretIm = getPPM(finSecret); // read secret ppm file into struct
        if (secretIm -> rows * secretIm -> cols > contIm -> cols * contIm -> rows) {
            printf("secret image is bigger than container image\n");
        } else {
            printf("Enter secret PIN (number):\n"); // get secret from user
            scanf("%d", &secret);
            printf("Enter the most significant bits count to hide between 2 and 7 of the secret image:\n"); // get the most sig. bits to hide from user
            scanf("%lf", &msb);
            struct PPM *encodedIm = (struct PPM *) malloc(sizeof(struct PPM)); // allocate space for new struct
            encodedIm = encode(contIm, secretIm, secret, msb); // call encode method
            printf("The encoded ppm file:\n"); // print the encode file
            showPPM(encodedIm);
            writePPM(encodedIm, fout); // write to file the encoded ppm
        }
    } else if (strcmp(argv[1], "d") == 0) { // decode
        fout = fopen(argv[3], "w");
        struct PPM *encodedIm = getPPM(finContainer); // read encoded ppm file into ppm struct
        fseek(finContainer, 0, SEEK_SET);
        struct PPM *decodedIm = getPPM(finContainer); // write encoded ppm as decoded ppm
        printf("Enter secret PIN (number):\n"); // get secret from user
        scanf("%d", &secret);
        printf("Enter the most significant bits count the was hidden between 2 and 7 of the secret image:\n"); // get msb from user
        scanf("%lf", &msb);
        decode(encodedIm, decodedIm, secret, msb); // decode
        printf("The decoded ppm file:\n"); // print the decode ppm
        showPPM(decodedIm);
        writePPM(decodedIm, fout); // write to output file
    } else {
        printf("Unrecognized arguments\n");
    }

    return 0;
}
