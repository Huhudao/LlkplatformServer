#include <stdlib.h>
#include "LlkInfo.h"
#include "../tools/Log.h"
#include "../tools/helps.h"

LlkInfo::LlkInfo(): row(dftRow), col(dftCol){
}

LlkInfo::LlkInfo(size_t n, size_t m): row(n), col(m){
}

void LlkInfo::init(){
	logger.logDebug("init llk game.");
	picRemain = row * col;
	pictures = vector<vector<int> >(row, vector<int>(col, -1));
	canGo = vector<vector<vector<int> > >(row, vector<vector<int> >(col, vector<int>(4)));
	for(size_t i = 0; i < row; i++){
		for(size_t j = 0; j < col; j++){
			canGo[i][j][LlkInfo::up] = i - 1;
			canGo[i][j][LlkInfo::down] = i + 1;
			canGo[i][j][LlkInfo::left] = j - 1;
			canGo[i][j][LlkInfo::right] = j + 1;
		}
	}
	buildPics();
}

void LlkInfo::buildPics(){
	vector<int> picIds(picNum * mxNumPerPic);
	for(size_t i = 0; i < picIds.size(); i++){
		picIds[i] = rand() % picNum;
	}
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
	for(size_t i = 0; i < row; i++){
		for(size_t j = 0; j < col; j++){
			printf("%d ", pictures[i][j]);
		}
		puts("");
	}
}

void LlkInfo::setSize(size_t n, size_t m){
	row = n, col = m;
	init();
}

pair<bool, bool> LlkInfo::link(size_t x1, size_t y1, size_t x2, size_t y2){
	if(x1 == x2 && y1 == y2){
		logger.logError("two same coordinates.");
		return make_pair(0, 0);
	}
	logger.logDebug("user linking.");
	pair<bool, bool> res = make_pair(0, 0);
	if(x1 < 0 || x1 >= row || x2 < 0 || x2 >= row) return res;
	if(y1 < 0 || y1 >= col || y2 < 0 || y2 >= col) return res;
	if(pictures[x1][y1] < 0 || pictures[x2][y2] < 0 || pictures[x1][y1] != pictures[x2][y2]){
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
	logger.logDebug("link horizontal.");
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
	logger.logDebug("link vertical.");
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
	logger.logDebug("updating game.");
	for(size_t i = x - 1; i >= 0; i--){
		canGo[i][y][LlkInfo::down] = canGo[x][y][LlkInfo::down];
		if(pictures[i][y] >= 0) break;
	}
	for(size_t i = x + 1; i < row; i++){
		canGo[i][y][LlkInfo::up] = canGo[x][y][LlkInfo::up];
		if(pictures[i][y] >= 0) break;
	}
	for(size_t j = y - 1; j >= 0; j--){
		canGo[x][j][LlkInfo::right] = canGo[x][y][LlkInfo::right];
		if(pictures[x][j] >= 0) break;
	}
	for(size_t j = y + 1; j < col; j++){
		canGo[x][j][LlkInfo::left] = canGo[x][y][LlkInfo::left];
		if(pictures[x][j] >= 0) break;
	}
}

string LlkInfo::picsToStr(){
	char str[row * col * 5 + 15];
	int pos = 0;
	memset(str, 0, sizeof(str));
	sprintf(str + pos, "%d\n%d\n", row, col);
	while(str[pos] != '\0') pos++;
	for(size_t i = 0; i < row; i++){
		for(size_t j = 0; j < col; j++){
			sprintf(str + pos, "%d\n", pictures[i][j]);
			while(str[pos] != '\0') pos++;
		}
	}
	return string(str);
}
