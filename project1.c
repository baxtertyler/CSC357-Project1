#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#pragma pack(push, 1)

typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned int LONG;
typedef unsigned char BYTE;

/* struct declarations */
typedef struct tagBITMAPFILEHEADER
{
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
}fileHeader;

typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
}infoHeader;

/* function declarations */
BYTE get_red(BYTE*, float, float, int, int);
BYTE get_green(BYTE*, float, float, int, int);
BYTE get_blue(BYTE*, float, float, int, int);
void checkInput(int, char*[]);

int main(int argc, char *argv[]) {
    
    
    
    unsigned char* pad;

    /*          IMAGE 1 */
    FILE *image1; fileHeader fh1; infoHeader ih1; unsigned char* image1DATA;

    /*          IMAGE 2 */
    FILE *image2; fileHeader fh2; infoHeader ih2; unsigned char* image2DATA;

    /*          IMAGE 3 */
    FILE *image3; unsigned char* image3DATA;

    /*          OTHER VARS */
    /* float ratio = argv[4]*/
    float ratio;
    int i, j, row_size1, row_size2;

    checkInput(argc, argv);


    image1 = fopen(argv[1], "rb");
    image2 = fopen(argv[2], "rb");
    ratio = atof(argv[3]);
    if (ratio == 0.0) {
        printf("%s\n%s\n", "Ratio must be: 0 < ratio < 1", 
            "[program name] [imagefile1.bmp] [imagefile2.bmp] [ratio] [outputfile.bmp]"); 
        exit(1);
    } else if (ratio < 0 || ratio > 1) {
        printf("%s\n%s\n", "Ratio must be: 0 < ratio < 1", 
            "[program name] [imagefile1.bmp] [imagefile2.bmp] [ratio] [outputfile.bmp]"); 
        exit(1);
    }
    image3 = fopen(argv[4], "wb");


    /*  
            DO ALL PREP WORK FOR IMAGE 1
             - check to make sure file exists and is of the correct format
             - create image header and file header
             - allocate space for image data
             - find row size that allocates for padding
             - add the image data to the image1DATA pointer
    */
    if (image1 == NULL) {
        printf("%s\n%s\n", "image1 does not exist, expected: ", 
            "[program name] [imagefile1.bmp] [imagefile2.bmp] [ratio] [outputfile.bmp]"); 
        exit(1);
    }
    fread(&fh1.bfType, sizeof(WORD), 1, image1);
    if (fh1.bfType != 19778) {
        printf("%s\n%s\n", "image1 is not a .BMP file, expected: ", 
            "[program name] [imagefile1.bmp] [imagefile2.bmp] [ratio] [outputfile.bmp]"); 
        exit(1);
    }
    fread(&fh1.bfSize, sizeof(DWORD), 1, image1);
    fread(&fh1.bfReserved1, sizeof(WORD), 1, image1);
    fread(&fh1.bfReserved2, sizeof(WORD), 1, image1);
    fread(&fh1.bfOffBits, sizeof(DWORD), 1, image1);
    fread(&ih1.biSize, sizeof(DWORD), 1, image1);
    fread(&ih1.biWidth, sizeof(LONG), 1, image1);
    fread(&ih1.biHeight, sizeof(LONG), 1, image1);
    fread(&ih1.biPlanes, sizeof(WORD), 1, image1);
    fread(&ih1.biBitCount, sizeof(WORD), 1, image1);
    fread(&ih1.biCompression, sizeof(DWORD), 1, image1);
    fread(&ih1.biSizeImage, sizeof(DWORD), 1, image1);
    fread(&ih1.biXPelsPerMeter, sizeof(LONG), 1, image1);
    fread(&ih1.biYPelsPerMeter, sizeof(LONG), 1, image1);
    fread(&ih1.biClrUsed, sizeof(DWORD), 1, image1);
    fread(&ih1.biClrImportant, sizeof(DWORD), 1, image1);

    /* calculates the row size in bytes including padding */
    row_size1 = ((ih1.biWidth * ih1.biBitCount + 31) / 32) * 4;
    image1DATA = (unsigned char*)malloc(row_size1 * ih1.biWidth * 3 * sizeof(BYTE));
    if (image1DATA == NULL) {
        printf("%s", "MALLOC FAILURE");
        exit(1);
    }
    pad = (unsigned char*)malloc(4 * ih1.biHeight + 1);
    if (pad == NULL) {
        printf("%s", "MALLOC FAILURE");
        exit(1);
    }
    for (i = 0; i < ih1.biHeight * 3; i++) {
        for (j = 0; j < row_size1; j++) {
            if (j < (ih1.biWidth*3)) {
                fread(image1DATA + i * row_size1 + j, sizeof(BYTE), 1, image1);
            } else {
                fread(&pad, sizeof(BYTE), 1, image1);
            }
        }
    }
    




    /*  
            DO ALL PREP WORK FOR IMAGE 2
             - check to make sure file exists and is of the correct format
             - create image header and file header
             - allocate space for image data
             - find row size that allocates for padding
             - add the image data to the image2DATA pointer
    */
    if (image2 == NULL) {
        printf("%s\n%s\n", "image2 does not exist, expected: ", 
            "[program name] [imagefile1.bmp] [imagefile2.bmp] [ratio] [outputfile.bmp]"); 
        free(image1DATA);
        free(pad);
        exit(1);
    }
    fread(&fh2.bfType, sizeof(WORD), 1, image2);
    if (fh2.bfType != 19778) {
        printf("%s\n%s\n", "image1 is not a .BMP file, expected: ", 
            "[program name] [imagefile1.bmp] [imagefile2.bmp] [ratio] [outputfile.bmp]"); 
        free(image1DATA);
        free(pad);
        exit(1);
    }
    fread(&fh2.bfSize, sizeof(DWORD), 1, image2);
    fread(&fh2.bfReserved1, sizeof(WORD), 1, image2);
    fread(&fh2.bfReserved2, sizeof(WORD), 1, image2);
    fread(&fh2.bfOffBits, sizeof(DWORD), 1, image2);
    fread(&ih2.biSize, sizeof(DWORD), 1, image2);
    fread(&ih2.biWidth, sizeof(LONG), 1, image2);
    fread(&ih2.biHeight, sizeof(LONG), 1, image2);
    fread(&ih2.biPlanes, sizeof(WORD), 1, image2);
    fread(&ih2.biBitCount, sizeof(WORD), 1, image2);
    fread(&ih2.biCompression, sizeof(DWORD), 1, image2);
    fread(&ih2.biSizeImage, sizeof(DWORD), 1, image2);
    fread(&ih2.biXPelsPerMeter, sizeof(LONG), 1, image2);
    fread(&ih2.biYPelsPerMeter, sizeof(LONG), 1, image2);
    fread(&ih2.biClrUsed, sizeof(DWORD), 1, image2);
    fread(&ih2.biClrImportant, sizeof(DWORD), 1, image2);

    row_size2 = ((ih2.biWidth * ih2.biBitCount + 31) / 32) * 4;
    image2DATA = (unsigned char*)malloc(ih2.biHeight * 3 * row_size2 * sizeof(BYTE));
    if (image2DATA == NULL) {
        printf("%s", "MALLOC FAILURE");
        exit(1);
    }
    for (i = 0; i < ih2.biHeight * 3; i++) {
        for (j = 0; j < row_size2; j++) {
            if (j < (ih2.biWidth*3)) {
                fread(image2DATA + i * row_size2 + j, sizeof(BYTE), 1, image2);
            } else {
                fread(&pad, sizeof(BYTE), 1, image2);
            }    
        }
    }





    /*  
            DO ALL PREP WORK FOR IMAGE 3
             - create file
             - write in file from larger image
             - allocate space for image data
    */
    if (ih1.biWidth >= ih2.biWidth)  {
        fwrite(&fh1.bfType, sizeof(WORD), 1, image3);
        fwrite(&fh1.bfSize, sizeof(DWORD), 1, image3);
        fwrite(&fh1.bfReserved1, sizeof(WORD), 1, image3);
        fwrite(&fh1.bfReserved2, sizeof(WORD), 1, image3);
        fwrite(&fh1.bfOffBits, sizeof(DWORD), 1, image3);
        fwrite(&ih1.biSize, sizeof(DWORD), 1, image3);
        fwrite(&ih1.biWidth, sizeof(LONG), 1, image3);
        fwrite(&ih1.biHeight, sizeof(LONG), 1, image3);
        fwrite(&ih1.biPlanes, sizeof(WORD), 1, image3);
        fwrite(&ih1.biBitCount, sizeof(WORD), 1, image3);
        fwrite(&ih1.biCompression, sizeof(DWORD), 1, image3);
        fwrite(&ih1.biSizeImage, sizeof(DWORD), 1, image3);
        fwrite(&ih1.biXPelsPerMeter, sizeof(LONG), 1, image3);
        fwrite(&ih1.biYPelsPerMeter, sizeof(LONG), 1, image3);
        fwrite(&ih1.biClrUsed, sizeof(DWORD), 1, image3);
        fwrite(&ih1.biClrImportant, sizeof(DWORD), 1, image3);
        image3DATA = (unsigned char*)malloc(ih1.biHeight * 3 * row_size1 * sizeof(BYTE));
        if (image3DATA == NULL) {
            printf("%s", "MALLOC FAILURE");
            exit(1);
        }
    }
    else {
        fwrite(&fh2.bfType, sizeof(WORD), 1, image3);
        fwrite(&fh2.bfSize, sizeof(DWORD), 1, image3);
        fwrite(&fh2.bfReserved1, sizeof(WORD), 1, image3);
        fwrite(&fh2.bfReserved2, sizeof(WORD), 1, image3);
        fwrite(&fh2.bfOffBits, sizeof(DWORD), 1, image3);
        fwrite(&ih2.biSize, sizeof(DWORD), 1, image3);
        fwrite(&ih2.biWidth, sizeof(LONG), 1, image3);
        fwrite(&ih2.biHeight, sizeof(LONG), 1, image3);
        fwrite(&ih2.biPlanes, sizeof(WORD), 1, image3);
        fwrite(&ih2.biBitCount, sizeof(WORD), 1, image3);
        fwrite(&ih2.biCompression, sizeof(DWORD), 1, image3);
        fwrite(&ih2.biSizeImage, sizeof(DWORD), 1, image3);
        fwrite(&ih2.biXPelsPerMeter, sizeof(LONG), 1, image3);
        fwrite(&ih2.biYPelsPerMeter, sizeof(LONG), 1, image3);
        fwrite(&ih2.biClrUsed, sizeof(DWORD), 1, image3);
        fwrite(&ih2.biClrImportant, sizeof(DWORD), 1, image3);
        image3DATA = (unsigned char*)malloc(ih2.biHeight * 3 * row_size1 * sizeof(BYTE));
        if (image1DATA == NULL) {
            printf("%s", "MALLOC FAILURE");
            exit(1);
        }
    }
    
    



    /*
                PROCESS IMAGE DATA FOR NEW IMAGE
    */
    for (i = 0; i < (ih1.biWidth * ih1.biHeight * 3)+1; i+=3) {
        image3DATA[i] = (image1DATA[i] * ratio) + (image2DATA[i] * (1-ratio));
        image3DATA[i+1] = (image1DATA[i+1] * ratio) + (image2DATA[i+1] * (1-ratio));
        image3DATA[i+2] = (image1DATA[i+2] * ratio) + (image2DATA[i+2] * (1-ratio));
    
    }
    for (i = 0; i < ih1.biHeight; i++) {
        for (j = 0; j < row_size1; j++) {
            if (j < ih1.biWidth*3) {
                fwrite(image3DATA + i * row_size1 + j, sizeof(BYTE), 1, image3);
            }
            else {
                fwrite(&pad, sizeof(BYTE), 1, image3);
            }
        }
    }
    




    /*
                FINISH UP PROGRAM
                - free malloc
                - close files
                - return 0
    */
    fclose(image1);
    fclose(image2);
    fclose(image3);
    free(image1DATA);
    free(image2DATA);
    free(image3DATA);
    free((unsigned char*)pad);
    return 0;
}

void checkInput(int argc, char *argv[]) {
    if (argc != 5) {
        if (argc > 5) {
            printf("%s\n%s\n", "Too many arguments, expected", 
            "[program name] [imagefile1.bmp] [imagefile2.bmp] [ratio] [outputfile.bmp]");
        }
        else if (argc == 4) {
            printf("%s\n%s\n", "Missing outputfile, expected", 
            "[program name] [imagefile1.bmp] [imagefile2.bmp] [ratio] [outputfile.bmp]");
        }
        else if (argc == 3) {
            printf("%s\n%s\n", "Missing outputfile/ratio, expected", 
            "[program name] [imagefile1.bmp] [imagefile2.bmp] [ratio] [outputfile.bmp]");
        }
        else if (argc == 2) {
            printf("%s\n%s\n", "Missing outputfile/ratio/imagefile2, expected", 
            "[program name] [imagefile1.bmp] [imagefile2.bmp] [ratio] [outputfile.bmp]");
        }
        else if (argc == 1) {
            printf("%s\n%s\n", "Missing outputfile/ratio/imagefile2/imagefile1, expected", 
            "[program name] [imagefile1.bmp] [imagefile2.bmp] [ratio] [outputfile.bmp]");
        }
        exit(1);
    }
}
