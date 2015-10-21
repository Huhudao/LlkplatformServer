#include "Server.h"

DBConnPool dbconnPool;

Log logger;

int main(){
	logger.start();
	logger.logInfo("Server started.");
	printf("started\n");
	ThreadPool threadPool;
	threadPool.start();
	dbconnPool.start();
	Address serverAddress(AF_INET, 1111, INADDR_ANY);
	ServerSocket server(serverAddress);
	server.start();
	while(true){
		ServerSocket::ClientPtr client = server.acceptClient();
		User *user = new User(client, dbconnPool.take());
		threadPool.runTask(boost::bind(&User::threadFunc, user));
	}
}
