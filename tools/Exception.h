#ifndef EXCEPTION_H
#define EXCEPTION_H

class Exception{
public:
	Exception(){}
};

class ThreadException: public Exception{
public:
	ThreadException(){}
};

class SocketException: public Exception{
public:
	SocketException(){}
};

class SocketCreateException: public SocketException{
public:
	SocketCreateException(){}
};

class SocketBindExceptiob: public SocketException{
public:
	SocketBindException(){}
};

class SocketAcceptException: public SocketException{
public:
	socketAcceptException(){}
};

class IOException: public Exception{
public:
	IOException(){}
};
#endif
