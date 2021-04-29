# steganography-c

Steganography in C.

Steganography is the process of hiding a text message or an image in another image.

## Compiling

```
gcc steg.c common.c -o steg
gcc steg_lsb.c common.c -o steg_lsb
gcc steg_img.c common.c -o steg_img
```

## Running

1. Steg.c

This is the basic steganography file.  The main method requires 3 arguments: requires as a first argument ‘e’ or ‘d’ to either encode or decode, for encoding it needs two more arguments, the input file and output file, for decode it just needs the encoded file. The program starts by asking the user for a message to be hidden, then asks the user of the secret, then prints the encoded ppm file.![image](https://user-images.githubusercontent.com/29354102/116612433-96f04400-a948-11eb-8fa5-3ddc5bcf2c84.png)




