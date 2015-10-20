#include "Server.h"

Log logger;

int main(){
	logger.start();
	logger.logInfo("Server started\n");
	ThreadPool threadPool;
	threadPool.start();
	DBConnPool dbconnPool;
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
