#include "Thread.h"

Thread::Thread(const Func &func, const std::string &nm):started(false), joined(false), run(func), name(nm){
}

Thread::~Thread(){
	if(started && !joined){
		pthread_detach(tid);
	}
}

void Thread::start(){
	assert(!started);
	started = true;
	int ret = pthread_create(&tid, NULL, startThread, this);
	assert(ret == 0);
}

void Thread::join(){
	assert(started);
	assert(!joined);
	joined = true;
	int ret = pthread_join(tid, NULL);
	assert(ret == 0);
}

pthread_t Thread::getTid(){
	return tid;
}

void* Thread::startThread(void *val){
	Thread *thread = static_cast<Thread*>(val);
	thread->run();
	return NULL;
}
