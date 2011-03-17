#include "Client.h"

Client::Client() : ACE_Event_Handler(), m_stream(), m_session(m_stream) {
	std::cout << "[Client] New client joined!\n";
}

Client::~Client() {
	std::cout << "[Client] Client disconnected!\n";
	if (m_stream.close() == -1)
		std::cerr << "[Client] Failed to close the socket." << std::endl;
}

ACE_SOCK_Stream& Client::getStream() {
	return m_stream;
}

ACE_HANDLE Client::get_handle() const {
	return m_stream.get_handle();
}

int Client::handle_input(ACE_HANDLE handle) {
    std::cout << "[Client] Data received.\n";
	return Dispatcher::getInstance(m_session)->test();
}

int Client::handle_close(ACE_HANDLE, ACE_Reactor_Mask) {
    delete this;
    return 0;
}
