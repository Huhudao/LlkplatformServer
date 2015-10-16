#include <time.h>
#include <sys/time.h>

#include "ClientSocket.h"

int& ClientSocket::getSockfd(){
	return sockfd;
}

void ClientSocket::setSockfd(int fd){
	sockfd = fd;
}

struct sockaddr_in& ClientSocket::getAddrin(){
	return addr.getAddrin();
}

void ClientSocket::setRecvTimeo(struct timeval &tv){
	int ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval));
	assert(ret == 0);
}

int ClientSocket::recvBuf(void *buf, size_t mxSize){
	char *buff = buf;
	int ret = recv(sockfd, buff, mxSize, 0);
	return ret;
}

int ClientSocket::sendBuf(void *buf, size_t sz){
	int ret = send(sockfd, buf, sz, 0);
	return ret;
}
