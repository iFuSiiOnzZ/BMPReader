#include "BMPReader.h"
#include <stdio.h>

#define CLAMP(x)  (((x) > (255)) ? (255) : (((x) < (0)) ? (0) : (x)))

BMPReader::BMPReader(const std::string &fPath) : mPixels(NULL)
{
    FILE *pFile = NULL;
    memset(&mBMPHeader, 0, sizeof(BMPHeader));

    if(fopen_s(&pFile, fPath.c_str(), "rb") != 0) return;
    fread(&mBMPHeader, (size_t)1, (size_t) sizeof(BMPHeader), pFile);

    if(mBMPHeader.mBitsXPixel != 24)
    {
        fclose(pFile);
        exit(EXIT_FAILURE);
    }

    unsigned int mallocSize = mBMPHeader.mImgHeight * mBMPHeader.mImgWidth * 3;
    mPixels = (unsigned char *) malloc (mallocSize * sizeof(unsigned char));

    unsigned char pad[4] = { 0 };
    unsigned int padding = (4 - ((mBMPHeader.mImgWidth * 3) % 4)) % 4;

    fseek(pFile, mBMPHeader.mDataOffset, SEEK_SET);
    for(unsigned int i = 0; i < mBMPHeader.mImgHeight; i++)
    {
        fread(mPixels + i * mBMPHeader.mImgWidth * 3, (size_t) 1, (size_t) mBMPHeader.mImgWidth * 3, pFile);
        fread(&pad, 1, padding, pFile);
    }

    fclose(pFile);
}

BMPReader::~BMPReader(void)
{
    if (mPixels)
    {
        free(mPixels);
        mPixels = NULL;
    }
}

void BMPReader::showHeader(void)
{
    printf("Identifier: %d\n",mBMPHeader.mBMPType);
    printf("Padding: %d\n\n", 4 - mBMPHeader.mImgWidth * 3 % 4);

    printf("File size: %d\n", mBMPHeader.mFileSize);
    printf("Image size: %d\n\n", mBMPHeader.mImageSize);

    printf("Header size: %d\n",mBMPHeader.mHeaderSize);
    printf("Data offset: %d\n\n", mBMPHeader.mDataOffset);

    printf("Image width: %d\n", mBMPHeader.mImgWidth);
    printf("Image heigh: %d\n\n", mBMPHeader.mImgHeight);

    printf("Bits per pixel: %d\n", mBMPHeader.mBitsXPixel);
    printf("Vertical resolution: %d\n", mBMPHeader.mVResolution);
    printf("Horizontal resolution: %d\n\n", mBMPHeader.mHResolution);
}

void BMPReader::saveFile(const std::string &fPath)
{
    printf("Writing file...\t");
    FILE *pFile = NULL;

    if(fopen_s(&pFile, fPath.c_str(), "wb") != 0) return;
    fwrite(&mBMPHeader, (size_t)1, (size_t) sizeof(BMPHeader), pFile);

    unsigned int padding = (4 - ((mBMPHeader.mImgWidth * 3) % 4)) % 4;
    fseek(pFile, mBMPHeader.mDataOffset, SEEK_SET);

    for(unsigned int i = 0; i < mBMPHeader.mImgHeight; i++)
    {
        fwrite(mPixels + i * mBMPHeader.mImgWidth * 3, (size_t) mBMPHeader.mImgWidth * 3, (size_t) 1, pFile);
        fwrite("\0", 1, padding, pFile);
    }

    fclose(pFile);
    printf("done!\n");
}

void BMPReader::addBrightness(int b)
{
    printf("Adding brightness...\t");
    unsigned int nPixels = mBMPHeader.mImgHeight * mBMPHeader.mImgWidth * 3;

    for(unsigned char *p = mPixels; p < mPixels + nPixels; p += 3)
    {
        *(p + 0) = CLAMP(*(p + 0) + b);
        *(p + 1) = CLAMP(*(p + 1) + b);
        *(p + 2) = CLAMP(*(p + 2) + b);
    }

    printf("done!\n");
}

void BMPReader::toNegative(void)
{
    printf("Converting to negative...\t");
    unsigned int nPixels = mBMPHeader.mImgHeight * mBMPHeader.mImgWidth * 3;

    for(unsigned char *p = mPixels; p < mPixels + nPixels; p += 3)
    {
        *(p + 0) = 255 - *(p + 0);
        *(p + 1) = 255 - *(p + 1);
        *(p + 2) = 255 - *(p + 2);
    }

    printf("done!\n");
}

void BMPReader::toGray(void)
{
    printf("Converting to gray...\t");
    unsigned int nPixels = mBMPHeader.mImgHeight * mBMPHeader.mImgWidth * 3;

    for(unsigned char *p = mPixels; p < mPixels + nPixels; p += 3)
    {
        unsigned char g = (unsigned char) (((*(p + 0)) * 0.3 + (*(p + 1)) * 0.59 + (*(p + 2)) * 0.11) + 0.5);
        *(p + 0) = *(p + 1) = *(p + 2) = g;
    }

    printf("done!\n");
}

void BMPReader::setAngle(float angle)
{
    printf("Rotating image...\t");

    angle = angle * 3.141562f / 180.0f;
    //int nWidth = (int) abs(floor(cosf(angle) * mBMPHeader.mImgWidth + sinf(angle) * mBMPHeader.mImgHeight));
    //int nHeight = (int) abs(floor(cosf(angle) * mBMPHeader.mImgHeight + sinf(angle) * mBMPHeader.mImgWidth));

    //float tx = mBMPHeader.mImgWidth / 2.0f;
    //float ty = mBMPHeader.mImgHeight / 2.0f;

    int nWidth = (int) mBMPHeader.mImgWidth;
    int nHeight = (int) mBMPHeader.mImgHeight;

    float tx = nWidth / 2.0f;
    float ty = nHeight / 2.0f;

    unsigned int padding = (4 - ((nWidth * 3) % 4)) % 4;
    unsigned int mallocSize = nHeight * nWidth * 3 + padding * nWidth;

    unsigned char *newImg = (unsigned char *) malloc (mallocSize * sizeof(unsigned char));
    memset(newImg, 0, mallocSize * sizeof(unsigned char));

    for(int cy = 0; cy < nHeight; cy++) for(int cx = 0; cx < nWidth; cx++)
    {
        int x = (int) (((cx - tx) * cosf(-angle) - (cy - ty) * sinf(-angle)) + tx);
        int y = (int) (((cx - tx) * sinf(-angle) + (cy - ty) * cosf(-angle)) + ty);

        if(x < 0 || y < 0 || x >= (int) mBMPHeader.mImgWidth || y >= (int) mBMPHeader.mImgHeight) continue;
        int m = y * mBMPHeader.mImgWidth * 3 + x * 3;
        int p = cy * nWidth * 3 + cx * 3;

        *(newImg + p + 0) = *(mPixels + m + 0);
        *(newImg + p + 1) = *(mPixels + m + 1);
        *(newImg + p + 2) = *(mPixels + m + 2);
    }

    free(mPixels);
    mPixels = newImg;

    mBMPHeader.mImgWidth = nWidth;
    mBMPHeader.mImgHeight = nHeight;

    mBMPHeader.mImageSize = nWidth * nHeight * 3;
    mBMPHeader.mFileSize = sizeof(BMPHeader) + mallocSize;

     printf("done!\n\n");
}

void BMPReader::setSize(int nWidth, int nHeight)
{
    printf("Rescaling image...\t");

    unsigned int padding = (4 - ((nWidth * 3) % 4)) % 4;
    unsigned int mallocSize = nHeight * nWidth * 3 + padding * nHeight;
    unsigned char *newImg = (unsigned char *) malloc (mallocSize * sizeof(unsigned char));

    float sy = (float) nHeight / (float) mBMPHeader.mImgHeight;
    float sx = (float) nWidth / (float) mBMPHeader.mImgWidth;

    for(int cy = 0; cy < nHeight; cy++) for(int cx = 0; cx < nWidth; cx++)
    {
        int p = cy * nWidth * 3 + cx * 3;
        int m = ((int) (cy / sy) * mBMPHeader.mImgWidth * 3) + ((int) (cx / sx) * 3);

        *(newImg + p + 0) = *(mPixels + m + 0);
        *(newImg + p + 1) = *(mPixels + m + 1);
        *(newImg + p + 2) = *(mPixels + m + 2);
    }

    free(mPixels);
    mPixels = newImg;

    mBMPHeader.mImgWidth = nWidth;
    mBMPHeader.mImgHeight = nHeight;

    mBMPHeader.mImageSize  = nWidth * nHeight * 3;
    mBMPHeader.mFileSize   = sizeof(BMPHeader) + mallocSize;

    printf("Done!\n");
}

static int Gradient(unsigned char *pImage, unsigned int R, unsigned int C, unsigned int Width, int G[3][3])
{
    unsigned char * p11 = pImage + Width * (R + 0) * 3 + C * 3;
    unsigned char * p10 = p11 - 3;
    unsigned char * p12 = p10 + 3;

    unsigned char * p01 = pImage + Width * (R - 1) * 3 + C * 3;
    unsigned char * p00 = p01 - 3;
    unsigned char * p02 = p01 + 3;

    unsigned char * p21 = pImage + Width * (R + 1) * 3 + C * 3;
    unsigned char * p20 = p21 - 3;
    unsigned char * p22 = p21 + 3;

    int p0 = *p00 * G[0][0] + *p01 * G[0][1] + *p02 * G[0][2];
    int p1 = *p10 * G[1][0] + *p11 * G[1][1] + *p12 * G[1][2];
    int p2 = *p20 * G[2][0] + *p21 * G[2][1] + *p22 * G[2][2];

    int p = p0 + p1 + p2;
    return p;
}

void BMPReader::sobelFilter(float filter)
{
    printf("Sobel edge detection...\t");
    unsigned int nWidth = mBMPHeader.mImgWidth;
    unsigned int nHeight = mBMPHeader.mImgHeight;

    unsigned int padding = (4 - ((nWidth * 3) % 4)) % 4;
    unsigned int mallocSize = nHeight * nWidth * 3 + padding * nHeight;

    unsigned char *newImg = (unsigned char *) malloc (mallocSize * sizeof(unsigned char));
    memset(newImg, 0, mallocSize);

    int GX[3][3] =
    {
        { -1, 0, +1 },
        { -2, 0, +2 },
        { -1, 0, +1 },
    };

    int GY[3][3] =
    {
        { -1, -2, -1 },
        {  0,  0,  0 },
        { +1, +2, +1 },
    };

    for(unsigned int r = 1; r < nHeight - 1; ++r)
    {
        for(unsigned int c = 1; c < nWidth - 1; ++c)
        {
            int Gx = Gradient(mPixels, r, c, nWidth, GX);
            int Gy = Gradient(mPixels, r, c, nWidth, GY);

            float v = sqrtf((float)(Gx * Gx + Gy * Gy)) ;

            newImg[r * nWidth * 3 + c * 3 + 0] = filter >= 1.0f ? (v > filter ? 255 : 0) : CLAMP((int) (v + 0.5f));
            newImg[r * nWidth * 3 + c * 3 + 1] = filter >= 1.0f ? (v > filter ? 255 : 0) : CLAMP((int) (v + 0.5f));
            newImg[r * nWidth * 3 + c * 3 + 2] = filter >= 1.0f ? (v > filter ? 255 : 0) : CLAMP((int) (v + 0.5f));
        }
    }

    free(mPixels);
    mPixels = newImg;
    printf("done\n");
}

static int Blur(unsigned char *pImage, unsigned int R, unsigned int C, unsigned int Width, float K[3][3])
{
    unsigned char * p01 = pImage + Width * (R - 1) * 3 + C * 3;
    unsigned char * p00 = p01 - 3;
    unsigned char * p02 = p01 + 3;

    unsigned char * p11 = pImage + Width * (R + 0) * 3 + C * 3;
    unsigned char * p10 = p11 - 3;
    unsigned char * p12 = p10 + 3;

    unsigned char * p21 = pImage + Width * (R + 1) * 3 + C * 3;
    unsigned char * p20 = p21 - 3;
    unsigned char * p22 = p21 + 3;

    float p0 = *p00 * K[0][0] + *p01 * K[0][1] + *p02 * K[0][2];
    float p1 = *p10 * K[1][0] + *p11 * K[1][1] + *p12 * K[1][2];
    float p2 = *p20 * K[2][0] + *p21 * K[2][1] + *p22 * K[2][2];

    float p = p0 + p1 + p2;
    return (int) (p + 0.5f);
}

static float Gaussian1D(int x, float s)
{
    float exp = ((x * x) / (2.0f * s * s));
    float bse = ((1.0f) / sqrtf(2.0f * 3.141592f * s * s));

    return bse * expf(-exp);
}

static float Gaussian2D(int x, int y, float s)
{
    float exp = ((x * x + y * y) / (2.0f * s * s));
    float bse = ((1.0f) / sqrtf(2.0f * 3.141592f * s * s));

    return bse * expf(-exp);
}

void BMPReader::blurFilter(int nPass, float sigma)
{
    printf("Blur filter...\t");
    unsigned int nWidth = mBMPHeader.mImgWidth;
    unsigned int nHeight = mBMPHeader.mImgHeight;

    unsigned int padding = (4 - ((nWidth * 3) % 4)) % 4;
    unsigned int mallocSize = nHeight * nWidth * 3 + padding * nHeight;

    unsigned char *newImg = (unsigned char *)malloc(mallocSize * sizeof(unsigned char));
    memcpy(newImg, mPixels, mallocSize * sizeof(unsigned char));

    float K[3][3] =
    {
        { Gaussian2D(1, 1, sigma), Gaussian2D(1, 1, sigma), Gaussian2D(1, 1, sigma) },
        { Gaussian2D(1, 1, sigma), Gaussian2D(2, 2, sigma), Gaussian2D(1, 1, sigma) },
        { Gaussian2D(1, 1, sigma), Gaussian2D(1, 1, sigma), Gaussian2D(1, 1, sigma) },
    };

    /*float K[3][3] =
    {
        { Gaussin1D(1, sigma), Gaussin1D(1, sigma), Gaussin1D(1, sigma) },
        { Gaussin1D(1, sigma), Gaussin1D(2, sigma), Gaussin1D(1, sigma) },
        { Gaussin1D(1, sigma), Gaussin1D(1, sigma), Gaussin1D(1, sigma) },
    };*/

    float kT = 0.0f;
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) kT += K[i][j];
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) K[i][j] /= kT;

    unsigned char *pBlurArr[2] =
    {
        (nPass % 2) == 0 ? mPixels : newImg,
        (nPass % 2) == 0 ? newImg : mPixels
    };

    while (nPass > 0)
    {
        size_t ReadFrom = (nPass - 0) % 2;
        size_t WriteTo = (nPass - 1) % 2;
        --nPass;

        for (unsigned int r = 1; r < nHeight - 1; ++r)
        {
            for (unsigned int c = 1; c < nWidth - 1; ++c)
            {
                int v0 = Blur(pBlurArr[ReadFrom] + 0, r, c, nWidth, K);
                int v1 = Blur(pBlurArr[ReadFrom] + 1, r, c, nWidth, K);
                int v2 = Blur(pBlurArr[ReadFrom] + 2, r, c, nWidth, K);

                pBlurArr[WriteTo][r * nWidth * 3 + c * 3 + 0] = CLAMP((int)v0);
                pBlurArr[WriteTo][r * nWidth * 3 + c * 3 + 1] = CLAMP((int)v1);
                pBlurArr[WriteTo][r * nWidth * 3 + c * 3 + 2] = CLAMP((int)v2);

                if (c == 1) // NOTE(Andrie): Copy the same pixels from second column to first columns
                {
                    pBlurArr[WriteTo][r * nWidth * 3 + 0] = CLAMP((int)v0);
                    pBlurArr[WriteTo][r * nWidth * 3 + 1] = CLAMP((int)v1);
                    pBlurArr[WriteTo][r * nWidth * 3 + 2] = CLAMP((int)v2);
                }
                else if (c == (nWidth - 2)) // NOTE(Andrei): Copy to same pixels from next-to-last column to last column
                {
                    pBlurArr[WriteTo][r * nWidth * 3 + (c + 1) * 3 + 0] = CLAMP((int)v0);
                    pBlurArr[WriteTo][r * nWidth * 3 + (c + 1) * 3 + 1] = CLAMP((int)v1);
                    pBlurArr[WriteTo][r * nWidth * 3 + (c + 1) * 3 + 2] = CLAMP((int)v2);
                }

                if (r == 1) // NOTE(Andrie): Copy the same pixels from second row to first row
                {
                    pBlurArr[WriteTo][(r - 1) * nWidth * 3 + c * 3 + 0] = CLAMP((int)v0);
                    pBlurArr[WriteTo][(r - 1) * nWidth * 3 + c * 3 + 1] = CLAMP((int)v1);
                    pBlurArr[WriteTo][(r - 1) * nWidth * 3 + c * 3 + 2] = CLAMP((int)v2);
                }
                else if (r == (nHeight - 2)) // NOTE(Andrei): Copy to same pixels from next-to-last row to last row
                {
                    pBlurArr[WriteTo][(r + 1) * nWidth * 3 + c * 3 + 0] = CLAMP((int)v0);
                    pBlurArr[WriteTo][(r + 1) * nWidth * 3 + c * 3 + 1] = CLAMP((int)v1);
                    pBlurArr[WriteTo][(r + 1) * nWidth * 3 + c * 3 + 2] = CLAMP((int)v2);
                }
            }
        }
    }

    free(mPixels);
    mPixels = newImg;
    printf("done\n");
}