#include "BMPReader.h"

BMPReader::BMPReader(const std::string &fName)
{
    this->mFileName.assign(fName);
    memset(&this->mBMPHeader, 0, sizeof(BMPHeader));
}

BMPReader::~BMPReader(void)
{
    this->mFileName.clear();
    this->mColors.clear();
}
