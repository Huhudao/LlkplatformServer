#include <stdlib.h>
#include "LlkInfo.h"

LlkInfo::LlkInfo(): row(dftRow), col(dftCol){
}

LlkInfo::LlkInfo(size_t n, size_t m): row(n), col(m){
}

void LlkInfo::init(){
	//MutexLockGuard lock(mutex);
	picRemain = row * col;
	pictures = vector<vector<int> >(row, vector<int>(col, -1));
	canGo = vector<vector<vector<int> > >(row, vector<vector<int> >(col, vector<int>(4)));
	for(size_t i = 0; i < row; i++){
		for(size_t j = 0; j < col; j++){
			canGo[i][j][LlkInfo::up] = canGo[i][j][LlkInfo::down] = i;
			canGo[i][j][LlkInfo::left] = canGo[i][j][LlkInfo::right] = j;
		}
	}
	buildPics();
}

void LlkInfo::buildPics(){
	vector<int> picIds(picNum * mxNumPerPic);
	for(size_t i = 0; i < picIds.size(); i++){
		size_t j = rand() % picIds.size();
		if(i != j){
			swap(picIds[i], picIds[j]);
		}
	}
	vector<pair<size_t, size_t> > cors(row * col);
	for(size_t i = 0; i < row; i++){
		for(size_t j = 0; j < col; j++){
			cors[i * col + j] = make_pair(i, j);
		}
	}
	for(size_t i = 0; i < cors.size(); i++){
		int j = rand() % cors.size();
		if(i != j){
			swap(cors[i], cors[j]);
		}
	}
	assert(picIds.size() * 2 >= cors.size());
	for(size_t i = 0; i < cors.size(); i++){
		pictures[cors[i].first][cors[i].second] = picIds[i / 2];
	}
}

void LlkInfo::setSize(size_t n, size_t m){
	//MutexLockGuard lock(mutex);
	row = n, col = m;
	init();
}

pair<bool, bool> LlkInfo::link(size_t x1, size_t y1, size_t x2, size_t y2){
	assert(x1 != x2 || y1 != y2);
	//MutexLockGuard lock(mutex);
	pair<bool, bool> res = make_pair(0, 0);
	if(x1 < 0 || x1 >= row || x2 < 0 || x2 >= row) return res;
	if(y1 < 0 || y1 >= col || y2 < 0 || y2 >= col) return res;
	if(pictures[x1][y1] < 0 || pictures[x2][y2] < 0){
		res.second = (picRemain == 0);
		return res;
	}
	if(canGetHor(x1, y1, x2, y2)){
		res.first = true;
	}
	else if(canGetVer(x1, y1, x2, y2)){
		res.first = true;
	}
	if(res.first){
		pictures[x1][y1] = pictures[x2][y2] = -1;
		update(x1, y1);
		update(x2, y2);
		picRemain--;
	}
	if(picRemain == 0){
		res.second = true;
	}
	return res;
}

bool LlkInfo::canGetHor(size_t x1, size_t y1, size_t x2,size_t y2){
	if(y1 > y2){
		swap(x1, x2);
		swap(y1, y2);
	}
	for(size_t j = y1; j <= y2; j++){
		if(canGo[x1][j][LlkInfo::left] <= y1
				&& canGo[x2][j][LlkInfo::right] >= y2
				&& (x1 < x2 ? canGo[x1][j][LlkInfo::down] >= x2 : canGo[x1][j][LlkInfo::up] <= x2)){
			return true;
		}
	}
	return false;
}

bool LlkInfo::canGetVer(size_t x1, size_t y1, size_t x2, size_t y2){
	if(x1 > x2){
		swap(x1, x2);
		swap(y1, y2);
	}
	for(size_t i = x1; i < x2; i++){
		if(canGo[i][y1][LlkInfo::up] <= x1
				&& canGo[i][y2][LlkInfo::down] >= y2
				&& (y1 < y2 ? canGo[i][y1][LlkInfo::right] >= y2 : canGo[i][y1][LlkInfo::left] <= y2)){
			return true;
		}
	}
	return false;
}

void LlkInfo::update(size_t x, size_t y){
	int previ = (x + 1 < row ? x + 1 : x);
	for(size_t i = x; i >= 0; i--){
		canGo[i][y][LlkInfo::down] = canGo[previ][y][LlkInfo::down];
		if(pictures[i][y] >= 0) break;
	}
	previ = (x - 1 >= 0 ? x - 1 : x);
	for(size_t i = x; i < row; i++){
		canGo[i][y][LlkInfo::up] = canGo[previ][y][LlkInfo::up];
		if(pictures[i][y] >= 0) break;
	}
	int prevj = (y + 1 < col ? y + 1 : y);
	for(size_t j = y; j >= 0; j--){
		canGo[x][j][LlkInfo::right] = canGo[x][prevj][LlkInfo::right];
		if(pictures[x][j] >= 0) break;
	}
	prevj = (y - 1 >= 0 ? y - 1 : y);
	for(size_t j = y; j < col; j++){
		canGo[x][j][LlkInfo::left] = canGo[x][prevj][LlkInfo::left];
		if(pictures[x][j] >= 0) break;
	}
}
