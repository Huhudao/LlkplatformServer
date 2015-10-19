#include "Server.h"

int main(){
	ThreadPool threadPool;
	threadPool.start();
	DBConnPool dbconnPool;
	dbconnPool.start();
	Address serverAddress(AF_INET, 1111, INADDR_ANY);
	ServerSocket server(serverAddress);
	server.start();
	while(true){
		ServerSocket::ClientPtr client = server.acceptClient();
		User *user = new User(client);
		threadPool.runTask(boost::bind(&User::threadFunc, user));
	}
}
