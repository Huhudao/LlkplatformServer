#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <assert.h>

#include "Mutex.h"
#include "Condition.h"
#include "BlockingQueue.h"

class Log{
public:
	enum LogLevel{
		DEBUG,
		INFO,
		WARN,
		ERROR
	};
private:
	
};
#endif
