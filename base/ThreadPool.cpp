#include "ThreadPool.h"

void ThreadPool::start(){
	assert(threadIds.empty());
	assert(taskQueue.empty());
	MutexLockGuard lock(mutex);
	running = true;
	for(size_t i = 0; i < maxSize; i++){
		pthread_t thid;
		int ret = pthread_create(&thid, NULL, ThreadPool::threadFunc, NULL);
		assert(ret == 0);
		threadIds.push_back(thid);
	}
}

void ThreadPool::stop(){
	{
		MutexLockGuard lock(mutex);
		running = false;
		notEmpty.notifyAll();
	}
/*	for(size_t i = 0; i < threadIds.size(); i++){
		pthread_join(threadIds[i], NULL);
	}
*/
}

template <class T>
void ThreadPool::runTask(T task){
	MutexLockGuard lock(mutex);
	if(taskQueue.size() < maxNumTask){
		taskQueue.push_back(task);
		notEmpty.notify();
	}
}

template <class T>
T ThreadPool::take(){
	MutexLockGuard lock(mutex);
	while(taskQueue.empty()){
		notEmpty.wait();
	}
	T val(taskQueue.front());
	taskQueue.pop_front();
	return T;
}

template <class T>
void* ThreadPool::threadFunc(){
	while(running){
		T task = take();
		task->doJob();
	}
}
