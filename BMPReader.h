#pragma once

#include <string>

#pragma pack(push, 1)
typedef struct BMPHeader
{
    unsigned    short   int     mBMPType;                   /* Magic number for file */
    unsigned    int             mFileSize;                  /* Size of file */

    unsigned    short   int     mReserved1;                 /* Reserved */
    unsigned    short   int     mReserved2;                 /* Reserved */

    unsigned    int             mDataOffset;                /* Offset to bitmap data */
    unsigned    int             mHeaderSize;                /* Size of info header */

    unsigned    int             mImgWidth;                  /* Width of image */
    unsigned    int             mImgHeight;                 /* Height of image */

    unsigned    short   int     mPlansNr;                   /* Number of color planes */
    unsigned    short   int     mBitsXPixel;                /* Number of bits per pixel */

    unsigned    int             mCompression;               /* Type of compression to use */
    unsigned    int             mImageSize;                 /* Size of image data */

    unsigned    int             mHResolution;               /* X pixels per meter */
    unsigned    int             mVResolution;               /* Y pixels per meter */

    unsigned    int             mNumberOfColors;            /* Number of colors used */
    unsigned    int             mNumberOfImportantColors;   /* Number of important colors */
} BMPHeader;
#pragma pack(pop)

class BMPReader
{
    private:
        unsigned char *mPixels;
        BMPHeader      mBMPHeader;

    public:
        BMPReader(const std::string &);
        ~BMPReader(void);

        void    setSize(int, int);
        void    setAngle(float);
        void    addBrightness(int);

        void    showHeader(void);
        void    toNegative(void);
        void    toGray(void);

        void    saveFile(const std::string &);
        void    blurFilter(int, int);
        void    sobelFilter(float);
};
