# steganography-c

Steganography in C for PPM files.

Steganography is the process of hiding a text message or an image in another image.

## Compiling

```
gcc steg.c common.c -o steg
gcc steg_lsb.c common.c -o steg_lsb
gcc steg_img.c common.c -o steg_img
```

## Running

1. steg.c

This is the basic steganography file.  The main method requires 3 arguments: requires as a first argument ‘e’ or ‘d’ to either encode or decode, for encoding it needs two more arguments, the input file and output file, for decode it just needs the encoded file. The program starts by asking the user for a message to be hidden, then asks the user of the secret, then prints the encoded ppm file.

```
./steg e snail.ppm output.ppm
./steg d output.ppm
```

2. steg_lsb.c


Similar to steg.c but includes two extra features:
 * Parameterized least significant bits hiding of message.
 * Option to use a string as the secret

3. steg_img.c 

Has the same functionalities as steg.c, but instead of hiding
a message, it hides an image. To encode, the user needs to enter 3  arguments after ‘e’, container image, secret
image and output image. To decode, the user need to enter 2 arguments after ‘d’, encoded image, and output image (decoded image).

```
./stegi e blackbuck.ppm snail.ppm encoded_image.ppm
./steg d encoded_image.ppm decoded_image.ppm
```


