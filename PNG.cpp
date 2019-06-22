#include"pch.h"
#include"PNG.h"
#pragma pack(1)

PNG::PNG() {
	std::vector<BlockInfo*> V ;
}
PNG::~PNG() {
	for (std::vector<BlockInfo*>::iterator it = V.begin(); it != V.end(); it++) {
		if (*it != NULL) {
			delete *it;
			*it = NULL;
		}
	}
	V.clear();
}
char* PNG::PNGRead(const char* Filename) {
	char* buffer = new char[MAXSIZE];
	memset(buffer, 0, MAXSIZE);
	FILE* fp;
	errno_t err;
	char* bHead = buffer;
	uint8_t* cur = (uint8_t *)bHead;
	char MagicNum[8];

	//open .png file and read info into buffer
	if (err = fopen_s(&fp, Filename, "rb") != 0) {
		std::cout << "open failed" << std::endl;
		exit(1);
	}
	int size = fread(buffer, 1, MAXSIZE, fp);

	//read the "magic number"
	for (int i = 0; i < 8; i++) {
		MagicNum[i] = *cur;
		std::cout << std::hex << (uint16_t)*cur << std::endl;
		cur++;
	}
	//Magicnum should be ID of PNG
	uint64_t Magicnum = *((uint64_t*)bHead);
	if (Magicnum != 0x0A1A0A0D474E5089) {//this is not a .png file
		std::cout << "wrong file type" << std::endl;
		exit(1);
	}


	uint8_t* curHead = cur;//cueHead is pointing to the beginnig of the first Info Block
	while (*((uint64_t*)curHead) != 0) {
		BlockInfo* tmp = new BlockInfo;
		uint8_t length[4];
		for (int i = 0; i < 4; i++) {
			length[3 - i] = *(curHead++);
		}
		uint32_t* Plen = (uint32_t*)length;
		tmp->length = *Plen;
		for (int i = 0; i < 4; i++) {
			tmp->type[i] = *(curHead++);
		}
		tmp->PtData = curHead;
		curHead += *Plen;
		tmp->crc = 0;
		curHead += 4;
		tmp->filesize = size;

		V.push_back(tmp);
	}
	return buffer;
}
void PNG::PrintBlocks(BlockInfo* B) {
	std::cout << "--------------------------------------------" << std::endl;
	std::cout << "name : " << (uint32_t)*B->type << std::endl;
	std::cout << "length : " <<std::dec<< B->length << std::endl;
	std::cout << "pointer : " << B->PtData << std::endl;
	std::cout << "CRC : " << B->crc << std::endl;
	std::cout << "--------------------------------------------" << std::endl;
}
void PNG::PNGWrite(int size,char* buf,const char* Filename) {
	FILE* fp;
	errno_t err;

	if (Filename == NULL) {
		std::cout << "empty filename!" << std::endl;
		return;
	}
	fopen_s(&fp, Filename, "r");
	if (fp) {
		std::cout << "filename already used" << std::endl;
		return;
	}

	if (err = fopen_s(&fp, Filename, "wb") != 0) {
		std::cout << "create file failed" << std::endl;
		return;
	}
	std::string tmp = buf;
	fwrite(buf, 1,size , fp);
	fclose(fp);
	std::cout << "write successfully" << std::endl;
}