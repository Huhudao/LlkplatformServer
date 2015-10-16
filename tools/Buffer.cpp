#include <string.h>
#include "Buffer.h"

size_t Buffer::size(){
	return writeInd;
}

size_t Buffer::sizeRemain(){
	return maxLength - writeInd;
}

void Buffer::append(const char *val, int len){
	assert(sizeRemain() - len >= 0);
	memcpy(buff + writeInd, val, sizeof(char) * len);
}

void Buffer::writeToFile(FILE *file){
	assert(writeInd <= maxLength);
	if(writeInd <= 0) return;
	buff[writeInd++] = '\0';
	fprintf(file, buff);
	writeInd = 0;
}
