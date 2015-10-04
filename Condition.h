#ifndef CONDITION_H
#define CONDITION_H

#include <pthread.h>
#include <boost/noncopyable.hpp>

class Condition:public boost::noncopyable{
private:
	Mutex &mutex;
	pthread_cond_t condition;
public:
	explicit Condition(Mutex &mutexP){
		mutex = mutexP;
		pthread_cond_init(&condition, NULL);
	}

	~Condition(){
		pthread_cond_destroy(&condition);
	}

	void wait(){
		pthread_cond_wait(&condition, mutex.getMutex());
	}

	void notify(){
		pthread_cond_signal(&condition);
	}

	void notifyAll(){
		pthread_cond_broadcast(&condition);
	}
	bool waitForSeconds(int seconds);
}
