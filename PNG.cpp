#include"pch.h"
#include"PNG.h"
#pragma pack(1)

PNG::PNG() {
	std::vector<BlockInfo*> V ;
	width = 0;
	height = 0;
	depth = 0;
	colorType = 0;
	compression = 0;
	filter = 0;
	scan = 0;
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
void PNG::IHDRread() {
//	std::vector<BlockInfo*>::iterator it;
	uint8_t H[4], W[4];
	for (int i = 0; i < V.size();i++) {
		if (*((uint32_t*)V[i]->type) != 0x52444849) {
			continue;
		}
		else {
			uint8_t* dHead = V[i]->PtData;
			uint8_t* cur = dHead;
			
			for (int i = 0; i < 4; i++) {
				W[3 - i] = *(cur++);
			}
			for (int i = 0; i < 4; i++) {
				H[3 - i] = *(cur++);
			}
			height = *((uint32_t*)H);
			width = *((uint32_t*)W);
			depth = *(cur++);
			colorType = *(cur++);
			compression = *(cur++);
			filter = *(cur++);
			scan = *(cur++);
			std::cout <<std::dec<< width<<" " << height <<" "<<(uint16_t)depth<< std::endl;
			std::cout << (uint16_t)colorType << " " << (uint16_t)compression << " " << (uint16_t)filter << " " << (uint16_t)scan << std::endl;
		}
	}
}
void PNG::PrintImageInfo() {
	std::cout << "-----------ImageInfo------------" << std::endl;
	std::cout << "HEIGHT : " << height << std::endl;
	std::cout << "WIDTH : " << width << std::endl;
	std::cout << "DEPTH : " << depth << std::endl;
	std::cout << "ColorType : " << colorType << std::endl;
	std::cout << "Compression : " << compression << std::endl;
	std::cout << "Filter : " << filter << std::endl;
	std::cout << "Scan : " << scan << std::endl;
	std::cout << "--------------------------------"<<std::endl;
}
MATRIX* PNG::PNGDecrypt() {
	int offset = 0;
	unsigned char* buffer = new unsigned char[MAXSIZE];
	unsigned char* newbuffer = nullptr;
	unsigned char* scanbuffer = nullptr;
	for (int i = 0; i < V.size(); i++) {
		if (*((uint32_t*)V[i]->type) != 0x54414449) {
			continue;
		}
		else {
			memcpy(buffer+offset, V[i]->PtData, V[i]->length);
			offset += V[i]->length;
		}
	}
	unsigned char* bHead = buffer;
	unsigned char* cur = bHead;
	uLongf len  = 4*offset;
	
	newbuffer = new unsigned char[len];
	if (uncompress(newbuffer, &len, buffer, offset) != Z_OK) {
		std::cout << "uncompress failed" << std::endl;
		return nullptr;
	}
	int length = len;

	scanbuffer = BufScan(length,newbuffer,scan);

	MATRIX* res = new MATRIX(height, std::vector<uint32_t>(width, 0));

	switch (colorType) {
	case 0:
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				uint32_t tmp = 0;
				unsigned char *t = (unsigned char *)&tmp;
				*t = *(scanbuffer);//R
				*(t + 1) = (uint8_t)0;//G
				*(t + 2) = (uint8_t)0;//B
				*(t + 3) = (uint8_t)0;//Alpha
				scanbuffer = scanbuffer + 1;
				(*res)[i][j] = tmp;
			}
		}
		break;
	case 2:
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				uint32_t tmp = 0;
				unsigned char *t = (unsigned char *)&tmp;
				*t = *(scanbuffer);//R
				*(t + 1) = *(scanbuffer+1);//G
				*(t + 2) = *(scanbuffer+2);//B
				*(t + 3) = (uint8_t)0;//Alpha
				scanbuffer = scanbuffer + 3;
				(*res)[i][j] = tmp;
			}
		}
		break;
	//case 3:    索引
	//	for (int i = 0; i < height; i++) {
	//		for (int j = 0; j < width; j++) {
	//			uint32_t tmp = 0;
	//			unsigned char *t = (unsigned char *)&tmp;
	//			*t = *(scanbuffer);//R
	//			*(t + 1) = *(scanbuffer + 1);//G
	//			*(t + 2) = *(scanbuffer + 2);//B
	//			*(t + 3) = (uint8_t)0;//Alpha
	//			scanbuffer = scanbuffer + 3;
	//			(*res)[i][j] = tmp;
	//		}
	//	}
	//	break;
	case 4:
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				uint32_t tmp = 0;
				unsigned char *t = (unsigned char *)&tmp;
				*t = *(scanbuffer);//R
				*(t + 1) = (uint8_t)0;//G
				*(t + 2) = (uint8_t)0;//B
				*(t + 3) = *(scanbuffer+1);//Alpha
				scanbuffer = scanbuffer + 2;
				(*res)[i][j] = tmp;
			}
		}
		break;
	case 6:
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				uint32_t tmp = 0;
				unsigned char *t = (unsigned char *)&tmp;
				*t = *(scanbuffer);//R
				*(t + 1) = *(scanbuffer+1);//G
				*(t + 2) = *(scanbuffer+2);//B
				*(t + 3) = *(scanbuffer + 3);//Alpha
				scanbuffer = scanbuffer + 4;
				(*res)[i][j] = tmp;
			}
		}
		break;
	}

	return res;
}
unsigned char* PNG::BufScan(int len,unsigned char* bufin, int type) {
	int k = 1;
	int colors;
	if (colorType == 0 || colorType==3) colors = 1;
	if (colorType == 2) colors = 3;
	if (colorType == 4) colors = 2;
	if (colorType == 6) colors = 4;
	int BytesPerPixel = std::max(1,colors * depth / 8);
	int BytesPerRow = BytesPerPixel * width;
	std::cout << BytesPerRow << std::endl;
	int offset =  0;
	int length = BytesPerPixel * width * height;
	unsigned char *line = new unsigned char[BytesPerRow];
	unsigned char *bufout = new unsigned char[len];
	memset(bufout, 0, length);
	for (int i = 0; i < len; i += BytesPerRow+1) {
		memcpy(line, bufin + k + offset, BytesPerRow);
		uint16_t type = (uint16_t)bufin[i];
		std::cout << type << std::endl;
		switch (type) {
		case 0:
			FilterNone(bufout,line, BytesPerPixel, BytesPerRow, offset);
			break;
		case 1:
			FilterSub(bufout ,line, BytesPerPixel, BytesPerRow, offset);
			break;
		case 2:
			FilterUp(bufout,line, BytesPerPixel, BytesPerRow, offset);
			break;
		case 3:
			FilterAvg(bufout,line, BytesPerPixel, BytesPerRow, offset);
			break;
		case 4:
			FilterPaeth(bufout,line, BytesPerPixel, BytesPerRow, offset);
			break;
		default:
			std::cout << "Unknown Type" << std::endl;
			break;
		}
		offset += BytesPerRow;
		k++;
	}
	return bufout;
}

///////////////////////////////////////////////////////////////////////////////////////
// x 是我们操作的对象

// c b
// a x


//不做任何操作，直接放上去
void PNG::FilterNone(unsigned char* bufout, unsigned char*  line, int BytesPerPixel, int BytesPerRow, int offset) {
	for (int i = 0; i < BytesPerRow; i++) {//对每一行循环
		bufout[offset + i] = line[i];//按位置放上去
	}
}

//和b作差,当x在最顶端时不做操作
void PNG::FilterUp(unsigned char* bufout, unsigned char*  line, int BytesPerPixel, int BytesPerRow, int offset) {
	if (offset < BytesPerRow) {//如果是第一行
		for (int i = 0; i < BytesPerRow; i++) {//不做操作，按位置放上去
			bufout[offset + i] = line[i];
		}
	}
	else {//不是第一行
		for (int i = 0; i < BytesPerRow; i++) {
			unsigned char tmp = bufout[offset + i - BytesPerRow];//这是b
			unsigned char v = line[i] + tmp;//原x = 现x + b；
			bufout[offset + i] = v & 0xff;//防止溢出
		}
	}
}

//和a做差，当x在最左边时不做操作
void PNG::FilterSub(unsigned char* bufout, unsigned char*  line, int BytesPerPixel, int BytesPerRow, int offset) {
	for (int i = 0; i < BytesPerRow; i++) {
		if (i < BytesPerPixel) {//如果是第一个像素
			bufout[offset + i] = line[i];//直接放上去
		}
		else {//不是第一个像素
			unsigned char tmp = bufout[offset + i - BytesPerPixel];//这是a
			unsigned char v = line[i] + tmp;// x = x' + a
			bufout[offset + i] = v & 0xff;
		}
	}
}

//取 a b 的平均值，与这个平均值作差。 如果x在最左上角，不做操作，x在最顶端，只和a的1/2作差，x在最左边，只和b的1/2作差
void PNG::FilterAvg(unsigned char* bufout, unsigned char*  line, int BytesPerPixel, int BytesPerRow, int offset) {
	if (offset < BytesPerRow) {//第一行
		for (int i = 0; i < BytesPerRow; i++) {
			if (i < BytesPerPixel) {//第一列
				bufout[offset + i] = line[i];//直接放
			}
			else {//不是第一列
				unsigned char tmp = bufout[offset + i - BytesPerPixel];//这是 a
				unsigned char v = line[i] + (tmp>>1);//x = x’ + a
				bufout[offset + i] = v & 0xff;
			}
		}
	}
	else {//不是第一行
		for (int i = 0; i < BytesPerRow; i++) {
			if (i < BytesPerPixel) {//第一列
				unsigned char tmp = bufout[offset + i - BytesPerRow];//这是 b
				unsigned char v = line[i] + (tmp>>1);//x = x‘ + b
				bufout[offset + i] = v & 0xff;
			}
			else {//不是第一列
				unsigned char tmpa = bufout[offset + i - BytesPerPixel];//a
				unsigned char tmpb = bufout[offset + i - BytesPerRow];//b

				unsigned char v = line[i] + ((tmpa + tmpb) >> 1);//x = x' +（a+b）/2
				bufout[offset + i] = v & 0xff;
			}
		}
	}
}

// p = a+b-c
//pa = abs(p-a)
//pb = abs(p-b)
//pc = abs(p-c)
//if(pa<=pb && pa<=pc) pr = a;
//else if(pb<=pc) pr=b;
//else pr = c
// x = x-c;
void PNG::FilterPaeth(unsigned char* bufout, unsigned char*  line, int BytesPerPixel, int BytesPerRow, int offset) {
	if (offset < BytesPerRow) {//第一行
		for (int i = 0; i < BytesPerRow; i++) {
			if (i < BytesPerPixel) {//第一列
				bufout[offset + i] = line[i];
			}
			else {//不是第一列
				unsigned char tmp = bufout[offset + i - BytesPerPixel];//a
				unsigned char v = line[i] + tmp;//x = x' + a
				bufout[offset + i] = v & 0xff;
			}
		}
	}
	else {//不是第一行
		for (int i = 0; i < BytesPerRow; i++) {
			if (i < BytesPerPixel) {//第一列
				unsigned char tmp = bufout[offset + i - BytesPerRow];//b
				unsigned char v = line[i] + tmp;//x = x' + b
				bufout[offset + i] = v & 0xff;
			}
			else {//不是第一列
				unsigned char a = bufout[offset + i - BytesPerPixel];//a
				unsigned char b = bufout[offset + i - BytesPerRow];//b
				unsigned char c = bufout[offset + i - BytesPerPixel - BytesPerRow];//c

			//	int p = a + b - c;
				int pa = abs(b-c);
				int pb = abs(a-c);
				int pc = abs(a+b-c-c);
				unsigned char pr;

				if (pa <= pb && pa <= pc) pr = a;
				else if (pb <= pc) pr = b;
				else pr = c;

				unsigned char v = line[i] + pr;//x = x' + pr
				bufout[offset + i] = v & 0xff;
			}
		}
	}
}