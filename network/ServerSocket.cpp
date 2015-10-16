#include "ServerSocket.h"

ServerSocket::ServerSocket(Address theAddr){
	initSock();
	addr = theAddr;
}

ServerSocket::ServerSocket(sa_family_t family, in_port_t port, const char* ip){
	initSock();
	addr = Address(family, port, ip);
}

ServerSocket::ServerSocket(sa_family_t family, in_port_t port, in_addr_t ip){
	initSock();
	addr = Address(family, port, ip);
}

ServerSocket::~ServerSocket(){
	stop();
}

void ServerSocket::initSock(){
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(sockfd != -1);
}

void ServerSocket::stop(){
	close(sockfd);
}

void ServerSocket::start(){
	int ret = bind(sockfd, (struct sockaddr*)&addr.getAddrin, sizeof(struct sockaddr));
	assert(ret != -1);
	ret = listen(sockfd, backlog);
	assert(ret != -1);
}

ClientSocket ServerSocket::acceptClient(){
	socklen_t sz = sizeof(struct sockaddr);
	ClientSocket client;
	int clientfd = accept(sockfd, (struct sockaddr*)&client.getAddrin(), &sz);
	assert(clientfd != -1);
	client.setSockfd(clientfd);
	return client;
}
