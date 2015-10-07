#include "BlockingQueue.h"

template <typename T>
void BlockingQueue::put(const T &val){
	MutexLockGuard lock(mutex);
	queue.push_back(val);
	notEmpty.notify();
}

template <typename T>
T BlockingQueue::take(){
	MutexLockQuard lock(mutex);
	while(queue.empty()){
		notEmpty.wait();
	}
	assert(!queue.empty());
	T val(queue.front());
	queue.pop_front();
	return val;
}

size_t BlockingQueue::size(){
	MutexLockGuard lock(mutex);
	return queue.size();
}
