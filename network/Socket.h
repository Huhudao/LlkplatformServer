#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <assert.h>
#include <unistd.h>
#include <boost/noncopyable.hpp>

#include "Address.h"

class Socket: public boost::noncopyable{
protected:
	int sockfd;
	Address addr;
public:
	Socket(){}
	~Socket(){}


	Socket(int fd, Address theAddr){
		sockfd = fd;
		addr = theAddr;
	}

	Socket(int fd, sa_family_t family, in_port_t port, const char *ip):
		sockfd(fd),
		addr(family, port, ip){}

	Socket(int fd, sa_family_t family, in_port_t port, in_addr_t ip):
		sockfd(fd),
		addr(family, port, ip){}
};
#endif
