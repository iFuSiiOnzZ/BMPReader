#include "BMPReader.h"

BMPReader::BMPReader(const std::string &fPath)
{
    std::FILE *pFile = NULL;
    std::memset(&this->mBMPHeader, 0, sizeof(BMPHeader));
    
    if(fopen_s(&pFile, fPath.c_str(), "rb") != 0) return;
    
    std::fread(&this->mBMPHeader.mBMPType, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mBMPType), pFile);
    std::fread(&this->mBMPHeader.mFileSize, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mFileSize), pFile);

    std::fread(&this->mBMPHeader.mReserved1, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mReserved1), pFile);
    std::fread(&this->mBMPHeader.mReserved2, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mReserved2), pFile);

    std::fread(&this->mBMPHeader.mDataOffset, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mDataOffset), pFile);
    std::fread(&this->mBMPHeader.mHeaderSize, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mHeaderSize), pFile);

    std::fread(&this->mBMPHeader.mImgWidth, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mImgWidth), pFile);
    std::fread(&this->mBMPHeader.mImgHeight, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mImgHeight), pFile);

    std::fread(&this->mBMPHeader.mPlansNr, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mPlansNr), pFile);
    std::fread(&this->mBMPHeader.mBitsXPixel, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mBitsXPixel), pFile);

    std::fread(&this->mBMPHeader.mCompression, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mCompression), pFile);
    std::fread(&this->mBMPHeader.mImageSize, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mImageSize), pFile);

    std::fread(&this->mBMPHeader.mHResolution, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mHResolution), pFile);
    std::fread(&this->mBMPHeader.mVResolution, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mVResolution), pFile);

    std::fread(&this->mBMPHeader.mNumberOfColors, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mNumberOfColors), pFile);
    std::fread(&this->mBMPHeader.mNumberOfImportantColors, (size_t) 1, (size_t) sizeof(this->mBMPHeader.mNumberOfImportantColors), pFile);

    if(this->mBMPHeader.mBitsXPixel != 24)
    {
        std::fclose(pFile);
        return;
    }
    
    unsigned int mallocSize = this->mBMPHeader.mImgHeight * this->mBMPHeader.mImgWidth * 3;
    this->mPixels = (unsigned char *) malloc (mallocSize * sizeof(unsigned char));
    
    std::fseek(pFile, this->mBMPHeader.mDataOffset, SEEK_SET);
    std::fread(this->mPixels,  (size_t) 1, (size_t) mallocSize, pFile);
    
    std::fclose(pFile);
}

BMPReader::~BMPReader(void)
{
    delete(this->mPixels);
    this->mPixels = NULL;
}

void BMPReader::showHeader(void)
{
    std::printf("Identifier: %d\n", this->mBMPHeader.mBMPType);
    std::printf("File size: %d\n\n", this->mBMPHeader.mFileSize);
    
    std::printf("Header size: %d\n",this->mBMPHeader.mHeaderSize);
    std::printf("Data offset: %d\n\n", this->mBMPHeader.mDataOffset);
    
    std::printf("Image width: %d\n", this->mBMPHeader.mImgWidth);
    std::printf("Image heigh: %d\n\n", this->mBMPHeader.mImgHeight);
    
    std::printf("Bits per pixel: %d\n", this->mBMPHeader.mBitsXPixel);
    std::printf("Vertical resolution: %d\n", this->mBMPHeader.mVResolution);
    std::printf("Horizontal resolution: %d\n\n", this->mBMPHeader.mHResolution);
    
    std::printf("Image size: %d\n", this->mBMPHeader.mImageSize);
    std::printf("Image compression: %d\n\n", this->mBMPHeader.mCompression);
}

void BMPReader::seveFile(const std::string &fPath)
{
    printf("Writting file...\t");
    std::FILE *pFile = NULL;
    
    if(fopen_s(&pFile, fPath.c_str(), "wb") != 0) return;
    
    std::fwrite(&this->mBMPHeader.mBMPType, (size_t) sizeof(this->mBMPHeader.mBMPType), (size_t) 1, pFile);
    std::fwrite(&this->mBMPHeader.mFileSize, (size_t) sizeof(this->mBMPHeader.mFileSize), (size_t) 1, pFile);

    std::fwrite(&this->mBMPHeader.mReserved1, (size_t) sizeof(this->mBMPHeader.mReserved1), (size_t) 1, pFile);
    std::fwrite(&this->mBMPHeader.mReserved2, (size_t) sizeof(this->mBMPHeader.mReserved2), (size_t) 1, pFile);

    std::fwrite(&this->mBMPHeader.mDataOffset, (size_t) sizeof(this->mBMPHeader.mDataOffset), (size_t) 1, pFile);
    std::fwrite(&this->mBMPHeader.mHeaderSize, (size_t) sizeof(this->mBMPHeader.mHeaderSize), (size_t) 1, pFile);

    std::fwrite(&this->mBMPHeader.mImgWidth, (size_t) sizeof(this->mBMPHeader.mImgWidth), (size_t) 1, pFile);
    std::fwrite(&this->mBMPHeader.mImgHeight, (size_t) sizeof(this->mBMPHeader.mImgHeight), (size_t) 1, pFile);

    std::fwrite(&this->mBMPHeader.mPlansNr, (size_t) sizeof(this->mBMPHeader.mPlansNr), (size_t) 1, pFile);
    std::fwrite(&this->mBMPHeader.mBitsXPixel, (size_t) sizeof(this->mBMPHeader.mBitsXPixel), (size_t) 1, pFile);

    std::fwrite(&this->mBMPHeader.mCompression, (size_t) sizeof(this->mBMPHeader.mCompression), (size_t) 1, pFile);
    std::fwrite(&this->mBMPHeader.mImageSize, (size_t) sizeof(this->mBMPHeader.mImageSize), (size_t) 1, pFile);

    std::fwrite(&this->mBMPHeader.mHResolution, (size_t) sizeof(this->mBMPHeader.mHResolution), (size_t) 1, pFile);
    std::fwrite(&this->mBMPHeader.mVResolution, (size_t) sizeof(this->mBMPHeader.mVResolution), (size_t) 1, pFile);

    std::fwrite(&this->mBMPHeader.mNumberOfColors, (size_t) sizeof(this->mBMPHeader.mNumberOfColors), (size_t) 1, pFile);
    std::fwrite(&this->mBMPHeader.mNumberOfImportantColors, (size_t) sizeof(this->mBMPHeader.mNumberOfImportantColors), (size_t) 1, pFile);
    
    unsigned int ws = this->mBMPHeader.mImgHeight * this->mBMPHeader.mImgWidth * 3;
    
    std::fseek(pFile, this->mBMPHeader.mDataOffset, SEEK_SET);
    std::fwrite(this->mPixels, (size_t) ws, (size_t) 1, pFile);
    
    std::fclose(pFile);
    printf("done!\n\n");
}

void BMPReader::addBrightness(int b)
{
    printf("Adding brightness...\t");
    unsigned int nPixels = this->mBMPHeader.mImgHeight * this->mBMPHeader.mImgWidth * 3;
    
    for(unsigned char *p = this->mPixels; p < this->mPixels + nPixels; p += 3)
    {
        *(p + 0) = CLAMP(*(p + 0) + b);
        *(p + 1) = CLAMP(*(p + 1) + b);
        *(p + 2) = CLAMP(*(p + 2) + b);
    }
    
    printf("done!\n\n");
}

void BMPReader::toNegative(void)
{
    printf("Converting to negative...\t");
    unsigned int nPixels = this->mBMPHeader.mImgHeight * this->mBMPHeader.mImgWidth * 3;
    
    for(unsigned char *p = this->mPixels; p < this->mPixels + nPixels; p += 3)
    {
        *(p + 0) = 255 - *(p + 0);
        *(p + 1) = 255 - *(p + 1);
        *(p + 2) = 255 - *(p + 2);
    }
    
    printf("done!\n\n");
}

void BMPReader::toGray(void)
{
    printf("Converting to gray...\t");
    unsigned int nPixels = this->mBMPHeader.mImgHeight * this->mBMPHeader.mImgWidth * 3;
    
    for(unsigned char *p = this->mPixels; p < this->mPixels + nPixels; p += 3)
    {
        unsigned char g = (unsigned char) ((*(p + 0)) * 0.3 + (*(p + 1)) * 0.59 + (*(p + 2)) * 0.11);
        *(p + 0) = g;
        *(p + 1) = g;
        *(p + 2) = g;
    }
    
    printf("done!\n\n");
}
