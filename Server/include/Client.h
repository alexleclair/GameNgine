#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <ace/Event_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/streams.h>
#include <ace/Time_Value.h>
#include "Session.h"
#include "config.h"
#include "Packet.h"

//This read handler is created by the accept handler and handles all the data
//exchange between client and server.
class Client : public ACE_Event_Handler {
    private:
		//The stream socket used for data exchange.
        ACE_SOCK_Stream m_stream;
		//User's session
		Session m_session;
		//Static so we wont re-allocate it on stack every packet we get
		static uint8_t m_buffer[NGINE_SOCKET_BUFFER];

    public:
		//Initialization.
        Client(void);
		//Clean up data.
        virtual ~Client();
		//Provide access to the internal stream socket.
        ACE_SOCK_Stream &getStream(void);
		int handle_accept(); //forged member function, ACE is missing this

        //--- Overridden methods from the ACE_Event_Handler BELOW ---
        
        //Provides the handle of mStream;
        virtual ACE_HANDLE get_handle(void) const;
		//Handles the data excahnge between client and server.
		virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE);
        //Deletes this instance of the read handler.
        virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask);
};

#endif //CLIENT_H

