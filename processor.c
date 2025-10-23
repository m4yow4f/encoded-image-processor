#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct
{
    int width;
    int height;
    int grey_level;
} Metadata;

int validImage (char encoding[], Metadata *metadata);
void decompress (char encoding[], int (*result)[80], Metadata metadata);
void zoomIn(int image[][80], int (*result)[159], Metadata *metadata);
void smooth(int image[][159], int (*result)[159], Metadata metadata, int iterations);
void showImage(int image[][159], Metadata metadata);

/*The main fuction takes in an encoding and decodes each pixel and prints it*/
int main()
{
    char encoded_image[56009];

    Metadata image;
    Metadata *p_image = &image;

    if (!validImage(encoded_image, p_image))
    {
        fprintf(stderr, "Image is not encoded correctly. Please try again with a diferent file.");
        return 0;
    }
    
    int decoded_image[100][80];
    int (*p_d_image)[80] = decoded_image;
    
    decompress(encoded_image, p_d_image, image);

    int zoomed_image[199][159];
    int (*p_z_image)[159] = zoomed_image;
    zoomIn(decoded_image, p_z_image, p_image);

    int smoothed_image[199][159];
    int (*p_s_image)[159] = smoothed_image;
    smooth(zoomed_image, p_s_image, image, 3);

    showImage(smoothed_image,image);

    
    return 0;
}

/*This function checks some encoding is valid and saves width, height and grey-level of the image to be produced. 2 Arguments: encoding - this is the encoding to be checked, metadata - this is the struct where the information on the image is stored*/
int validImage(char encoding[], Metadata *metadata) 
{
    char spaces[4];

    scanf("%2d%1c%3d%1c%1d%1c",&metadata->width, &spaces[0], &metadata->height, &spaces[1], &metadata->grey_level, &spaces[2]);
    fgets(encoding, 56009,stdin);

    int pixels = metadata->height * metadata->width;
    int len_count;
    
    int i;
    int len_num_digits=0;
    for (i = 0; i < strlen(encoding); i+=(3+len_num_digits))
    {
        int len;
        int pixlvl;

        pixlvl = encoding[i] - 48;
        sscanf(encoding + 1 + i, "%4d", &len);

        if (pixlvl > (metadata->grey_level - 1) || pixlvl < 0)
        {
            return 0;
        }
        len_count += len;

        char buffer[4];
        sprintf(buffer, "%d", len);
        len_num_digits = strlen(buffer);
    }

    if (metadata->width <= 0 || metadata->width > 80 || metadata->height <=0 || metadata->height > 100 || metadata->grey_level < 2 || metadata->grey_level > 4 || spaces[0] != ' ' || spaces[1] != ' ' || spaces[2] != ' ' || len_count != pixels)
    {
        return 0;
    }
    
    return 1;
}

/*This function takes an encoding at translates it into an image in a 2d array. 3 arguments: encoding - this is the enocded image to be decoded, result - this is the pointer to the array where the image is stored, metadata - this is information on the image to produce the image*/
void decompress (char encoding[], int (*result)[80], Metadata metadata)
{
    int i;
    int len_num_digits = 0;
    int width = 0;
    int height = 0;
    for (i = 0; i < strlen(encoding); i+=(3+len_num_digits))
    {
        int len;
        sscanf(encoding + 1 + i, "%4d", &len);
        int ii;
        for (ii = 0; ii < len; ii++)
        {
            if (width == metadata.width)
            {
                height++;
                width = 0;
            }
            result[height][width] = encoding[i] - 48;
            width++;
        }
        
        char buffer[4];
        sprintf(buffer, "%d", len);
        len_num_digits = strlen(buffer);
    }
}
/*This function adds the extra pixels for the zoomed in image. 3 arguments: image - pointer to the original image array, result - pointer to array of zoomed in image, metadata- this is a pointer to the information on the image to produce the image */
void zoomIn(int image[][80], int (*result)[159], Metadata *metadata)
{
    /*Changing values with 1 to have 3*/
    int j;
    for(j=0; j < metadata->height; j++)
    {
        int i;
        for(i=0; i < metadata->width; i++)
        {
            if(image[j][i] == 1)
            {
                image[j][i] = 3;
            }
        }
    }
    
    metadata->width = (2 * metadata->width) - 1;
    metadata->height = (2 * metadata->height) - 1;
    metadata->grey_level = 4;

    /*Adding the extra pixels*/
    int jj;
    for(j=0, jj=0; j < metadata->height; j+=2,jj++)
    {
        int i;
        int ii;
        for(i=0, ii=0; i < metadata->width; i+=2,ii++)
        {
            result[j][i] = image[jj][ii];
            result[j][i+1] = 0;
        }

        for(i = 0; i < metadata->width;i++)
        {
            result[j+1][i] = 0;
        }
    }

    /*Calculating values for extra pixels*/
    for(j=0,jj=1; j < metadata->height; j+=2,jj+=2)
    {
        int i;
        for(i=0; i< metadata->width; i++)
        {
            if(i%2!=0)
            {
                result[j][i] = ((result[j][i-1]) + (result[j][i+1])) / 2;
            }
        }

        for(i = 0; i < metadata->width;i++)
        {
            if (i%2==0)
            {
                result[jj][i] = ( (result[jj-1][i]) + (result[jj+1][i]) ) / 2;
            }
            else
            {
                result[jj][i] = (((result[jj+1][i+1]) + (result[jj-1][i-1]) + (result[jj+1][i-1]) + (result[jj-1][i+1])) + 1 ) / 4;
            }
        }
    }
}

/*This function smoothes a zoomed image using an antialising formula. 4 Arguments: image - this is a pointer to the array of the zoomed in image that needs to be smoothed, results - this is a pointer to the array that holds the smoothed image, metadata - this is information on the image to produce the image, iterations - this is how many times the image should be smoothed*/
void smooth(int image[][159], int (*result)[159],Metadata metadata, int iterations)
{
    int iteration;
    for(iteration = 0; iteration < iterations; iteration++)
    {
        int j;
        int i;
        for(j=0; j < metadata.height; j++)
        {
            for(i=0; i < metadata.width; i++)
            {
                if (j == 0 || j == metadata.height-1 || i == 0 || i == metadata.width-1)
                {
                    result[j][i] = image[j][i];
                }
                else
                {
                    result[j][i] = ( ( (image[j-1][i-1]) + ((2)*(image[j][i-1])) + (image[j+1][i-1]) + ((2)*(image[j-1][i])) + ((4)*(image[j][i])) + ((2)*(image[j+1][i])) + (image[j-1][i+1]) + ((2)*(image[j][i+1])) + (image[j+1][i+1]) ) + 7) / 16;
                }
            }
        }

        for(j=0; j < metadata.width; j++)
        {
            for(i=0; i<metadata.width;i++)
            {
                image[j][i] = result[j][i];
            }
        }
    }
}

/*This function prints out an image array. 2 Arguments: image - the array that stores the image, metadata - this is information on the image to produce the image*/
void showImage(int image[][159], Metadata metadata)
{
    const char greyLevels[3][4] = {{'#',' ','\0', '\0'}, {'#', '.', ' ', '\0'}, {'#', ':', '.', ' '}};

    int j;
    for(j=0; j < metadata.height; j++)
    {
        int i;
        for(i=0; i < metadata.width; i++)
        {
            printf("%c", greyLevels[metadata.grey_level - 2][(image[j][i])]);
        }
        printf("%c",'\n');
    }
}
