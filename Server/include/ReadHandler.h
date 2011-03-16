#ifndef READ_HANDLER_H
#define READ_HANDLER_H

#include "config.h"
#include <iostream>
#include <ace/Event_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/streams.h>
#include <ace/Time_Value.h>
#include <ace/Log_Msg.h>

//This read handler is created by the accept handler and handles all the data
//exchange between client and server. The client makes two requests to the
//server. The first asks the server to create a buffer which will hold the
//data sent in the second call.
class ReadHandler : public ACE_Event_Handler {
    private:
		//The stream socket used for data exchange.
        ACE_SOCK_Stream mStream;
		//The size of the data array.
        int mDataSize;
		//The array containing the client's data.
        char *mData;

    public:
		//Initialization.
        ReadHandler(void);
		//Clean up data.
        virtual ~ReadHandler();
		//Provide access to the internal stream socket.
        ACE_SOCK_Stream &getStream(void);

        //--- Overridden methods from the ACE_Event_Handler BELOW ---
        
        //Provides the handle of mStream;
        virtual ACE_HANDLE get_handle(void) const;
		//Handles the data excahnge between client and server.
		virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE);
        //Deletes this instance of the read handler.
        virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask);
};

#endif //READ_HANDLER_H

