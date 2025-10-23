
# Encoded Image Processor

Program to convert and process a run-length-encoded (RLE) image.





## About

When given an image in a RLE format it is necessary to be able to decompress and view it. 

This program follows the encoding format to decompress it into a matrix then incoporates an anti-aliasing formula to reduce the pixelated edges of the image.

## Image Encoding Overview

The format to encode an image is seen below:

`<width> <height> <grey-levels> <pix1> <len1> <pix2> <len2>...`

- <> represents an integer value
- Whitespaces seperate the values
- <width> is the image width in pixels
- <height> is the image height in pixels
- <grey-levels> is the number of grey levels in the image (eg, 2 for black and white).
- Image pixels can only take values between 0 and grey-levels-1
- <pixN> is the grey level of the next pixel
- <lenN> is the number of times the pixel is repeated

If there are two grey levels: 
- Space ' ' for pixel value grey-level-1
- '#' for value 0 (black)

If there are three grey levels:
- Dot '.' for value 1

If there are four grey levels:
- Colon ':' for 1
- Dot '.' for 2

*"panda_encoding.run" is an example encoded image that can be used with the program*
## Run Locally

Clone the project

~~~bash
git clone https://github.com/m4yow4f/encoded-image-processor.git
~~~

Install the GCC compiler

**On macOS/Linux:**
~~~bash
sudo apt install build-essential       # Ubuntu/Debian

or

brew install gcc                       # macOS (Homebrew)
~~~

**On Windows:**

Follow instructions here - https://code.visualstudio.com/docs/cpp/config-mingw

Create an executable

**On macOS/Linux:**
~~~bash
gcc processor.c -o processor
~~~

**On Windows:**
~~~bash
gcc processor.c -o processor.exe
~~~

Run the program

~~~bash
./processor
~~~
