#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "Mutex.h"
#include "Buffer.h"
#include "Condition.h"
#include "BlockingQueue.h"

class Log{
public:
	enum LogLevel{
		DEBUG,
		INFO,
		WARN,
		ERROR,
		NOTHING
	};
private:
	typedef boost::shared_ptr<Buffer> SharedBuffer;
	static const int seconds = 5;
	bool running;
	LogLevel level;
	pthread_t threadId;
	FILE *file;
	SharedBuffer curr, next;
	Mutex mutex;
	Condition hasBuff;
	std::vector<SharedBuffer> buffers;
public:
	Log(): curr(new Buffer()), next(new Buffer()), mutex(), hasBuff(mutex){
		//running = true;
		level = DEBUG;
		file = fopen("/var/Llkplatform/llk.log", "w");
		assert(file != NULL);
		buffers.clear();
	}

	Log(LogLevel lvl, char *path): curr(new Buffer()), next(new Buffer()), mutex(), hasBuff(mutex){
		//running = true;
		level = lvl;
		file = fopen(path, "w");
		assert(file != NULL);
		buffers.clear();
	}

	~Log(){
		if(running){
			running = false;
			hasBuffer.notify();
		}
		if(file != NULL) fclose(file);
	}

	void setLevel(LogLevel lvl);
	void setFile(char *path);
	void logDebug(const char *val, int len);
	void logInfo(const char *val, int len);
	void logWarn(const char *val, int len);
	void logError(const char *val, int len);
	void start();

private:
	void* threadFunc();
	void logMessage(const char *val, int len);
};
#endif
