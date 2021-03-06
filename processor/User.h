#ifndef USER_H
#define USER_H

#include <set>
#include <string>
#include <utility>
#include <time.h>
#include <mysql/mysql.h>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "Account.h"
#include "../tools/Mutex.h"
#include "../tools/DBConnPool.h"
#include "../network/ClientSocket.h"

using namespace std;

class GameTable;

class User: public boost::noncopyable{
public:
	typedef boost::shared_ptr<ClientSocket> ClientPtr;
	typedef boost::shared_ptr<GameTable> TablePtr;
private:
	static Mutex mutexUsers;
	static set<User*> users;
	static const int mxLen = 15;
	bool hasSignIn;
	bool ready;
	int score;
	MYSQL *mysql;
	clock_t lastTime;
	Mutex mutexSend;
	Mutex mutexRdSc;
	Mutex mutexAcco;
	ClientPtr sock;
	TablePtr table;
	Account account;
public:
	User(ClientPtr sk, MYSQL *sql): hasSignIn(0), ready(0), score(0),
		mysql(sql), lastTime(0), mutexSend(), mutexRdSc(),
		mutexAcco(), sock(sk), table(), account(){}
	~User(){}
	
	static void push(User *u);
	static void erase(User *u);
	static string allUsers();
	void init();
	bool isReady();
	string getName();
	void plusScore();
	int getScore();
	string gameNums();
	void win();
	void lose();
	void sendMsg(string val);
	void threadFunc();
	
private:
	bool signUp(string &name, string &pwd);
	bool signIn(std::string &name, string &pwd);
	bool sit(string &stableId, string &sp, string &sn);
	void stand(bool cksat);
	void chat(string &sval);
	void link(string &sx1, string &sy1, string &sx2, string &sy2);
	void beReady();
	void unReady();
	void echo();
	void end();
	void logOut();
	void closeSock();
	bool checkSigned();
	bool checkSat();
	void broadCast(string val);
	void destroy();
	void solve(int id, string *strs, bool &connecting);
	pair<int, int> qryType(string &str);
};
#endif
