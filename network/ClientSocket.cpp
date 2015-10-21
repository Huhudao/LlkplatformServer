#include <time.h>
#include <stdio.h>
#include <sys/time.h>

#include "ClientSocket.h"

int& ClientSocket::getSockfd(){
	return sockfd;
}

void ClientSocket::setSockfd(int fd){
	sockfd = fd;
}

struct sockaddr_in* ClientSocket::getAddrin(){
	return addr.getAddrin();
}

void ClientSocket::setRecvTimeo(struct timeval &tv){
	int ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval));
	assert(ret == 0);
}

int ClientSocket::recvBuf(char *buf, size_t mxSize){
	int ret = recv(sockfd, buf, mxSize, 0);
	return ret;
}

int ClientSocket::sendBuf(const char *buf, size_t sz){
	int ret = send(sockfd, buf, sz, 0);
	return ret;
}

void ClientSocket::closeSock(){
	close(sockfd);
}
