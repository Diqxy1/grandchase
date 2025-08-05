#pragma once 

namespace AdlerCheckSum
{
	unsigned int adler32(unsigned int adler, const unsigned char *buf, unsigned int len);
	unsigned int adler32_combine(unsigned int adler1, unsigned int adler2, unsigned int len2);

	unsigned int adler32(const char* filename,unsigned int *size=0);
	unsigned int adler32(std::istream &stream,int offset,int length,bool resetposition=true);
}
