# Steganography in C for PPM files.

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

# Example input & output

| <img src="https://github.com/user-attachments/assets/c2832cf9-d1a3-48af-98c5-68ef733534ae" width="160"/><br><b>Img A - Container Image</b> | <img src="https://github.com/user-attachments/assets/beed5bb1-58db-4034-97b2-b8cf8fc4a837" width="160"/><br><b>Img B - Image to be encoded</b> |
| :---: | :---: |
| <img src="https://github.com/user-attachments/assets/9624025e-3897-45d8-a2cd-a446594473ce" width="160"/><br><b>Img C - Output image after encoding Img B into Img A using 2 bits of the RGB values. Looks almost identical to the original image.</b> | <img src="https://github.com/user-attachments/assets/157258fe-ff47-40d1-ac10-27dc23c579a6" width="160"/><br><b>Img D - Output image after decoding Img C. Some quality has been lost.</b> |
| <img src="https://github.com/user-attachments/assets/1ebad34b-6156-4505-89f2-24053a7c72aa" width="160"/><br><b>Img E - Output image after encoding Img B into Img A using 7 bits of the RGB values. Alteration can be seen.</b> | <img src="https://github.com/user-attachments/assets/ac7a701b-69bd-42be-93fd-c9c3410cb444" width="160"/><br><b>Img F - Output image after decoding Img E. Looks almost identical to the original image.</b> |


