#include "ReadHandler.h"

ReadHandler::ReadHandler() : ACE_Event_Handler(), mStream(), mDataSize(0), mData(0) {
	std::cout << "[ReadHandler] New client joined!\n";
}

ReadHandler::~ReadHandler() {
	std::cout << "[ReadHandler] Client disconnected!\n";
	if (mStream.close() == -1)
		std::cerr << "[ReadHandler] Failed to close the socket." << std::endl;
    delete[] mData;
}

ACE_SOCK_Stream& ReadHandler::getStream() {
	return mStream;
}

ACE_HANDLE ReadHandler::get_handle() const {
	return mStream.get_handle();
}

int ReadHandler::handle_input(ACE_HANDLE) {
    //ACE timeout value for sockets
	ACE_Time_Value connTimeout(NGINE_TIMEOUT);
	//The response sent to the client
    char response = 0;

    // get the desired size from the client
    // Note: only use the sizeof and pointer to int on compatible
    // platforms (i.e. little-endian/big-endian, data type size)
    if (mStream.recv_n(&mDataSize, sizeof(mDataSize), &connTimeout) != sizeof(mDataSize)) {
		std::cerr << "[ReadHandler] Failed to receive request." << std::endl;
		return -1;
    }

	mDataSize = sizeof(mDataSize); //loop
    //std::cout << "[ReadHandler] Data size: " << mDataSize << "\n";
    
	//Check mDataSize for plausability then allocate memory
    /*
	if (mDataSize > 0) {
		mData = new (std::nothrow) char[mDataSize];
        if (mData == 0)
			std::cerr << "[ReadHandler] Failed to allocate data buffer." << std::endl;
        else
            response = 'K';
    }
	*/
	response = 'K';

	//send the response to the client (which is still 0, if the
    //allocation did not succeed)
    if (mStream.send_n(&response, sizeof(response), &connTimeout) != 1) {
		std::cerr << "[ReadHandler] Failed to send response." << std::endl;
		return -1;
	}

    if (response == 'K')
		return 0; //get another request from the same client
    else
        return -1; //the client will not send data if response != 'K'

	//return -1; // ask for removal, since client does not send any more data
}

int ReadHandler::handle_close(ACE_HANDLE, ACE_Reactor_Mask) {
    delete this;
    return 0;
}
