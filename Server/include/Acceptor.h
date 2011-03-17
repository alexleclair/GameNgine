#ifndef ACCEPTOR_H
#define ACCEPTOR_H

//Configuration
#include "config.h"
//ACE library
#include <ace/Event_Handler.h>
#include <ace/Reactor.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Auto_Ptr.h>
#include <ace/INET_Addr.h>
//Dependencies
#include <iostream>
#include "Client.h"

//This accept handler is based on the provided solution from the ACE course.
class Acceptor : public ACE_Event_Handler {
	private:
		//The reactor to which the accept handler belongs.
        ACE_Reactor *m_reactor;
		//The socket used for incoming conections.
        ACE_SOCK_Acceptor m_acceptor;
    public:
		//The reactor which will use this accept handler.
        Acceptor(ACE_Reactor *reactor = 0);
		//The destructor exists for tracing purposes.
        virtual ~Acceptor();
		//Open the listening socket and register the handler with the reactor.
        //return 0 on success, -1 on failure
        int open();
		
		//--- Overridden methods from the ACE_Event_Handler below ---
        
        //Provides the handle of mAcceptor.
        virtual ACE_HANDLE get_handle() const;
		//Create a read handler for the new connection and register that
        //handler with the reactor.
        virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE);
        //Close the listening socket.
        virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask);
};

#endif //ACCEPTOR_H
