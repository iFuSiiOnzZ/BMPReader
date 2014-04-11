#include "BMPReader.h"

BMPReader::BMPReader(const std::string &fPath)
{
    std::FILE *pFile = NULL;
	std::memset(&this->mBMPHeader, 0, sizeof(BMPHeader));

	fopen_s(&pFile, fPath.c_str(), "rb");
    if(pFile == NULL) return;

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
	this->mColors = (BYTE *) malloc (mallocSize * sizeof(BYTE));

	std::fseek(pFile, this->mBMPHeader.mDataOffset, SEEK_SET);
	std::fread(this->mColors,  (size_t) 1, (size_t) mallocSize, pFile);

	std::fclose(pFile);
}

BMPReader::~BMPReader(void)
{
	delete(this->mColors);
	this->mColors = NULL;
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
	printf("Writting file in progress...\n");
    std::FILE *pFile = NULL;

	fopen_s(&pFile, fPath.c_str(), "wb");
    if(pFile == NULL) return;

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
	std::fwrite(this->mColors, (size_t) ws, (size_t) 1, pFile);

	std::fclose(pFile);
	printf("Writting file in done...\n\n");
}

void BMPReader::negative(void)
{
	printf("Negative in progress...\n");
	unsigned int colorsSize = this->mBMPHeader.mImgHeight * this->mBMPHeader.mImgWidth * 3;

	for(BYTE *p = this->mColors; p < this->mColors + colorsSize; p += 3)
	{
		*(p + 0) = 255 - *(p + 0);
		*(p + 1) = 255 - *(p + 1);
		*(p + 2) = 255 - *(p + 2);
	}

	printf("Negative in done...\n\n");
}
