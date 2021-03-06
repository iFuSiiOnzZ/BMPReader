#include "BMPReader.h"
#include <stdio.h>

template <typename inputType, typename outputType = inputType> outputType clamp(inputType min, inputType val, inputType max)
{
    return static_cast<outputType>(val > max ? max : val < min ? min : val);
}

BMPReader::BMPReader(const std::string &fPath) : mPixels(NULL)
{
    FILE *pFile = NULL;
    memset(&mBMPHeader, 0, sizeof(BMPHeader));

    if (fopen_s(&pFile, fPath.c_str(), "rb") != 0) return;
    fread(&mBMPHeader, (size_t)1, (size_t) sizeof(BMPHeader), pFile);

    if (mBMPHeader.mBitsXPixel != 24)
    {
        fclose(pFile);
        exit(EXIT_FAILURE);
    }

    unsigned int mallocSize = mBMPHeader.mImgHeight * mBMPHeader.mImgWidth * 3;
    mPixels = (unsigned char *)malloc(mallocSize * sizeof(unsigned char));

    unsigned char pad[4] = { 0 };
    unsigned int padding = (4 - ((mBMPHeader.mImgWidth * 3) % 4)) % 4;

    fseek(pFile, mBMPHeader.mDataOffset, SEEK_SET);
    for (unsigned int i = 0; i < mBMPHeader.mImgHeight; i++)
    {
        fread(mPixels + i * mBMPHeader.mImgWidth * 3, (size_t)1, (size_t)mBMPHeader.mImgWidth * 3, pFile);
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
    printf("Identifier: %d\n", mBMPHeader.mBMPType);
    printf("Padding: %d\n\n", 4 - mBMPHeader.mImgWidth * 3 % 4);

    printf("File size: %d\n", mBMPHeader.mFileSize);
    printf("Image size: %d\n\n", mBMPHeader.mImageSize);

    printf("Header size: %d\n", mBMPHeader.mHeaderSize);
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

    if (fopen_s(&pFile, fPath.c_str(), "wb") != 0) return;
    fwrite(&mBMPHeader, (size_t)1, (size_t) sizeof(BMPHeader), pFile);

    unsigned int padding = (4 - ((mBMPHeader.mImgWidth * 3) % 4)) % 4;
    fseek(pFile, mBMPHeader.mDataOffset, SEEK_SET);

    for (unsigned int i = 0; i < mBMPHeader.mImgHeight; i++)
    {
        fwrite(mPixels + i * mBMPHeader.mImgWidth * 3, (size_t)mBMPHeader.mImgWidth * 3, (size_t)1, pFile);
        fwrite("\0", 1, padding, pFile);
    }

    fclose(pFile);
    printf("done!\n");
}

void BMPReader::addBrightness(int b)
{
    printf("Adding brightness...\t");
    unsigned int nPixels = mBMPHeader.mImgHeight * mBMPHeader.mImgWidth * 3;

    for (unsigned char *p = mPixels; p < mPixels + nPixels; p += 3)
    {
        *(p + 0) = clamp<int, unsigned char>(0, *(p + 0) + b, 255);
        *(p + 1) = clamp<int, unsigned char>(0, *(p + 1) + b, 255);
        *(p + 2) = clamp<int, unsigned char>(0, *(p + 2) + b, 255);
    }

    printf("done!\n");
}

void BMPReader::gammaCorrection(float gamma)
{
    printf("Gamma correction...\t");
    unsigned int nPixels = mBMPHeader.mImgHeight * mBMPHeader.mImgWidth * 3;

    for (unsigned char *p = mPixels; p < mPixels + nPixels; p += 3)
    {
        *(p + 0) = clamp<float, unsigned char>(0.0f, pow(*(p + 0) / 255.0f, (1.0f / gamma)) * 255.0f, 255.0f);
        *(p + 1) = clamp<float, unsigned char>(0.0f, pow(*(p + 1) / 255.0f, (1.0f / gamma)) * 255.0f, 255.0f);
        *(p + 2) = clamp<float, unsigned char>(0.0f, pow(*(p + 2) / 255.0f, (1.0f / gamma)) * 255.0f, 255.0f);
    }

    printf("done!\n");
}

void BMPReader::toNegative(void)
{
    printf("Converting to negative...\t");
    unsigned int nPixels = mBMPHeader.mImgHeight * mBMPHeader.mImgWidth * 3;

    for (unsigned char *p = mPixels; p < mPixels + nPixels; p += 3)
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

    for (unsigned char *p = mPixels; p < mPixels + nPixels; p += 3)
    {
        unsigned char g = (unsigned char)(((*(p + 0)) * 0.3 + (*(p + 1)) * 0.59 + (*(p + 2)) * 0.11) + 0.5);
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

    int nWidth = (int)mBMPHeader.mImgWidth;
    int nHeight = (int)mBMPHeader.mImgHeight;

    float tx = nWidth / 2.0f;
    float ty = nHeight / 2.0f;

    unsigned int padding = (4 - ((nWidth * 3) % 4)) % 4;
    unsigned int mallocSize = nHeight * nWidth * 3 + padding * nWidth;

    unsigned char *newImg = (unsigned char *)malloc(mallocSize * sizeof(unsigned char));
    memset(newImg, 0, mallocSize * sizeof(unsigned char));

    for (int cy = 0; cy < nHeight; cy++) for (int cx = 0; cx < nWidth; cx++)
    {
        int x = (int)(((cx - tx) * cosf(-angle) - (cy - ty) * sinf(-angle)) + tx);
        int y = (int)(((cx - tx) * sinf(-angle) + (cy - ty) * cosf(-angle)) + ty);

        if (x < 0 || y < 0 || x >= (int)mBMPHeader.mImgWidth || y >= (int)mBMPHeader.mImgHeight) continue;
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
    unsigned char *newImg = (unsigned char *)malloc(mallocSize * sizeof(unsigned char));

    float sy = (float)nHeight / (float)mBMPHeader.mImgHeight;
    float sx = (float)nWidth / (float)mBMPHeader.mImgWidth;

    for (int cy = 0; cy < nHeight; cy++) for (int cx = 0; cx < nWidth; cx++)
    {
        int p = cy * nWidth * 3 + cx * 3;
        int m = ((int)(cy / sy) * mBMPHeader.mImgWidth * 3) + ((int)(cx / sx) * 3);

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

    printf("Done!\n");
}

static int Gradient(unsigned char *pImage, unsigned int R, unsigned int C, unsigned int Width, int G[3][3])
{
    unsigned char * p11 = pImage + Width * (R + 0) * 3 + C * 3;
    unsigned char * p10 = p11 - 3;
    unsigned char * p12 = p11 + 3;

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

    unsigned char *newImg = (unsigned char *)malloc(mallocSize * sizeof(unsigned char));
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

    #pragma omp parallel for num_threads(8)
    for (int r = 1; r < (int)nHeight - 1; ++r)
    {
        for (int c = 1; c < (int)nWidth - 1; ++c)
        {
            int Gx = Gradient(mPixels, r, c, nWidth, GX);
            int Gy = Gradient(mPixels, r, c, nWidth, GY);

            float v = sqrtf((float)(Gx * Gx + Gy * Gy));
            int p = (int)(v + 0.5f);

            newImg[r * nWidth * 3 + c * 3 + 0] = (filter >= 1.0f ? (v > filter ? clamp<int, unsigned char>(0, p, 255) : 0) : clamp<int, unsigned char>(0, p, 255));
            newImg[r * nWidth * 3 + c * 3 + 1] = (filter >= 1.0f ? (v > filter ? clamp<int, unsigned char>(0, p, 255) : 0) : clamp<int, unsigned char>(0, p, 255));
            newImg[r * nWidth * 3 + c * 3 + 2] = (filter >= 1.0f ? (v > filter ? clamp<int, unsigned char>(0, p, 255) : 0) : clamp<int, unsigned char>(0, p, 255));
        }
    }

    free(mPixels);
    mPixels = newImg;
    printf("done\n");
}

static int BlurWrapAround(unsigned char *pImage, int R, int C, int Width, int Height, int Ks, float *K)
{
    int Kr = Ks / 2;
    float p = 0.0f;

    for (int i = -Kr; i <= Kr; ++i)  for (int j = -Kr; j <= Kr; ++j)
    {
        int R1 = R + i, C1 = C + j;

        if (R1 < 0) R1 = Height - 1 + R1;
        if (R1 > Height - 1) R1 = i;

        if (C1 < 0) C1 = Width - 1 + C1;
        if (C1 > Width - 1) C1 = j;

        p += *(pImage + Width * R1 * 3 + C1 * 3) * K[(i + Kr) * Ks + (j + Kr)];
    }

    return (int)(p + 0.5f);
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

static void GenGaussianKernel(int Ks, float *K)
{
    float s = 0.0f, sg = Ks * 0.5f;
    int Kr = Ks / 2;

    for (int i = -Kr; i <= Kr; ++i) for (int j = -Kr; j <= Kr; ++j)
    {
        K[(i + Kr) * Ks + j + Kr] = Gaussian2D(i, Ks, sg) * Gaussian2D(j, Ks, sg);
        s += K[(i + Kr) * Ks + j + Kr];
    }

    for (int i = 0; i < Ks; ++i) for (int j = 0; j < Ks; ++j)
    {
        K[i * Ks + j] /= s;
    }
}

void BMPReader::blurFilter(int nPass, int Ks)
{
    printf("Blur filter...\t");
    unsigned int nWidth = mBMPHeader.mImgWidth;
    unsigned int nHeight = mBMPHeader.mImgHeight;

    unsigned int padding = (4 - ((nWidth * 3) % 4)) % 4;
    unsigned int mallocSize = nHeight * nWidth * 3 + padding * nHeight;

    unsigned char *newImg = (unsigned char *)malloc(mallocSize * sizeof(unsigned char));
    memset(newImg, 0, mallocSize * sizeof(unsigned char));

    if (Ks <= 1) Ks = 3;
    if (Ks % 2 == 0) ++Ks;

    float *K = (float *)malloc(sizeof(float) * (Ks * Ks));
    GenGaussianKernel(Ks, K);

    float kT = 0.0f;
    int ModTwo = (nPass % 2);

    unsigned char *pBlurArr[2] =
    {
        ModTwo == 0 ? mPixels : newImg,
        ModTwo == 0 ? newImg : mPixels
    };

    while (nPass > 0)
    {
        size_t ReadFrom = (nPass - 0) % 2;
        size_t WriteTo = (nPass - 1) % 2;
        --nPass;

        #pragma omp parallel for num_threads(8)
        for (int r = 0; r < (int)nHeight; ++r)
        {
            for (int c = 0; c < (int)nWidth; ++c)
            {
                int v0 = BlurWrapAround(pBlurArr[ReadFrom] + 0, r, c, nWidth, nHeight, Ks, K);
                int v1 = BlurWrapAround(pBlurArr[ReadFrom] + 1, r, c, nWidth, nHeight, Ks, K);
                int v2 = BlurWrapAround(pBlurArr[ReadFrom] + 2, r, c, nWidth, nHeight, Ks, K);

                pBlurArr[WriteTo][r * nWidth * 3 + c * 3 + 0] = clamp<int, unsigned char>(0, v0, 255);
                pBlurArr[WriteTo][r * nWidth * 3 + c * 3 + 1] = clamp<int, unsigned char>(0, v1, 255);
                pBlurArr[WriteTo][r * nWidth * 3 + c * 3 + 2] = clamp<int, unsigned char>(0, v2, 255);
            }
        }
    }

    free(pBlurArr[1]);
    free(K);

    mPixels = pBlurArr[0];
    printf("done\n");
}
