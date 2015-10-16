#include <boost/bind.hpp>

#include "Log.h"
#include "helps.h"

Log::Log():
	curr(new Buffer()),
	next(new Buffer()),
	mutex(),
	hasBuffer(mutex),
	thread(boost::bind(&Log::threadFunc, this), std::string("Logger")) {
	//running = true;
	level = DEBUG;
	file = fopen("/var/Llkplatform/llk.log", "w");
	assert(file != NULL);
	buffers.clear();
}

Log::Log(LogLevel lvl, char *path):
	curr(new Buffer()),
	next(new Buffer()),
	mutex(),
	hasBuffer(mutex),
	thread(boost::bind(&Log::threadFunc, this), std::string("Logger")) {
	//running = true;
	level = lvl;
	file = fopen(path, "w");
	assert(file != NULL);
	buffers.clear();
}

void Log::setLevel(LogLevel lvl){
	level = lvl;
}

void Log::setFile(char *path){
	if(file != NULL){
		fclose(file);
	}
	file = fopen(path, "w");
	assert(file != NULL);
}

void Log::logMessage(const char *val, int len){
	MutexLockGuard lock(mutex);
	if(curr->sizeRemain() >= len){
		curr->append(val, len);
	}
	else{
		buffers.push_back(curr);
		if(next){
			movePtr(curr, next);
		}
		else{
			curr.reset(new Buffer());
		}
		curr->append(val, len);
		hasBuffer.notify();
	}
}

void Log::logDebug(const char *val, int len){
	if(DEBUG >= level) logMessage(val, len);
}

void Log::logInfo(const char *val, int len){
	if(INFO >= level) logMessage(val, len);
}

void Log::logWarn(const char *val, int len){
	if(WARN >= level) logMessage(val, len);
}

void Log::logError(const char *val, int len){
	if(ERROR >= level) logMessage(val, len);
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
		//assert(file != NULL);
		MutexLockGuard lock(mutex);
		if(buffers.empty()){
			hasBuffer.waitForSeconds(seconds);
		}
		buffers.push_back(curr);
		movePtr(curr, spare1);
		std::swap(buffers, buffersToWrite);
		if(!next){
			movePtr(next, spare2);
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
