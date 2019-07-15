#ifndef BMP_READER_H
#define BMP_READER_H

#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <vector>
#include <iomanip>
#include <string>
#include <stddef.h>
#include <memory.h>

#pragma pack(1)

#define MATRIX std::vector<std::vector<uint32_t>>
#define MAXSIZE 10000000

struct bmpFileHead {
	uint16_t bfType;
	uint32_t bfSize;
	uint32_t bfReserved;
	uint32_t bfOffbits;
};
struct bmpInfoHead {
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPelsPerMeter;
	uint32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImprotant;
};

class Bmp {
private:
	void Print(bmpFileHead *p);
	void Print(bmpInfoHead *p);
	void WriteFileHead(bmpFileHead **H, MATRIX* layer,int x);
	void WriteInfoHead(bmpInfoHead **I, MATRIX* layer,int x);
	void ReadFileHead(bmpFileHead *H,char **buffer);
	void ReadInfoHead(bmpInfoHead *I,char **buffer);
public:
	MATRIX* BmpRead(const char *Filename);
	bool BmpWrite(MATRIX* layer,const char *Filename);
};

#pragma pack()

#endif //BMP_READER_H