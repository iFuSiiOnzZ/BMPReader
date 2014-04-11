#pragma once

#include <cstdio>
#include <vector>

#include <cmath>
#include <string>
#include <cstring>

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

    unsigned    short   int     mPlansNr;
    unsigned    short   int     mBitsXPixel;

    unsigned    int             mCompression;
    unsigned    int             mImageSize;

    unsigned    int             mHResolution;
    unsigned    int             mVResolution;

    unsigned    int             mNumberOfColors;
    unsigned    int             mNumberOfImportantColors;
}BMPHeader;

class BMPReader
{
    private:
        unsigned char *mPixels;
        BMPHeader      mBMPHeader;

    public:
        BMPReader           (const std::string &);
        ~BMPReader          (void);

        void    showHeader  (void);
		void	toNegative	(void);
		void	toGray		(void);
		
		void	seveFile	(const std::string &);
};
