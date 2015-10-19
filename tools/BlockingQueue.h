#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H

#include <deque>
#include <assert.h>
#include <boost/noncopyable.hpp>

#include "Mutex.h"
#include "Condition.h"

template <typename T>
class BlockingQueue: public boost::noncopyable{
private:
	Mutex mutex;
	Condition notEmpty;
	std::deque<T> queue;
public:
	BlockingQueue(): mutex(), notEmpty(mutex), queue(){
	}

	~BlockingQueue(){}

	void put(const T &val);
	void clear();
	T take();
	size_t size();
	bool empty();
};
#endif
