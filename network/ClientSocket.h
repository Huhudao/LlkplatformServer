#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include "Address.h"
#include "Socket.h"

class ClientSocket: protected Socket{
public:
	ClientSocket(){}
	
	int& getSockfd();
	void setSockfd(int fd);
	struct sockaddr_in& getAddrin();
	void setRecvTimeo(struct timeval &tv);
	int recvBuf(void *buf, size_t mxSize);
	int sendBuf(void *buf, size_t sz);
};

