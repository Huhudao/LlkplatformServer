#include "BlockingQueue.h"

template <class T>
void BlockingQueue<T>::clear(){
	MutexLockGuard lock(mutex);
	queue.clear();
}

template <class T>
void BlockingQueue<T>::put(const T &val){
	MutexLockGuard lock(mutex);
	queue.push_back(val);
	notEmpty.notify();
}

template <class T>
T BlockingQueue<T>::take(){
	MutexLockGuard lock(mutex);
	while(queue.empty()){
		notEmpty.wait();
	}
	assert(!queue.empty());
	T val(queue.front());
	queue.pop_front();
	return val;
}

template <class T>
size_t BlockingQueue<T>::size(){
	MutexLockGuard lock(mutex);
	return queue.size();
}

template <class T>
bool BlockingQueue<T>::empty(){
	MutexLockGuard lock(mutex);
	return queue.empty();
}
