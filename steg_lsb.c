/*
 * This is a steganography file in C for PPM files, similar to steg.c but includes two extra features:
 * 1) Parameterized least significant bits hiding of message.
 * 2) Option to use a string as the secret
 *
 * @author Omar Basem
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "common.h"


// function to encode the bits, used inside the encode method
void encodeBits(struct PPM * im, char * binary, int x, int l,  int * isUsed, double lsb) { // x: number of pixels, l: number of bits
    int i, j=-1, k;
    int r, g, b;
    char *rBinary;
    char *gBinary;
    char *bBinary;
    int randomPixel;
    int bitPos;
    for (i = 0; i < x; i++) { // iterate for the number of pixels needed to hide the message binary
        randomPixel = 0 + rand() % ((im->rows * im->cols)); // choose a random pixel no greater than the pixels count
        while (isUsed[randomPixel] == 1) { // if pixels are used, choose another pixel
            randomPixel = 0 + rand() % ((im->rows * im->cols));
        }
        isUsed[randomPixel] = 1; // mark that pixel as used
        // get binary values of the r, g, b of that pixel
        r = im->rgb[randomPixel]->r;
        g = im->rgb[randomPixel]->g;
        b = im->rgb[randomPixel]->b;
        rBinary = intToBinary(r, 32);
        gBinary = intToBinary(g, 32);
        bBinary = intToBinary(b, 32);
        bitPos = 32; // start at position 8 (index 7) for the next color value
        for (k = 0; k < ceil(lsb/3); k++) { // ex: ceil (4/3) = 2
            if (j != l - 1) // if j = l-1 then all the bits have already been encoded
                rBinary[--bitPos] = binary[++j];
        }
        im->rgb[randomPixel]->r = binaryToDecimal(atoi(rBinary));
        bitPos = 32;
        for (k = 0; k < round(lsb/3); k++) { // ex: round (4/3) = 1, round(5/3)=2
            if (j != l - 1)
                gBinary[--bitPos] = binary[++j];
        }
        im->rgb[randomPixel]->g = binaryToDecimal(atoi(gBinary));
        bitPos = 32;
        for (k = 0; k < floor(lsb/3); k++) { // // ex: floor(4/3) = 1, floor(5/3)=1, floor(6/3)=2
            if (j != l - 1)
                bBinary[--bitPos] = binary[++j];
        }
        im->rgb[randomPixel]->b = binaryToDecimal(atoi(bBinary));
    }
}

// function to decode the bits, used inside the decode method
// same procedure as encodeBits above
void decodeBits(struct PPM *im, char *binary, int x, int l, int *isUsed, double lsb) {
    int i, j = -1, k;
    int r, g, b;
    char *rBinary;
    char *gBinary;
    char *bBinary;
    int randomPixel;
    int bitPos;
    for (i = 0; i < x; i++) {
        randomPixel = 0 + rand() % ((im->rows * im->cols));
        while (isUsed[randomPixel] == 1) {
            randomPixel = 0 + rand() % ((im->rows * im->cols));
        }
        isUsed[randomPixel] = 1;
        r = im->rgb[randomPixel]->r;
        g = im->rgb[randomPixel]->g;
        b = im->rgb[randomPixel]->b;
        rBinary = intToBinary(r, 32);
        gBinary = intToBinary(g, 32);
        bBinary = intToBinary(b, 32);
        bitPos = 32;
        for (k = 0; k < ceil(lsb/3); k++) {
            if (j != l - 1)
                binary[++j] = rBinary[--bitPos];
        }
        bitPos = 32;
        for (k = 0; k < round(lsb/3); k++) {
            if (j != l - 1)
                binary[++j] = gBinary[--bitPos];
        }
        bitPos = 32;
        for (k = 0; k < floor(lsb/3); k++) {
            if (j != l - 1)
                binary[++j] = bBinary[--bitPos];
        }
    }
}

// function to encode a message into ppm struct
struct PPM *encode(struct PPM *im, char *message, unsigned int mSize, unsigned int secret, double lsb) {
    char *mBinary; // var to hold the message in binary
    int *isUsed = (int *) malloc(sizeof(int) * (im -> rows * im -> cols)); // array to keep record of the used pixels
    mBinary = stringToBinary(message, mSize); // convert the message to binary
    int l = strlen(mBinary); // length of the binary string
    double x = ceil(l / lsb);  // the number of pixels needed to hide the binary string
    srand(secret); // seed using secret
    char *mSizeBinary = intToBinary(mSize, 32); // convert mSize (length of message) to binary
    encodeBits(im, mSizeBinary, ceil(32/lsb), 32, isUsed, lsb); // encode the msg length binary string, ceil(8/lsb) pixels needed, 8 is number of bits
    encodeBits(im, mBinary, x, l,  isUsed, lsb); // encode the message, x = pixels, l = number of bits
    return im;
}

// function to decode a message from a ppm
char *decode(struct PPM *im, unsigned int secret, double lsb) {
    char *mSizeBinary = (char *) malloc(33); // allocate space for binary string of the message length
    int *isUsed = (int *) malloc(sizeof(int) * (im -> rows * im -> cols));
    srand(secret);
    decodeBits(im, mSizeBinary, ceil(32/lsb), 32,  isUsed, lsb); //decode the message length, ceil(8/lsb): number of pixels, 8: number of bits
    int mSize = binaryToDecimal(atoi(mSizeBinary));
    char *mBinary = (char *) malloc(mSize * 8 + 1); // allocate space for the message in binary
    double x = ceil(mSize * 8.0 / lsb); // number of pixels that was encoded
    int l = mSize * 8; // number of bits
    decodeBits(im, mBinary, x, l,  isUsed, lsb); // decode the message
    return binaryToString(mBinary);
}



int main(int argc, char **argv) {
    char secretType;
    double lsb;
    unsigned int secret = 0;
    FILE *fin, *fout;
    fin = fopen(argv[2], "r");
    if (fin == NULL) {
        printf("copy: can't open %s\n", argv[1]);
    }
    if (strcmp(argv[1], "e") == 0) { // encode
        fout = fopen(argv[3], "w");
        char *message = (char *) malloc(1000); // allocate space for message
        unsigned int mSize;
        printf("Enter message:\n"); // get message from user
        scanf(" %[^\t\n]s", message);
        mSize = strlen(message);
        printf("Enter i for int secret, Enter s for String secret:\n"); // get secret type from user
        scanf(" %c", &secretType);
        printf("Enter the secret:\n"); // get secret from user
        if (secretType == 'i') {
            scanf(" %d", &secret);
        } else {
            int sum=0;
            char * secretString = (char *) malloc(100);
            scanf(" %[^\t\n]s", secretString);
            for (int i =0; i<strlen(secretString); i++) {
                secret += secretString[i];
            }
        }
        printf("Enter the lowest significant bits count to change between 1 and 21:\n"); // get number of least sig. bits
        scanf("%lf", &lsb);
        struct PPM *im = getPPM(fin); // read ppm file into struct
        struct PPM *encodedIm = (struct PPM *) malloc(sizeof(struct PPM)); // allocate space for new struct
        encodedIm = encode(im, message, mSize, secret, lsb); // encode method
        printf("The encoded ppm file:\n");
        showPPM(encodedIm); // print encoded ppm
        writePPM(encodedIm, fout); // write encoded ppm into output file
    } else if (strcmp(argv[1], "d") == 0) { // decode
        struct PPM *encodedIm = getPPM(fin); // read ppm file into struct
        printf("Enter i for int secret, Enter s for String secret:\n"); // get secret type from user
        scanf(" %c", &secretType);
        printf("Enter the secret:\n"); // get secret from user
        if (secretType == 'i') {
            scanf(" %d", &secret);
        }
        else {
            int sum=0;
            char * secretString = (char *) malloc(100);
            scanf(" %[^\t\n]s", secretString);
            for (int i =0; i<strlen(secretString); i++) {
                secret += secretString[i];
            }
        }
        printf("Enter the lowest significant bits count that was changed:\n"); // get number of least sig. bit from user
        scanf("%lf", &lsb);
        printf("decoded message:\n%s\n", decode(encodedIm, secret, lsb)); // print the message
    } else {
        printf("Unrecognized arguments\n");
    }

    return 0;
}
