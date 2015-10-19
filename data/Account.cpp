#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Account.h"

unsigned int Account::getId(){
	return id;
}

int Account::getAll(){
	return gameAll;
}

int Account::getWin(){
	return gameWin;
}

std::string Account::getName(){
	return name;
}

void Account::getData(MYSQL *mysql){
	char query[100];
	sprintf(query, "select gamewin, gameall from userinfo where id = %u", id);
	int ret = mysql_query(mysql, query);
	assert(ret == 0);
	MYSQL_RES *res = mysql_store_result(mysql);
	assert(res);
	assert(mysql_num_rows(res) > 0);
	MYSQL_ROW row = mysql_fetch_row(res);
	assert(mysql_num_fields(res) == 2);
	gameWin = atoi(row[0]);
	gameAll = atoi(row[1]);
	mysql_free_result(res);
}

void Account::plusAll(MYSQL *mysql){
	gameAll++;
}

void Account::plusWin(MYSQL *mysql){
	gameWin++;
}

void Account::logIn(unsigned int uid, int ga, int gw, const char *nm){
	id = uid;
	gameAll = ga;
	gameWin = gw;
	name = std::string(nm);
}

void Account::logOut(MYSQL *mysql){
	char query[100];
	sprintf(query, "update userinfo set gameall = %d where id = %d", gameAll, id);
	int ret = mysql_query(mysql, query);
	assert(ret == 0);
	sprintf(query, "update userinfo set gamewin = %d where id = %d", gameWin, id);
	ret = mysql_query(mysql, query);
	assert(ret == 0);
	sprintf(query, "update userinfo set lastonline = now() where id = %d", id);
	ret = mysql_query(mysql, query);
	assert(ret == 0);
}
