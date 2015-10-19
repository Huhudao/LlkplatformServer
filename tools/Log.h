#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "Mutex.h"
#include "Buffer.h"
#include "Thread.h"
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
	typedef boost::shared_ptr<Buffer> BufferPtr;
	static const int seconds = 5;
	bool running;
	LogLevel level;
	pthread_t threadId;
	FILE *file;
	BufferPtr curr, next;
	Mutex mutex;
	Condition hasBuffer;
	std::vector<BufferPtr> buffers;
	Thread thread;
public:
	Log();

	Log(LogLevel lvl, char *path);

	~Log(){
		if(running){
			running = false;
			hasBuffer.notify();
			thread.join();
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
	void threadFunc();
	void logMessage(const char *val, int len);
};

extern Log logger;
#endif
