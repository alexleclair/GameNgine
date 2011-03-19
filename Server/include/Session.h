#ifndef SESSION_H
#define SESSION_H

#include <ace/SOCK_Stream.h>
#include "Packet.h"

struct Session {
	//Constructor
	Session(ACE_SOCK_Stream& a_socket) : socket(a_socket) {}
	//Data
	ACE_SOCK_Stream& socket;
	Packet			 buffer;
};

#endif //SESSION_H