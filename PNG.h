#ifndef _PNG_
#define _PNG_
#include <iostream>
#include<stdint.h>
#include<iomanip>
#include<vector>
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

	PNG();
	~PNG();
	char* PNGRead(const char *Filename);
	void PrintBlocks(BlockInfo* B);
	void PNGWrite(int size,char* buf,const char *Filename); 

//	MATRIX* PNGEncrypt(uint8_t* PtData);
//	void PNGDecrypt(char** buffer,MATRIX* layer);
};
#endif // !_PNG_
