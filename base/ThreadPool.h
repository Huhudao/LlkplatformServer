#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <assert.h>
#include <vector>
#include <deque>

#include "Mutex.h"
#include "Condition.h"

template <class T>
class ThreadPool{
private:
	const size_t defaultSize = 100;
	bool running;
	size_t maxNumThread;
	size_t maxNumTask;
	Mutex mutex;
	Condition notEmpty;
	std::vector<pthread_t> threadIds;
	std::deque<T> taskQueue;

public:
	ThreadPool(): mutex(), notEmpty(mutex) {
		assert(threadIds.empty());
		assert(taskQueue.empty());
		running = false;
		maxNumThread = maxNumTask = defaultSize;
	}

	ThreadPool(size_t size): mutex(), notEmpty(mutex) {
		assert(threadIds.empty());
		assert(taskQueue.empty());
		running = false;
		maxNumThread = maxNumTask = size;
	}

	~ThreadPool(){
		if(running){
			stop();
		}
	}

	void start();
	void stop();
	void runTask(T task);

private:
	T take();
	void* threadFunc();
};
#endif
