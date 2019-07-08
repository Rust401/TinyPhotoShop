#include "BmpReader.h"
#include "utils.h"

//read picture and save it,only for bmp
MATRIX* Bmp::BmpRead(const char *Filename) {
	char *buffer = new char[MAXSIZE];//to save what we read from file
	bmpFileHead *H = new bmpFileHead;
	bmpInfoHead *I = new bmpInfoHead;
	FILE *fp;
    
    fp=fopen(Filename,"rb");
    if(fp==nullptr)err("open failed.");

	/* if (err = fopen_s(&fp, Filename, "rb") != 0) {//open file
		std::cout << "open pictrue failed" << std::endl;//if can not open, output error
		return NULL;
	} */
	fread(buffer, 1, MAXSIZE, fp);//read from file

	//read the filehead and infohead
	//ReadFileHead(H, &buffer);
	//ReadInfoHead(I, &buffer);
	memcpy(H, buffer, sizeof(bmpFileHead));
	buffer += sizeof(bmpFileHead);
	memcpy(I, buffer, sizeof(bmpInfoHead));
	buffer += sizeof(bmpInfoHead);

	//save the pointer of layer in res
	MATRIX *res = new MATRIX(I->biHeight, std::vector<uint32_t>(I->biWidth, 0));
	
	//the bmp RGB info is saved from the last row's first column, fill the row first and move up
	for (int i = I->biHeight - 1; i >= 0; i--) {
		for (int j = 0; j < (int)I->biWidth; j++) {
			uint32_t tmp = 0;
			char *t = (char *)&tmp;

			*t = *(buffer + 2);//R
			*(t + 1) = *(buffer + 1);//G
			*(t + 2) = *(buffer);//B
			*(t + 3) = (uint8_t)0;//Alpha
			buffer = buffer + 3;
			(*res)[i][j] = tmp;
		}
		// 32 bits Windows OS will complete every line to  integer times of 4 with "0", in order to improve performance
		// so we calculate the numbers o completed bits,and skip them
		//			std::cout << "CCC" << std::endl;
		buffer += (4 - (3 * (int)I->biWidth & 3)) & 3;
	}

	Print(H);
	Print(I);
	//free
	if (!I) {
		delete I;
		I = nullptr;
	}
	if (!H) {
		delete H;
		H = nullptr;
	}
	if (!buffer) {
		delete []buffer;
		buffer = nullptr;
	}
	

	return res;
}
void Bmp::BmpWrite(MATRIX* layer,const char* Filename) {
	FILE *fp;
	bmpFileHead *H = new bmpFileHead;
	bmpInfoHead *I = new bmpInfoHead;
	int height, width;
	if (Filename == NULL) {
		err("empty file name.");
		return;
	}
/* 	fp=fopen(Filename, "r");
	if (fp) {
		err("filename already used");
		return;
	} */
    fp=fopen(Filename,"wb");
    if(fp==nullptr){
        err("open failed.");
        return;
    }
	/* if (err = fopen_s(&fp,Filename, "wb")!=0) {
		std::cout << "crate file failed" << std::endl;
		return;
	} */
	//if the file can be opened
	char *buffer = new char[MAXSIZE];
	char *BHead = buffer;
	memset(buffer, 0, MAXSIZE);
	int temp;
	height = (*layer).size();
	width = (*layer)[0].size();

	buffer += (sizeof(bmpFileHead) + sizeof(bmpInfoHead));
	for (int i = height - 1; i >= 0; i--) {
		for (int j = 0; j < width; j++) {
			uint32_t tmp = (*layer)[i][j];
	/*		if (j < 5) {
				std::cout << tmp << " " << i << std::endl;
			}*/
			char *t = (char *)&tmp;
	//		std::cout << "111" << std::endl;
			*buffer = *(t + 2);
			*(buffer + 1) = *(t + 1);
			*(buffer + 2) = *t;
			buffer += 3;
		}
		temp = (4 - (3 * (int)width & 3)) & 3;
		for (int j = 0; j < temp; j++) {
//			std::cout << "222" << std::endl;
			*buffer = 0x00;
			buffer++;
		}
//		std::cout << "333" << std::endl;
	}

	//write the head
	WriteFileHead(&H, layer, temp);
	WriteInfoHead(&I, layer,temp);
	//copy the head info from beginning
	memcpy(BHead,H, sizeof(bmpFileHead));
	BHead += sizeof(bmpFileHead);
	memcpy(BHead,I, sizeof(bmpInfoHead));
	BHead -= sizeof(bmpFileHead);
	
	Print(H);
	Print(I);

	fwrite(BHead, 1,H->bfSize , fp);
	fclose(fp);
	std::cout << "write successfully" << std::endl;
	if (!I) {
		delete I;
		I = nullptr;
	}
	if (!H) {
		delete H;
		H = nullptr;
	}
	if (!buffer) {
		delete[]buffer;
		buffer = nullptr;
	}
	return;
}
void Bmp::ReadFileHead(bmpFileHead *H,char **buffer) {
	memcpy(H, *buffer, sizeof(bmpFileHead));//get file head
	*buffer += sizeof(bmpFileHead);//move pointer
}
void Bmp::ReadInfoHead(bmpInfoHead *I,char **buffer) {
	memcpy(I, *buffer, sizeof(bmpInfoHead));//get info head
	*buffer += sizeof(bmpInfoHead);//move pointer
}
void Bmp::WriteFileHead(bmpFileHead **H, MATRIX* layer,int x) {
	int height = (*layer).size();
	int width = (*layer)[0].size();
	(*H)->bfType = 0x4d42;
	(*H)->bfReserved = 0;
	(*H)->bfSize = height * (3 * width + x) + sizeof(bmpFileHead) + sizeof(bmpInfoHead);
	(*H)->bfOffbits = 0x36;
}
void Bmp::WriteInfoHead(bmpInfoHead **I, MATRIX* layer,int x) {
	(*I)->biHeight = (*layer).size();
	(*I)->biWidth = (*layer)[0].size();
	(*I)->biSize = 0x28;
	(*I)->biClrImprotant = 0;
	(*I)->biClrUsed = 0;
	(*I)->biCompression = 0;
	(*I)->biPlanes = 1;
	(*I)->biXPelsPerMeter = 0;
	(*I)->biYPelsPerMeter = 0;
	(*I)->biSizeImage = (*I)->biHeight * ((*I)->biWidth * 3 + x);
	(*I)->biBitCount = 0x18;
}
void Bmp::Print(bmpFileHead *p) {
	std::cout << "---------------------------------" << std::endl;
	std::cout << "bmpFileHead :" << std::endl;
	std::cout << (*p).bfType<< std::endl;
	std::cout << (*p).bfSize << std::endl;
	std::cout << (*p).bfReserved<< std::endl;
	std::cout << (*p).bfOffbits<< std::endl;
	std::cout << "---------------------------------" << std::endl;
}
void Bmp::Print(bmpInfoHead *p) {
	std::cout << "---------------------------------" << std::endl;
	std::cout << "bmpInfoHead :" << std::endl;
	std::cout << (*p).biSize<< std::endl;
	std::cout << (*p).biWidth<< std::endl;
	std::cout << (*p).biHeight<< std::endl;
	std::cout << (*p).biPlanes << std::endl;
	std::cout << (*p).biBitCount<< std::endl;
	std::cout << (*p).biCompression << std::endl;
	std::cout << (*p).biSizeImage<< std::endl;
	std::cout << (*p).biXPelsPerMeter<< std::endl;
	std::cout << (*p).biYPelsPerMeter<< std::endl;
	std::cout << (*p).biClrUsed<< std::endl;
	std::cout << (*p).biClrImprotant<< std::endl;
	std::cout << "---------------------------------" << std::endl;
}