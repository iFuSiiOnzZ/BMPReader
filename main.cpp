#include <cstdio>
#include <cstdlib>

#include "./BMPReader.h"

int main(int argc, char *argv[])
{
	BMPReader bmpReader("./nature.bmp");
	bmpReader.showHeader();

	bmpReader.negative();
	bmpReader.seveFile("./aa.bmp");

	std::printf("Press enter to exit ");
	getchar();

    return(EXIT_SUCCESS);
}

