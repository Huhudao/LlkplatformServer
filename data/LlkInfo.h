#ifndef LLKINFO_H
#define LLKINFO_H

#include <vector>
#include <string>
#include <utility>
#include <string.h>
#include <assert.h>

#include "../tools/Mutex.h"
#include "../tools/Condition.h"

using namespace std;

class LlkInfo{
private:
	static const int picNum = 30;
	static const size_t dftRow = 16;
	static const size_t dftCol = 16;
	static const int mxNumPerPic = 10;
	size_t row, col;
	int picRemain;
	//Mutex mutex;
	vector<vector<int> > pictures;
	vector<vector<vector<int> > > canGo;
public:
	enum Direction{
		up,
		down,
		left,
		right
	};
	LlkInfo();
	LlkInfo(size_t n, size_t m);
	~LlkInfo(){}

	void init();
	void buildPics();
	void setSize(size_t n, size_t m);
	pair<bool, bool> link(size_t x1, size_t y1, size_t x2, size_t y2);
	bool canGetHor(size_t x1, size_t y1, size_t x2, size_t y2);
	bool canGetVer(size_t x1, size_t y1, size_t x2, size_t y2);
	void update(size_t x, size_t y);
	string picsToStr();
};
#endif
