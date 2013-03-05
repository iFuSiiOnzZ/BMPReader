#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "./BMPHeader.h"
#include "./RGBHeader.h"

class BMPReader
{
    private:
        std::vector<RGBHeader>  mColors;
        std::string             mFileName;
        BMPHeader               mBMPHeader;

    public:
        BMPReader           (const std::string &);
        ~BMPReader          (void);
};
