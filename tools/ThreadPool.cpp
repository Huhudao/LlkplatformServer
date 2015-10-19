#include "ThreadPool.h"

void ThreadPool::start(){
	MutexLockGuard lock(mutex);
	assert(threads.empty());
	assert(taskQueue.empty());
	running = true;
	for(size_t i = 0; i < maxNumThread; i++){
		ThreadPtr thread(new Thread(boost::bind(&ThreadPool::threadFunc, this), "InPool"));
		threads.push_back(thread);
	}
}

void ThreadPool::stop(){
	{
		MutexLockGuard lock(mutex);
		running = false;
		notEmpty.notifyAll();
	}
	for(size_t i = 0; i < threads.size(); i++){
		threads[i]->join();
	}
}

void ThreadPool::runTask(Task task){
	MutexLockGuard lock(mutex);
	if(taskQueue.size() < maxNumTask){
		taskQueue.push_back(task);
		notEmpty.notify();
	}
}

ThreadPool::Task ThreadPool::take(){
	MutexLockGuard lock(mutex);
	while(running && taskQueue.empty()){
		notEmpty.wait();
	}
	Task task;
	if(!taskQueue.empty()){
		task = taskQueue.front();
		taskQueue.pop_front();
	}
	return task;
}

void ThreadPool::threadFunc(){
	while(running){
		Task task(take());
		if(task){
			task();
		}
	}
}
