#ifndef MUTEX_H
#define MUTEX_H
#include <pthread.h>
#include <assert.h>
#include <boost/noncopyable.hpp>

class Mutex:public boost::noncopyable{
private:
	pthread_mutex_t mutex;
public:
	Mutex(){
		pthread_mutex_init(&mutex, NULL);
	}
	~Mutex(){
		pthread_mutex_destroy(&mutex);
	}
	lock(){
		pthread_mutex_lock(&mutex);
	}
	unlock(){
		pthread_mutex_unlock(&mutex);
	}
	pthread_mutex_t* getMutex(){
		return &mutex;
	}
};

class MutexLockGuard:public boost::noncopyable{
private:
	Mutex &mutex;
	explicit MutexLockGuard(Mutex &mutexP){
		mutex = mutexP;
		mutex.lock();
	}
	~MutexLockGaurd(){
		mutex.unlock();
	}
};
#endif
