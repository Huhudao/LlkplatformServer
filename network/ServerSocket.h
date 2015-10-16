#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include "Address.h"
#include "Socket.h"
#include "ClientSocket.h"

class ServerSocket: protected Socket{
private:
	const int backlog = 128;
public:
	explicit ServerSocket(Address theAddr);
	ServerSocket(sa_family_t family, in_port_t port, const char *ip);
	ServerSocket(sa_family_t family, in_port_t port, in_addr_t ip);
	~ServerSocket();

private:
	void initSock();
	void stop();

public:
	void start();
	void acceptClient(ClientSocket &client);
};
#endif
