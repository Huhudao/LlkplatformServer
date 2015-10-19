#ifndef DBCONNPOOL_H
#define DBCONNPOOL_H

#include <set>
#include <deque>
#include <mysql/mysql.h>
#include <boost/noncopyable.hpp>

#include "Mutex.h"

class DBConnPool: public boost::noncopyable{
private:
	size_t initSize;
	Mutex mutex;
	std::deque<MYSQL*> spare;
	std::set<MYSQL*> working;
public:
	DBConnPool();
	DBConnPool(size_t sz);
	~DBConnPool();

	void start();
	void put(MYSQL* mysql);
	MYSQL* take();
};
#endif
