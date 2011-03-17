#include "Acceptor.h"

Acceptor::Acceptor(ACE_Reactor* reactor) :
        ACE_Event_Handler(),
        m_reactor(reactor == 0 ? ACE_Reactor::instance() : reactor),
        m_acceptor() {
}

Acceptor::~Acceptor() {
}

int Acceptor::open() {
	//Create the local address used for the service (NGINE_PORT is from config.h)
    ACE_INET_Addr addr(NGINE_PORT);
    //Open a port using the acceptor; reuse the address later
    if (m_acceptor.open(addr, 1) == -1) {
		std::cerr << "[Acceptor] Failed to open listening socket." << std::endl;
		return -1;
	}
    //Register the handler with the reactor
    if (m_reactor->register_handler(this, ACE_Event_Handler::ACCEPT_MASK) == -1) {
		std::cerr << "[Acceptor] Failed to register Acceptor" << std::endl;
		//Don't leave the acceptor open
        if (m_acceptor.close() == -1)
			std::cerr << "[Acceptor] Failed to close the socket after previous error." << std::endl;
		return -1;
    }

    return 0;
}

ACE_HANDLE Acceptor::get_handle() const {
	return m_acceptor.get_handle();
}

int Acceptor::handle_input(ACE_HANDLE) {
	ACE_INET_Addr clientAddr;
	//Create a new Client
    Client *client = new (ACE_nothrow) Client();
    if (client == NULL) {
		std::cerr << "[Acceptor] Failed to allocate session." << std::endl;
		return -1;
	}
    //Put the session in an auto pointer so we can return safely
    auto_ptr<Client> pR(client);
	//Accept the connection using the session's stream (read handler)
    if (m_acceptor.accept(client->getStream(), &clientAddr) == -1) {
		std::cerr << "[Acceptor] Failed to accept client connection." << std::endl;
		return -1;
	}
    //Register the session(read handler) with the reactor
    if (m_reactor->register_handler(client, ACE_Event_Handler::READ_MASK) == -1) {
		std::cerr << "[Acceptor] Failed to register Acceptor." << std::endl;
		return -1;
	}
    //From now on, the Session takes care of itself
    pR.release();
    return 0; // keep going
}

int Acceptor::handle_close(ACE_HANDLE, ACE_Reactor_Mask) {
	std::cout << "Acceptor::handle_close()\n";
	//Close the listening socket
    if (m_acceptor.close() == -1)
		std::cerr << "[Acceptor] Failed to close the socket." << std::endl;
	// no need to distinguish between error during close and normal close
    // since ACE does not evaluate the return value of handle_close()
    delete this;
    return 0;
}
