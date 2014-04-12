#include <cstdio>
#include <string>
#include <cstdlib>

#include "./BMPReader.h"

#define BMP_SHOWHEADER  (1 << 0)
#define BMP_TONEGATIVE  (1 << 1)
#define BMP_TOGRAY      (1 << 2)
#define BMP_Brightness  (1 << 3)

#define SET_BIT(var, bit)   ((var) |= (bit))
#define IS_SET(var, bit)    ((var) & (bit))

#define REMOVE_BIT(var, bit)    ((var) &= ~(bit))
#define TOGLE_BIT(var, bit)     ((var) ^= (bit))

struct ARGS
{
    unsigned int whatToDo;
    std::string fInputPath;
    std::string fOutputPath;
};

void help(void)
{
    std::printf("programName -i imputFile.bmp -o outFile.bmp [-toGray] [-toNegative] [-showHeader] [-addBrightness intValue] [-help]\n\n");
    std::printf("\t [-help] - Shows what you are reading\n");
    std::printf("\t [-toGray] - Converts the image to gray scale\n");
    std::printf("\t [-toNegative] - Converts the image to negative\n");
    std::printf("\t [-showHeader] - Shows the image header information\n");
    std::printf("\t [-addBrightness] - Changes the brightness of the image\n");
}

int main(int argc, char *argv[])
{
    struct ARGS args;
    args.whatToDo = 0;
    int brightness = 0;
    
    for(int i = 0;  i < argc; i++)
    { 
        if(!std::strcmp(argv[i], "-i")){ args.fInputPath.assign(argv[i + 1]);  }
        else if(!std::strcmp(argv[i], "-o")){ args.fOutputPath.assign(argv[i + 1]);  }
        
        else if(!std::strcmp(argv[i], "-help")){ help(); exit(EXIT_SUCCESS); }
        else if(!std::strcmp(argv[i], "-toGray")){ SET_BIT(args.whatToDo, BMP_TOGRAY); }
        else if(!std::strcmp(argv[i], "-toNegative")){ SET_BIT(args.whatToDo, BMP_TONEGATIVE); }
        else if(!std::strcmp(argv[i], "-showHeader")){ SET_BIT(args.whatToDo, BMP_SHOWHEADER); }
        else if(!std::strcmp(argv[i], "-addBrightness")){ SET_BIT(args.whatToDo, BMP_Brightness); brightness = std::atoi(argv[i + 1]); }
    }
    
    BMPReader bmpReader(args.fInputPath);
        if(IS_SET(args.whatToDo, BMP_Brightness)) bmpReader.addBrightness(brightness);
        if(IS_SET(args.whatToDo, BMP_SHOWHEADER)) bmpReader.showHeader();
        if(IS_SET(args.whatToDo, BMP_TONEGATIVE)) bmpReader.toNegative();
        if(IS_SET(args.whatToDo, BMP_TOGRAY))	  bmpReader.toGray();
    bmpReader.seveFile(args.fOutputPath);
    
    return(EXIT_SUCCESS);
}
