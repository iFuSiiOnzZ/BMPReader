﻿#include <stdio.h>
#include "./BMPReader.h"

#define BMP_BRIGHTNESS  (1 << 0)
#define BMP_SHOWHEADER  (1 << 1)
#define BMP_TONEGATIVE  (1 << 2)
#define BMP_RESIZE      (1 << 3)
#define BMP_TOGRAY      (1 << 4)
#define BMP_OUTPUT      (1 << 5)
#define BMP_ANGLE       (1 << 6)
#define BMP_SOBEL       (1 << 7)
#define BMP_BLUR        (1 << 8)
#define BMP_GAMMA       (1 << 9)

#define SET_BIT(var, bit)   ((var) |= (bit))
#define IS_SET(var, bit)    ((var) & (bit))

#define REMOVE_BIT(var, bit)    ((var) &= ~(bit))
#define TOGLE_BIT(var, bit)     ((var) ^= (bit))

typedef struct args_t
{
    unsigned int whatToDo;
    std::string fInputPath;
    std::string fOutputPath;
} args_t;

void help(void)
{
    printf("programName -i imputFile.bmp -o outFile.bmp [-rotate floatAngle] [-toBlur intPasses intKSize] [-toSobel floatThreshold] [-toGray] [-toNegative] [-showHeader] [-addBrightness intValue] [-resize intWidth intHeight] [-gameCorrection floatGamma] [-help]\n\n");
    printf("\t [-help] - Shows what you are reading\n");

    printf("\t [-toSobel] - Sobel filter for edge detection\n");
    printf("\t\t floatThreshold: if it is 0 (zero) output fill not be filtered else it will use the value as a threshold\n");

    printf("\t [-toBlur] - Gausian Blur using a 3x3 kernel\n");
    printf("\t\t intPasses: Number of passes\n");
    printf("\t\t intKSize: Kernel size\n");

    printf("\t [-toGray] - Convert the image to gray scale\n");

    printf("\t [-rotate] - Rotate the image by a given angle (canvas no resized)\n");
    printf("\t\t floatAngle: rotation angle in degrees\n");

    printf("\t [-resize] - Rescale the image to the new size\n");
    printf("\t\t intHeight: height of the new image\n");
    printf("\t\t intWidth: width of the new image\n");

    printf("\t [-toNegative] - Convert the image to negative\n");
    printf("\t [-showHeader] - Shows the image header information\n");

    printf("\t [-addBrightness] - Change the brightness of the image\n");
    printf("\t\t intValue: increases the brightness by the input value\n");

    printf("\t [-gammaCorrection] - Gamma correction\n");
    printf("\t\t float: Codificate the luminance of the image\n");
}

int main(int argc, char *argv[])
{
    args_t args;
    args.whatToDo = 0;

    int brightness = 0;
    int sx = 0, sy = 0;

    int blur = 1;
    int ks = 3;

    float sobel = 0.0f;
    float angle = 0.0f;

    float gamma = 2.2f;

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-i")) { args.fInputPath.assign(argv[i + 1]); }
        else if (!strcmp(argv[i], "-o")) { SET_BIT(args.whatToDo, BMP_OUTPUT); args.fOutputPath.assign(argv[i + 1]); }

        else if (!strcmp(argv[i], "-toSobel")) { SET_BIT(args.whatToDo, BMP_SOBEL); SET_BIT(args.whatToDo, BMP_TOGRAY); sobel = (float)atof(argv[i + 1]); }
        else if (!strcmp(argv[i], "-toBlur")) { SET_BIT(args.whatToDo, BMP_BLUR); blur = atoi(argv[i + 1]); ks = atoi(argv[i + 2]); }

        else if (!strcmp(argv[i], "-help")) { help(); exit(EXIT_SUCCESS); }
        else if (!strcmp(argv[i], "-showHeader")) { SET_BIT(args.whatToDo, BMP_SHOWHEADER); }

        else if (!strcmp(argv[i], "-toGray")) { SET_BIT(args.whatToDo, BMP_TOGRAY); }
        else if (!strcmp(argv[i], "-toNegative")) { SET_BIT(args.whatToDo, BMP_TONEGATIVE); }
        else if (!strcmp(argv[i], "-addBrightness")) { SET_BIT(args.whatToDo, BMP_BRIGHTNESS); brightness = atoi(argv[i + 1]); }

        else if (!strcmp(argv[i], "-rotate")) { SET_BIT(args.whatToDo, BMP_ANGLE); angle = (float)atof(argv[i + 1]); }
        else if (!strcmp(argv[i], "-resize")) { SET_BIT(args.whatToDo, BMP_RESIZE); sx = atoi(argv[i + 1]); sy = atoi(argv[i + 2]); }

        else if (!strcmp(argv[i], "-gammaCorrection")) { SET_BIT(args.whatToDo, BMP_GAMMA); gamma = atof(argv[i + 1]); }
    }

    if (!args.fInputPath.size())
    {
        help();
        return EXIT_FAILURE;
    }

    BMPReader bmpReader(args.fInputPath);

    if (IS_SET(args.whatToDo, BMP_BRIGHTNESS)) bmpReader.addBrightness(brightness);
    if (IS_SET(args.whatToDo, BMP_SHOWHEADER)) bmpReader.showHeader();

    if (IS_SET(args.whatToDo, BMP_TONEGATIVE)) bmpReader.toNegative();
    if (IS_SET(args.whatToDo, BMP_TOGRAY))	   bmpReader.toGray();

    if (IS_SET(args.whatToDo, BMP_ANGLE))      bmpReader.setAngle(angle);
    if (IS_SET(args.whatToDo, BMP_RESIZE))     bmpReader.setSize(sx, sy);

    if (IS_SET(args.whatToDo, BMP_BLUR))       bmpReader.blurFilter(blur, ks);
    if (IS_SET(args.whatToDo, BMP_SOBEL))      bmpReader.sobelFilter(sobel);

    if (IS_SET(args.whatToDo, BMP_GAMMA))      bmpReader.gammaCorrection(gamma);
    if (IS_SET(args.whatToDo, BMP_OUTPUT))     bmpReader.saveFile(args.fOutputPath);

    return(EXIT_SUCCESS);
}