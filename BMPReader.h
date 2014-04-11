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
        BYTE		  *mColors;
        BMPHeader      mBMPHeader;

    public:
        BMPReader           (const std::string &);
        ~BMPReader          (void);

        void    showHeader  (void);
		void	negative	(void);
		
		void	seveFile	(const std::string &);
};
