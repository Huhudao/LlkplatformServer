#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <mysql/mysql.h>
#include <boost/noncopyable.hpp>

class Account: public boost::noncopyable{
private:
	int gameAll;
	int gameWin;
	unsigned int id;
	std::string name;
public:
	Account(): id(0), gameAll(-1), gameWin(-1), name("admin"){}
	
	Account(unsigned int uid, int ga, int gw, const char *nm): id(uid), gameAll(ga), gameWin(gw), name(nm){}
	
	~Account(){}

	unsigned int getId();
	int getAll();
	int getWin();
	std::string getName();
	void getData(MYSQL *mysql);
	void plusAll();
	void plusWin();
	void logIn(unsigned int uid, int ga, int gw, const char *nm);
	void logOut(MYSQL *mysql);
};
#endif
