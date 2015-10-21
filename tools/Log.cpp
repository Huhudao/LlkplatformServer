#include <utility>
#include <time.h>
#include <sys/time.h>
#include <boost/bind.hpp>

#include "Log.h"
#include "Mutex.h"
#include "Condition.h"
#include "Buffer.h"
#include "Thread.h"
#include "helps.h"

Log::Log():
	curr(new Buffer()),
	next(new Buffer()),
	mutex(),
	hasBuffer(mutex),
	thread(boost::bind(&Log::threadFunc, this), std::string("Logger")) {
	level = DEBUG;
	file = fopen("./llk.log", "a+");
	assert(file != NULL);
	buffers.clear();
}

Log::Log(LogLevel lvl, char *path):
	curr(new Buffer()),
	next(new Buffer()),
	mutex(),
	hasBuffer(mutex),
	thread(boost::bind(&Log::threadFunc, this), std::string("Logger")) {
	level = lvl;
	file = fopen(path, "a+");
	assert(file != NULL);
	buffers.clear();
}

void Log::setLevel(LogLevel lvl){
	MutexLockGuard lock(mutex);
	level = lvl;
}

void Log::setFile(char *path){
	MutexLockGuard lock(mutex);
	if(file != NULL){
		fclose(file);
	}
	file = fopen(path, "a+");
	assert(file != NULL);
}

void Log::logMessage(const char *type, const char *val){
	MutexLockGuard lock(mutex);
	int len = strlen(type) + strlen(val);
	time_t curtm = time(0);
	struct tm *now = localtime(&curtm);
	char buf[50 + len];
	sprintf(buf, "%d-%2d-%2d %2d:%2d:%2d# %s%s\n", now->tm_year + 1900,
			now->tm_mon + 1,
			now->tm_mday,
			now->tm_hour,
			now->tm_min,
			now->tm_sec,
			type,
			val);
	len = strlen(buf);
	if(curr->sizeRemain() >= len){
		curr->append(buf, len);
	}
	else{
		buffers.push_back(curr);
		if(next){
			curr = std::move(next);
		}
		else{
			curr.reset(new Buffer());
		}
		curr->append(buf, len);
		hasBuffer.notify();
	}
}

void Log::logDebug(const char *val){
	if(DEBUG >= level) logMessage("(Debug) ", val);
}

void Log::logInfo(const char *val){
	if(INFO >= level) logMessage("(Info) ", val);
}

void Log::logWarn(const char *val){
	if(WARN >= level) logMessage("(Warn) ", val);
}

void Log::logError(const char *val){
	if(ERROR >= level) logMessage("(Error) ", val);
}

void Log::start(){
	running = true;
	thread.start();
}

void Log::threadFunc(){
	BufferPtr spare1(new Buffer()), spare2(new Buffer());
	std::vector<BufferPtr> buffersToWrite(0);
	while(running){
		assert((spare1) && spare1->size() == 0);
		assert((spare2) && spare2->size() == 0);
		assert(buffersToWrite.empty());
		assert(file != NULL);
		MutexLockGuard lock(mutex);
		if(buffers.empty()){
			hasBuffer.waitForSeconds(seconds);
		}
		buffers.push_back(curr);
		curr = std::move(spare1);
		std::swap(buffers, buffersToWrite);
		if(!next){
			next = std::move(spare2);
		}

		assert(!buffersToWrite.empty());
		for(size_t i = 0; i < buffersToWrite.size(); i++){
			buffersToWrite[i]->writeToFile(file);
		}
		if(!spare1){
			assert(buffersToWrite.size() > 0);
			spare1 = buffersToWrite[0];
		}
		if(!spare2){
			assert(buffersToWrite.size() > 1);
			spare2 = buffersToWrite[1];
		}
		buffersToWrite.clear();
	}
}
