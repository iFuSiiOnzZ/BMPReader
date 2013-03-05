#pragma once

typedef struct BMPHeader
{
    unsigned    short   int     mBMPType;
    unsigned    int             mFileSize;

    unsigned    short   int     mReserved1;
    unsigned    short   int     mReserved2;

    unsigned    int             mDataOffset;
    unsigned    int             mHeaderSize;

    unsigned    int             mImgWidth;
    unsigned    int             mImgHeight;

    unsigned    short   int     mPlanNr;
    unsigned    short   int     mBitsXPixel;

    unsigned    int             mCompression;
    unsigned    int             mFileSize;

    unsigned    int             mHResolution;
    unsigned    int             mVResolution;

    unsigned    int             mNumberOfColors;
    unsigned    int             mNumberOfImportantColors;
}BMPHeader;
