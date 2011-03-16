#include "AcceptHandler.h"

AcceptHandler::AcceptHandler(ACE_Reactor *reactor) :
        ACE_Event_Handler(),
        mReactor(reactor == 0 ? ACE_Reactor::instance() : reactor),
        mAcceptor() {
}

AcceptHandler::~AcceptHandler() {
}

int AcceptHandler::open() {
	//Create the local address used for the service (NGINE_PORT is from config.h)
    ACE_INET_Addr addr(NGINE_PORT);
    //Open a port using the acceptor; reuse the address later
    if (mAcceptor.open(addr, 1) == -1) {
		std::cerr << "[AcceptHandler] Failed to open listening socket." << std::endl;
		return -1;
	}
    //Register the handler with the reactor
    if (mReactor->register_handler(this, ACE_Event_Handler::ACCEPT_MASK) == -1) {
		std::cerr << "[AcceptHandler] Failed to register AcceptHandler" << std::endl;
		//Don't leave the acceptor open
        if (mAcceptor.close() == -1)
			std::cerr << "[AcceptHandler] Failed to close the socket after previous error." << std::endl;
		return -1;
    }

    return 0;
}

ACE_HANDLE AcceptHandler::get_handle() const {
	return mAcceptor.get_handle();
}

int AcceptHandler::handle_input(ACE_HANDLE) {
	ACE_INET_Addr clientAddr;
	//Create a new ReadHandler
    ReadHandler *r = new (ACE_nothrow) ReadHandler();
    if (r == 0) {
		std::cerr << "[AcceptHandler] Failed to allocate ReadHandler." << std::endl;
		return -1;
	}
    //Put reader in an auto pointer so we can return safely
    auto_ptr<ReadHandler> pR(r);
	//Accept the connection using the reader's stream
    if (mAcceptor.accept(r->getStream(), &clientAddr) == -1) {
		std::cerr << "[AcceptHandler] Failed to accept client connection." << std::endl;
		return -1;
	}
    //Register the ReadHandler with the reactor
    if (mReactor->register_handler(r, ACE_Event_Handler::READ_MASK) == -1) {
		std::cerr << "[AcceptHandler] Failed to register ReadHandler." << std::endl;
		return -1;
	}
    //From now on, the ReadHandler takes care of itself
    pR.release();
    return 0; // keep going
}

int AcceptHandler::handle_close(ACE_HANDLE, ACE_Reactor_Mask) {
	std::cout << "AcceptHandler::handle_close()\n";
	//Close the listening socket
    if (mAcceptor.close() == -1)
		std::cerr << "[AcceptHandler] Failed to close the socket." << std::endl;
	// no need to distinguish between error during close and normal close
    // since ACE does not evaluate the return value of handle_close()
    delete this;
    return 0;
}
