#ifndef ACCEPT_HANDLER_H
#define ACCEPT_HANDLER_H

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
#include "ReadHandler.h"

//This accept handler is based on the provided solution from the ACE course.
class AcceptHandler : public ACE_Event_Handler {
	private:
		//The reactor to which the accept handler belongs.
        ACE_Reactor *mReactor;
		//The socket used for incoming conections.
        ACE_SOCK_Acceptor mAcceptor;
    public:
		//The reactor which will use this accept handler.
        AcceptHandler(ACE_Reactor *reactor = 0);
		//The destructor exists for tracing purposes.
        virtual ~AcceptHandler();
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

#endif //ACCEPT_HANDLER_H
