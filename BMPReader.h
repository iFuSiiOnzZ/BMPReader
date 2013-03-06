#pragma once

#include <cstdio>
#include <vector>

#include <cmath>
#include <string>
#include <cstring>

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

        void    showHeader  (void);
};
