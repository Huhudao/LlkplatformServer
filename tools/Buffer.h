#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <assert.h>

class Buffer{
private:
	static const size_t maxLength = (1 << 20);
	char *buff;
	size_t readInd;
	size_t writeInd;
public:
	Buffer(){
		buff = new char[maxLength + 1];
		readInd = writeInd = 0;
	}

	Buffer(char *fl){
		buff = new char[maxLength + 1];
		readInd = writeInd = 0;
	}

	~Buffer(){
		if(buff != NULL) delete buff;
	}
	
	size_t size();
	size_t sizeRemain();
	void append(const char *val, int len);
	void writeToFile(FILE *file);
};

#endif
