#ifndef _PNG_
#define _PNG_
#include <iostream>
#include <algorithm>
#include<zlib.h>
#include<stdint.h>
#include<iomanip>
#include<vector>
#include<math.h>
#pragma pack(1)

#define MATRIX std::vector<std::vector<uint32_t>>
#define MAXSIZE 1000000
//PNG is consisted with several info blocks in this structure
struct BlockInfo{
	uint32_t length;//length of data part, 4 Bytes
	uint8_t type[5];//name of block,4 Bytes
	uint8_t* PtData;//pointer to where we save the data
	uint32_t crc;//crc verification, 0 in common case
	int filesize;
	BlockInfo() { memset(this, 0, sizeof(BlockInfo)); }//construction function
};
class PNG {
public:
	std::vector<BlockInfo*> V;
	uint32_t height;
	uint32_t width;
	uint8_t depth;
	uint8_t colorType;
	uint8_t compression;
	uint8_t filter;
	uint8_t scan;

	PNG();
	~PNG();
	char* PNGRead(const char *Filename);
	void PrintBlocks(BlockInfo* B);
	void PNGWrite(int size,char* buf,const char *Filename); 
	void IHDRread();
	void PrintImageInfo();
	unsigned char* BufScan(int len,unsigned char* bufin, int type);
	MATRIX* PNGDecrypt();
//	void PNGEncrypt(char** buffer,MATRIX* layer);
private:
	void FilterNone(unsigned char* bufout,unsigned char*  line,int BytesPerPixel,int BytesPerRow,int offset);
	void FilterUp(unsigned char* bufout, unsigned char*  line, int BytesPerPixel, int BytesPerRow, int offset);
	void FilterSub(unsigned char* bufout, unsigned char*  line, int BytesPerPixel, int BytesPerRow, int offset);
	void FilterAvg(unsigned char* bufout, unsigned char*  line, int BytesPerPixel, int BytesPerRow, int offset);
	void FilterPaeth(unsigned char* bufout, unsigned char*  line, int BytesPerPixel, int BytesPerRow, int offset);
};
#endif // !_PNG_
