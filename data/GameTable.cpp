#include <stdlib.h>
#include "User.h"
#include "GameTable.h"

Mutex GameTable::mutexNum;
int GameTable::tableNum = 1;
std::map<int, GameTable::TablePtr> GameTable::tables;

GameTable::GameTable(int id, int p, int n): started(0), tableId(id), pos(p),
	playerNum(n), readyNum(0), mutex(), users(), game(){}

GameTable::TablePtr GameTable::getTable(int id){
	MutexLockGuard lock(mutexNum);
	TablePtr table;
	if(tables.find(id) == tables.end()){
		//TODO log error
	}
	else{
		table = tables[id];
	}
	return table;
}

void GameTable::put(TablePtr table){
	MutexLockGuard lock(mutexNum);
	tables[table->tableId] = table;
}

GameTable::TablePtr GameTable::createTable(int p, int n){
	TablePtr table;
	{
		MutexLockGuard lock(mutexNum);
		table.reset(new GameTable(++tableNum, p, n));
	}
	put(table);
}

std::string GameTable::allTables(){
	MutexLockGuard lock(mutexNum);
	std::string all = std::string(itoa(tables.size())) + "\n";
	for(std::map<int, TablePtr>::iterator it = tables.begin(); it != tables.end(); it++){
		all += it->second->tableInfo();
	}
	return all;
}

bool GameTable::isStarted(){
	MutexLockGuard lock(mutex);
	return started;
}

bool GameTable::attach(User *u){
	MutexLockGuard lock(mutex);
	if(users.find(u) != users.end() || users.size() >= playerNum){
		return false;
	}
	else{
		users.insert(u);
		return true;
	}
}

std::string GameTable::tableInfo(){
	MutexLockGuard lock(mutex);
	std::string res = std::string(itoa(tableId)) + "\n" + itoa(pos) + "\n" + itoa(users.size()) + "\n";
	for(std::set<User*>::iterator it = users.begin(); it != users.end(); it++){
		res += (*it)->getName() + "\n";
	}
	return res;
}

void GameTable::detach(User *u){
	MutexLockGuard lock(mutex);
	if(users.find(u) == users.end()){
		//TODO log error
		return;
	}
	if(u->isReady()){
		readyNum--;
	}
	//broadCast(("Stand\n" + u->getName() + "\n").c_str());
	users.erase(u);
	if(users.empty()){
		MutexLockGuard lockNum(mutexNum);
		eraseTable(tableId);
	}
}

void GameTable::getReady(User *u){
	MutexLockGuard lock(mutex);
	readyNum++;
	broadCast("Ready\n" + u->getName() + "\n");
	if(readyNum == playerNum){
		startGame();
	}
}

void GameTable::unReady(User *u){
	MutexLockGuard lock(mutex);
	readyNum--;
	broadCast("Unready\n", u->getName() + "\n");
}

void GameTable::chat(User *u, const char *val){
	MutexLockGuard lock(mutex);
	broadCast("Chat\n" + u->getName() + "\n" + val + "\n");
}

void GameTable::link(User *u, size_t x1, size_t y1, size_t x2, size_t y2){
	MutexLockGuard lock(mutex);
	if(!started){
		broadCast("Link\n" + u->getName() + "\n" + "No\n");
	}
	else{
		ret = game.link(x1, y1, x2, y2);
		if(ret.first){
			u->plusScore();
			broadCast("Link\n" + u->getName() + "\n" + "Yes\n");
			if(ret.second){
				endGame();
			}
			return;
		}
		else{
			broadCast("Link\n" + u->getName() + "\n" + "No\n");
		}
	}
}

void GameTable::end(User *u){
	MutexLockGuard lock(mutex);
	endGame();
}

void GameTable::eraseTable(int id){
	tables.erase(id);
}

void GameTable::startGame(){
	game.init();
	started = true;
	broadCast("Start\n" + game.picsToStr() + "\n");
}

void GameTable::endGame(){
	if(!started) return;
	started = false;
	readyNum = 0;
	User *winner = NULL;
	int n = 0;
	for(std::set<User*>::iterator it = users.begin(); it != users.end(); it++){
		if(!winner || (*it)->getScore() > winner->getScore()){
			winner = (*it);
			n = 1;
		}
		else if((*it)->getScore() == winner->getScore()){
			n++;
		}
	}
	broadCast("End\nWinner\n" + (n == 1 ? winner->getName() : "None") + "\n");
	for(std::set<User*>::iterator it = users.begin(); it != users.end(); it++){
		if(n == 1 && winner == (*it)){
			(*it)->win();
		}
		else{
			(*it)->lose();
		}
	}
}

void GameTable::broadCast(std::string val){
	for(std::set<User*>::iterator it = users.begin(); it != users.end(); it++){
		(*it)->sendMsg(val);
	}
}
