#include <string.h>
#include <stdio.h>
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
	writeInd += len;
}

void Buffer::writeToFile(FILE *file){
	assert(writeInd <= maxLength);
	assert(file != NULL);
	if(writeInd <= 0) return;
	buff[writeInd++] = '\0';
	printf("%s", buff);
	fprintf(file, buff);
	fflush(file);
	writeInd = 0;
}
