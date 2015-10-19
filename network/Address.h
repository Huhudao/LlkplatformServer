#ifndef ADDRESS_H
#define ADDRESS_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>

class Address{
private:
	struct sockaddr_in addr;
public:
	Address(sa_family_t family, in_port_t port, const char *ip){
		addr.sin_family = family;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr(ip);
		bzero(&(addr.sin_zero), 8);
	}

	Address(sa_family_t family, in_port_t port, in_addr_t ip){
		addr.sin_family = family;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = ip;
		bzero(&(addr.sin_zero), 8);
	}

	~Address(){}
	
	struct sockaddr_in* getAddrin(){
		return &addr;
	}
};
#endif
