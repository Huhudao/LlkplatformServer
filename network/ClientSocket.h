#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <sys/time.h>

#include "Address.h"
#include "Socket.h"
#include "../tools/Mutex.h"

class ClientSocket: protected Socket{
public:
	ClientSocket(){}
	~ClientSocket(){}

	int& getSockfd();
	void setSockfd(int fd);
	struct sockaddr_in* getAddrin();
	void setRecvTimeo(struct timeval &tv);
	int recvBuf(char *buf, size_t mxSize);
	int sendBuf(const char *buf, size_t sz);
	void closeSock();
};
#endif
