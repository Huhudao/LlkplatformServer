#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <assert.h>
#include <vector>
#include <deque>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "Mutex.h"
#include "Thread.h"
#include "Condition.h"

class ThreadPool: public boost::noncopyable {
public:
	typedef boost::function<void ()> Task;
	typedef boost::shared_ptr<Thread> ThreadPtr;
private:
	static const size_t defaultSize = 100;
	bool running;
	size_t maxNumThread;
	size_t maxNumTask;
	Mutex mutex;
	Condition notEmpty;
	std::vector<ThreadPtr> threads;
	std::deque<Task> taskQueue;

public:
	ThreadPool(): mutex(), notEmpty(mutex), threads(), taskQueue() {
		assert(threads.empty());
		assert(taskQueue.empty());
		running = false;
		maxNumThread = maxNumTask = defaultSize;
	}

	ThreadPool(size_t size): mutex(), notEmpty(mutex), threads(), taskQueue() {
		assert(threads.empty());
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
	void runTask(Task task);

private:
	Task take();
	void threadFunc();
};
#endif
