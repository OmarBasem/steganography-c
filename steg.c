/*
 *  This is a steganography file in C for PPM files.
 *  The main method requires 3 arguments: requires as a first argument ‘e’ or ‘d’ to either encode or decode,
 *  for encoding it needs two more arguments, the input file and output file, for decode it just needs the encoded file.
 *  The program starts by asking the user for a message to be hidden, then asks the user of the secret, then prints
 *  the encoded ppm file.
 * @author Omar Basem
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "common.h"


// function to encode the bits, used inside the encode method
void encodeBits(struct PPM * im, char * binary, int x, int l,  int * isUsed) { // x: number of pixels, l: number of bits
    int i, j=-1;
    int r, g, b;
    char *rBinary;
    char *gBinary;
    char *bBinary;
    int randomPixel;
    for (i = 0; i < x; i++) { // iterate for the number of pixels needed to hide the message binary
        randomPixel = 0 + rand() % ((im->rows * im->cols)); // choose a random pixel no greater than the pixels count
        while (isUsed[randomPixel] == 1) { // if pixels is used, choose another pixel
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
        rBinary[31] = binary[++j]; // make the last bit of r binary = to the first bit of the message binary
        im->rgb[randomPixel]->r = binaryToDecimal(atoi(rBinary)); // covert back to decimal and assign
        if (j != l - 1) { // if j = (l-1), then the whole binary string has already been encoded
            gBinary[31] = binary[++j];
            im->rgb[randomPixel]->g = binaryToDecimal(atoi(gBinary));
        }
        if (j != l - 1) {
            bBinary[31] = binary[++j];
            im->rgb[randomPixel]->b = binaryToDecimal(atoi(bBinary));
        }
    }
}

// function to decode the bits, used inside the decode method
// same procedure as encodeBits above
void decodeBits(struct PPM * im, char * binary, int x, int l,  int * isUsed) {
    int i, j=-1;
    int r, g, b;
    char *rBinary;
    char *gBinary;
    char *bBinary;
    int randomPixel;
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
        binary[++j] = rBinary[31];
        if (j != l - 1) {
            binary[++j] = gBinary[31];
        }
        if (j != l - 1) {
            binary[++j] = bBinary[31];
        }

    }
}

// function to encode a message into ppm struct
struct PPM *encode(struct PPM *im, char *message, unsigned int mSize, unsigned int secret) {
    char *mBinary; // var to hold the message in binary
    int *isUsed = (int *) malloc(sizeof(int) * (im -> rows * im -> cols)); // array to keep record of the used pixels
    mBinary = stringToBinary(message, mSize); // convert the message to binary
    int l = strlen(mBinary); // length of the binary string
    double x = ceil(l / 3.0);  // the number of pixels needed to hide the binary string
    srand(secret); // seed using secret
    char *mSizeBinary = intToBinary(mSize, 32); // convert mSize (length of message) to binary
    encodeBits(im, mSizeBinary, 11, 32, isUsed); // encode the msg length binary string, 11 means 11 pixels needed to hide 32 bits, 32 is number of bits
    encodeBits(im, mBinary, x, l,  isUsed); // encode the message, x = pixels, l = number of bits
    return im;
}

// function to decode a message from a ppm
char *decode(struct PPM *im, unsigned int secret) {
    char *mSizeBinary = (char *) malloc(33); // allocate space for binary string of the message length
    int *isUsed = (int *) malloc(sizeof(int) * (im -> rows * im -> cols));
    srand(secret);
    decodeBits(im, mSizeBinary, 11, 32,  isUsed); //decode the message length, 3: number of pixels, 8: number of bits
    int mSize = binaryToDecimal(atoi(mSizeBinary));
    char *mBinary = (char *) malloc(mSize * 8 + 1); // allocate space for the message in binary
    double x = ceil(mSize * 8.0 / 3.0); // number of pixels that was encoded
    int l = mSize * 8; // number of bits
    decodeBits(im, mBinary, x, l,  isUsed); // decode the message
    return binaryToString(mBinary);
}


int main(int argc, char **argv) {
    unsigned int secret;
    FILE *fin, *fout;
    fin = fopen(argv[2], "r");
    if (fin == NULL) {
        printf("copy: can't open %s\n", argv[1]);
    }
    if (strcmp(argv[1], "e") == 0) { // encode
        fout = fopen(argv[3], "w");
        char *message = (char *) malloc(1000);
        unsigned int mSize;
        printf("Enter a message:\n"); // get message from user
        scanf(" %[^\t\n]s", message);
        mSize = strlen(message);
        printf("Enter secret:\n"); // get secret from user
        scanf(" %d", &secret);
        struct PPM *im = getPPM(fin); // read ppm file into ppm struct
        struct PPM *encodedIm = (struct PPM *) malloc(sizeof(struct PPM)); // allocate space for output ppm
        encodedIm = encode(im, message, mSize, secret); // encode
        printf("The encoded ppm file:\n");
        showPPM(encodedIm); // print the encoded ppm
        writePPM(encodedIm, fout); // write the encoded ppm to file
    } else if (strcmp(argv[1], "d") == 0) { // decode
        struct PPM * encodedIm = getPPM(fin); // read ppm file into struct
        printf("Enter secret:\n"); // get secret from user
        scanf("%d", &secret);
        printf("decoded message:\n%s\n", decode(encodedIm, secret)); // print the message
    } else {
        printf("Unrecognized arguments\n");
    }

    return 0;
}