#include "BMPReader.h"

BMPReader::BMPReader(const std::string &fName)
{
    this->mFileName.assign(fName);
    std::memset(&this->mBMPHeader, 0, sizeof(BMPHeader));

    std::FILE *pFile = fopen(this->mFileName.c_str(), "rb");
    unsigned int dataRead = 0;

    if(pFile != NULL)
    {
        dataRead += std::fread(&this->mBMPHeader.mBMPType, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mBMPType), pFile);
        dataRead += std::fread(&this->mBMPHeader.mFileSize, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mFileSize), pFile);

        dataRead += std::fread(&this->mBMPHeader.mReserved1, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mReserved1), pFile);
        dataRead += std::fread(&this->mBMPHeader.mReserved2, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mReserved2), pFile);

        dataRead += std::fread(&this->mBMPHeader.mDataOffset, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mDataOffset), pFile);
        dataRead += std::fread(&this->mBMPHeader.mHeaderSize, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mHeaderSize), pFile);

        dataRead += std::fread(&this->mBMPHeader.mImgWidth, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mImgWidth), pFile);
        dataRead += std::fread(&this->mBMPHeader.mImgHeight, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mImgHeight), pFile);

        dataRead += std::fread(&this->mBMPHeader.mPlansNr, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mPlansNr), pFile);
        dataRead += std::fread(&this->mBMPHeader.mBitsXPixel, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mBitsXPixel), pFile);

        dataRead += std::fread(&this->mBMPHeader.mCompression, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mCompression), pFile);
        dataRead += std::fread(&this->mBMPHeader.mImageSize, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mImageSize), pFile);

        dataRead += std::fread(&this->mBMPHeader.mHResolution, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mHResolution), pFile);
        dataRead += std::fread(&this->mBMPHeader.mVResolution, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mVResolution), pFile);

        dataRead += std::fread(&this->mBMPHeader.mNumberOfColors, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mNumberOfColors), pFile);
        dataRead += std::fread(&this->mBMPHeader.mNumberOfImportantColors, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mNumberOfImportantColors), pFile);

        std::fclose(pFile);

        if(this->mBMPHeader.mNumberOfColors == 0)
        {
            this->mBMPHeader.mNumberOfColors = (unsigned int) pow(2.0, (float) this->mBMPHeader.mBitsXPixel);
        }
    }
}

BMPReader::~BMPReader(void)
{
    this->mFileName.clear();
    this->mColors.clear();
}

void BMPReader::showHeader(void)
{
}
