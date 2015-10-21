#include "helps.h"

void mod(int &a, const int &m){
	while(a >= m){
		a -= m;
	}
}

std::string uitos(unsigned int v){
	if(v == 0){
		return "0";
	}
	char str[15];
	int p = 0;
	memset(str, 0, sizeof(str));
	while(v > 0){
		str[p++] = v % 10 + '0';
		v /= 10;
	}
	for(int i = 0, j = p - 1; i < p / 2; i++, j--){
		std::swap(str[i], str[j]);
	}
	return std::string(str);
}

