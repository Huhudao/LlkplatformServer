#include <assert.h>

#include "DBConnPool.h"
#include "Log.h"

DBConnPool::DBConnPool(): mutex(){
	initSize = 100;
}

DBConnPool::DBConnPool(size_t sz): mutex(){
	initSize = sz;
}

DBConnPool::~DBConnPool(){
	MutexLockGuard lock(mutex);
	while(!spare.empty()){
		MYSQL *mysql = spare.front();
		spare.pop_front();
		mysql_close(mysql);
	}
	for(std::set<MYSQL*>::iterator it = working.begin(); it != working.end(); it++){
		mysql_close(*it);
	}
}

void DBConnPool::start(){
	logger.logInfo("DBConnPool starting...");
	MutexLockGuard lock(mutex);
	assert(spare.empty());
	assert(working.empty());
	for(size_t i = 0; i < initSize; i++){
		MYSQL *mysql = mysql_init(NULL);
		spare.push_back(mysql);
	}
}

void DBConnPool::put(MYSQL *mysql){
	logger.logInfo("put conn into pool.");
	MutexLockGuard lock(mutex);
	if(working.find(mysql) == working.end()){
		logger.logError("putting an exsited conn into pool.");
	}
	else{
		working.erase(mysql);
		spare.push_back(mysql);
	}
}

MYSQL* DBConnPool::take(){
	logger.logDebug("take a conn from pool.");
	MutexLockGuard lock(mutex);
	MYSQL *mysql;
	if(spare.empty()){
		mysql = mysql_init(NULL);
	}
	else{
		mysql = spare.front();
		spare.pop_front();
	}
	working.insert(mysql);
}
