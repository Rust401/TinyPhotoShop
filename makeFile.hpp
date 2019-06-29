#ifndef _MMMMMM_
#define _MMMMMM_

#include "BmpReader.hpp"
#include "dude.h"
class File:public RS::BasicLayer 
{
public:
	File(){}
	File(dataBuffer& caiji) {
		reInit(caiji);
		setLayerName("ceshi");
	}
	void Buf2File(const char*Filename);
	void File2Buf(const char* Filename);
	void Print();
};
void File::Buf2File(const char*Filename) {
	FILE* fp;
	errno_t err;

	if (err = fopen_s(&fp, Filename, "wb") != 0) {
		std::cout << "open failed" << std::endl;
		return;
	}
	
	int height = datamatrix.size();
	int width = datamatrix[0].size();
	unsigned char* buffer = new unsigned char[height*width*4*2];
	unsigned char* bHead = buffer;

	
	memcpy(buffer, name.c_str(), name.length());
	buffer += 100;

	unsigned char* tmpH = (unsigned char*)&height;
	unsigned char* tmpW = (unsigned char*)&width;
	for (int i = 0; i < 4; i++) {
		*(buffer + i) = *(tmpH + i);
		*(buffer + 4 + i) = *(tmpW + i);
	}
	buffer += 8;
	unsigned char* tmpN = (unsigned char*)&layerNumber;
	for (int i = 0; i < 2; i++) {
		*(buffer++) = *(tmpN++);
	}
	if (Valid == true)
		*(buffer++) = 'T';
	else
		*(buffer++) = 'F';

	dataBuffer pixelMatrix ;
	getDataBuffer(pixelMatrix);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			uint32_t tmp = pixelMatrix[i][j];
			unsigned char* t = (unsigned char*)&tmp;
			*buffer = *t;
			*(buffer + 1) = *(t + 1);
			*(buffer + 2) = *(t + 2);
			*(buffer + 3) = *(t + 3);
			buffer += 4;
		}
	}
	//memcpy(buffer, &pixelMatrix, width*height);
	int filesize = 4*width * height + 100 + 11;
	fwrite(bHead, 1, filesize, fp);
	fclose(fp);
	std::cout << "success" << std::endl;
	return;
	
}
void File::File2Buf(const char* Filename) {
	FILE* fp;
	errno_t err;
	unsigned char* buffer = new unsigned char[MAXSIZE];
	unsigned char* bHead = buffer;
	if (err = fopen_s(&fp, Filename, "rb") != 0) {
		std::cout << "open failed" << std::endl;
	}
	int size = fread(buffer, 1, MAXSIZE, fp);
	memcpy((void*)name.c_str(), buffer, name.length());
	buffer += 100;
	int height = 0;
	int width =0;
	unsigned char* tmpH = (unsigned char*)&height;
	unsigned char* tmpW = (unsigned char*)&width;
	for (int i = 0; i < 4; i++) {
		*(tmpH + i) = *(buffer + i);
		*(tmpW + i) = *(buffer + 4 + i);
	}
	buffer += 8;
	uint16_t tmp = 0;
	unsigned char* tmpN = (unsigned char*)&tmp;
	for (int i = 0; i < 2; i++) {
		*(buffer++) = *(tmpN++);
	}
	layerNumber = tmp;
	if (*(buffer++) == 'T') Valid = true;
	else Valid = false;
	dataBuffer pixelBuffer(height,rowData(width));
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			uint32_t tmp = 0;
			unsigned char* t = (unsigned char*)&tmp;
			*t = *buffer;
			*(t + 1) = *(buffer + 1);
			*(t + 2) = *(buffer + 2);
			*(t + 3) = *(buffer + 3);
			buffer += 4;
			pixelBuffer[i][j] = tmp;
		}
	}
//	memcpy(&pixelBuffer, buffer, width*height*4);
	setDataMatrix(pixelBuffer);

	fclose(fp);
	std::cout << "success" << std::endl;
	return;
}
void File::Print(){
	display();
	displayData();
}
#endif // !_MMMMMM_
