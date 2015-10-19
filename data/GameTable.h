#ifndef GAMETABLE_H
#define GAMETABLE_H

#include <set>
#include <map>
#include <string>
#include <utility>
#include <assert.h>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "LlkInfo.h"
#include "../tools/Mutex.h"
#include "../tools/Condition.h"

class User;

class GameTable: public boost::noncopyable{
public:
	typedef boost::shared_ptr<GameTable> TablePtr;
private:
	static Mutex mutexNum;
	static int tableNum;
	static std::map<int, TablePtr> tables;
	bool started;
	int tableId;
	int pos;
	int playerNum;
	int readyNum;
	Mutex mutex;
	LlkInfo game;
	std::set<User*> users;
public:
	GameTable(int id, int p, int n);

	static TablePtr getTable(int id);
	static void put(TablePtr table);
	static TablePtr createTable(int p, int n);
	static std::string allTables();
	bool isStarted();
	bool attach(User *u);
	std::string tableInfo();
	void detach(User *u);
	void getReady(User *u);
	void unReady(User *u);
	void chat(User *u, const char *val);
	void link(User *u, size_t x1, size_t y1, size_t x2, size_t y2);
	void end(User *u);
private:
	static void eraseTable(int id);
	void startGame();
	void endGame();
	void broadCast(std::string val);
};
#endif
