#ifndef THREAD_H
#define THREAD_H

#include <string>
#include <assert.h>
#include <pthread.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>

class Thread: public boost::noncopyable {
public:
	typedef boost::function<void ()> Func;
private:
	bool started;
	bool joined;
	pthread_t tid;
	Func run;
	std::string name;

	static void* startThread(void *val);
public:
	//此处必须加const
	explicit Thread(const Func &func, const std::string &nm);
	~Thread();

	void start();
	void join();
	pthread_t getTid();
};
#endif
