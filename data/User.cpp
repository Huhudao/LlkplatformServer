#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include "User.h"
#include "GameTable.h"
#include "../tools/helps.h"

Mutex User::mutexUsers;
std::set<User*> User::users;
                                            //id stringNeed
const string snupP = "Signup\n";       //1  2
const string sninP = "Signin\n";       //2  2
const string sninyP = "SigninYes\n";   
const string sninnP = "SigninNo\n";
const string sitP = "Sit\n";           //3  3
const string standP = "Stand\n";       //4  0
const string readyP = "Ready\n";       //5  0
const string unreadyP = "Unready\n";   //6  0
const string chatP = "Chat\n";         //7  1
const string linkP = "Link\n";         //8  4
const string logoutP = "Logout\n";     //9  0
const string endP = "End\n";           //10 0
const string yesP = "Yes\n";
const string noP = "No\n";
const int sec = 5;
const int usec = 0;
const int mxBuf = 128;
const clock_t mxOff = 8000;

void User::push(User *u){
	MutexLockGuard lock(mutexUsers);
	users.insert(u);
}

void User::erase(User *u){
	MutexLockGuard lock(mutexUsers);
	users.erase(u);
}

std::string User::allUsers(){
	MutexLockGuard lock(mutexUsers);
	string all = string(itoa(user.size())) + "\n";
	for(set<User*>::iterator it = users.begin(); it != users.end(); it++){
		all += (*it)->getName() + "\n" + (*it)->gameNums();
	}
	return all;
}

void User::init(){
	MutexLockGuard lock(mutexRdSc);
	ready = false;
	score = 0;
}
bool User::isReady(){
	MutexLockGuard lock(mutexRdSc);
	return ready;
}

string User::getName(){
	return account.getName();
}

void User::plusScore(){
	MutexLockGuard lock(mutexRdSc);
	score++;
}

int User::getScore(){
	MutexLockGuard lock(mutexRdSc);
	return score;
}

string User::gameNums(){
	MutexLockGuard lock(mutexAcco);
	return string(itoa(account.getAll()) + "\n" + itoa(account.getWin()) + "\n");
}

void User::win(){
	init();
	MutexLockGuard lock(mutexAcco);
	account.plusAll();
	account.pluswin();
}

void User::lose(){
	init();
	MutexLockGuard lock(mutexAcco);
	account.plusAll();
}

void User::sendMsg(string val){
	MutexLockGuard lock(mutexSend);
	sock->sendBuf(val.c_str(), val.length());
}

bool User::signUp(string &name, string &pwd){
	if(name.length() > mxLen || pwd.length() > mxLen){
		sendMsg(snupP + noP);
		return false;
	}
	char query[100];
	sprintf(query, "select id from userinfo where name = \'%s\'", name.c_str());
	int ret = mysql_query(mysql, query);
	assert(ret == 0);
	MYSQL_RES *res = mysql_store_result(mysql);
	assert(res);
	if(mysql_num_rows(res) > 0){
		sendMsg(snupP + noP);
		return false;
	}
	sprintf(query, "insert userinfo set name = \'%s\', pwd = \'%s\',
			rgtime = now(), lastonline = now()", name.c_str(), pwd.c_str());
	ret = mysql_query(mysql, query);
	assert(ret == 0);
	sendMsg(snupP + yesP);
	return true;
}

bool User::signIn(string &name, string &pwd){
	char query[100];
	sprintf(query, "select id, gameall, gamewin from userinfo
			where name = \'%s\' and pwd = \'%s\'", name.c_str(), pwd.c_str());
	int ret = mysql_query(mysql, query);
	assert(ret == 0);
	MYSQL_RES *res = mysql_store_result(mysql);
	assert(res);
	if(mysql_num_rows(res) == 0){
		sendMsg(sninnP);
		return false;
	}
	MYSQL_ROW row = mysql_fetch_row(res);
	assert(mysql_num_fields(res) == 3);
	assert(atoi(row[0]) > 0);
	account.logIn(atoi(row[0]), atoi(row[1]), atoi(row[2]), name.c_str());
	hassignIn = true;
	push(this);
	sendMsg(sninyP + allUsers() + GameTable::allTables());
	broadCast(sninP + account.getName() + "\n" + row[1] + "\n" + row[2] + "\n");
	return true;
}

bool User::sit(string &stableId, string &sp, string &sn){
	int tableId = stoi(stableId), p = stoi(sp), n = stoi(sn);
	User::TablePtr theTable;
	if(tableId == 0){
		theTable = GameTable::createTable(p, n);
	}
	else{
		theTable = GameTable::getTable(tableId);
	}
	if(theTable){
		if(theTable->attach(this)){
			table = theTable;
			init();
			broadCast(sitP + account.getName() + "\n" + stableId + "\n"
					+ sp + "\n");
			return true;
		}
		else{
			//TODO log info
			return false;
		}
	}
	else{
		//TODO log error
		return false;
	}
}

void User::stand(){
	init();
	if(table){
		table->detach(this);
		table.reset();
		broadcast(standP + account.getName() + "\n");
	}
}

void User::chat(string &val){
	table->chat(this, val.c_str());
}

void User::link(string &sx1, string &sy1, string &sx2, string &sy2){
	table->link(this, stoi(sx1), stoi(sy1), stoi(sx2), stoi(sy2));
}

void User::beReady(){
	MutexLockGuard lock(mutexRdSc);
	ready = true;
	table->getReady(this);
}

void User::unReady(){
	MutexLockGuard lock(mutexRdSc);
	ready = false;
	table->unReady(this);
}

void User::logOut(){
	if(!hasSignIn){
		return;
	}
	hasSignIn = false;
	account.logOut(mysql);
	stand();
	User::erase(this);
	broadCast(logoutP + account.getName() + "\n");
}

void User::closeSock(){
	close(sock);
}

bool User::checkSigned(){
	if(!hasSignIn){
		//TODO log error
		return false;
	}
	return true;
}

bool User::checkSat(){
	if(!checkSigned()){
		return false;
	}
	if(!table){
		//TODO log error
		return false;
	}
	return true;
}

void User::broadCast(std::string val){
	MutexLockGuard lock(mutexUsers);
	for(set<User*>::iterator it = users.begin(); it != users.end(); it++){
		(*it)->sendMsg(val);
	}
}

void User::destroy(){
	delete this;
}

void User::solve(int id, string *strs, bool &connecting){
	switch(id){
		case 1: signUp(strs[0], strs[1]); break;
		case 2: signIn(strs[0], strs[1]); break;
		case 3: sit(strs[0], strs[1]); break;
		case 4: stand(); break;
		case 5: beReady(); break;
		case 6: unReady(); break;
		case 7: chat(strs[0]); break;
		case 8: link(strs[0], strs[1], strs[2], strs[3]); break;
		case 9: connecting = false; break;
		case 10: end(); break;
	}
}

pair<int, int> User::qryType(string &str){
	if(str == snupP) return make_pair(1, 2);
	else if(str == sninP) return make_pair(2, 2);
	else if(str == sitP) return make_pair(3, 3);
	else if(str == standP) return make_pair(4, 0);
	else if(str == readyP) return make_pair(5, 0);
	else if(str == unreadyP) return make_pair(6, 0);
	else if(str == chatP) return make_pair(7, 1);
	else if(str == linkP) return make_pair(8, 4);
	else if(str == logoutP) return make_pair(9, 0);
	else if(str == endP) return make_pair(10, 0);
	else{
		//TODO log error
	}
}

void User::threadFunc(){
	bool conneting = true;
	clock_t timeNow;
	struct timeval tv;
	tv.tv_sec = sec;
	tv.tv_usec = usec;
	sock->setRecvTimeo(tv);
	char buf[mxBuf];
	int begin = 0, end = 0, num = 0, recvNum = 0, nend = 0;
	string cmd;
	string args[4];
	pair<int, int> qry(0, 0);
	while(connecting){
		if(num == mxBuf){
			//TODO log error
			connecting = false;
			break;
		}
		if(num == 0 || begin < end){
			recvNum = sock->recvBuf(buf + end, mxBuf - end);
		}
		else{
			recvNum = sock->recvBuf(buf + end, begin - end);
		}
		timeNow = clock();
		if(recvNum <= 0 || (lastTime > 0 && recvNum - lastTime > mxOff)){
			//TODO log info
			connecting = false;
			break;
		}
		lastTime = timeNow;
		nend = end + recvNum;
		num += recvNum;
		mod(nend, mxBuf);
		for(int i = end; i != nend && connecting;){
			if(buf[i] == '\n'){
				if(begin <= i){
					cmd = string(buf+ begin, i - begin + 1);
				}
				else{
					cmd = string(buf + begin, mxBuf - begin) + string(buf, i + 1);
				}
				begin = i + 1;
				num -= cmd.length();
				mod(begin, mxbuf);
				if(qry.first == 0) qry = qrytype(cmd);
				else args[argsNum++] = cmd.strlen(0, cmd.length() - 1);
				if(argsNum == qry.second){
					solve(qry.first, args, connecting);
					qry = make_pair(0, 0);
					argsNum = 0;
				}
				else{
					//TODO log error
				}
			}
			i++;
			mod(i, mxbuf);
		}
	}
	logOut();
	closeSock();
	destroy();
}
