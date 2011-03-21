#include "Client.h"

uint8_t Client::m_buffer[NGINE_SOCKET_BUFFER]; //Static

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
    int received; //no need to initialize, will be set below
	while ((received = m_session.socket.recv(&m_buffer, NGINE_SOCKET_BUFFER)) > 0) {
		m_session.buffer.append(m_buffer, received);
		std::cout << "Input: ";
		m_session.buffer.printHex();
		std::cout << "\n";
	}
	return (!received ? -1 : 0); //when !received, peer disconnected, return -1.
	/*
	unsigned char response = 'K';
	if (m_session->socket.send_n(&response, sizeof(response), &connTimeout) != 1) {
		std::cerr << "[Dispatcher] Failed to send test byte." << std::endl;
	}
	return 0;
	*/
}

int Client::handle_close(ACE_HANDLE, ACE_Reactor_Mask) {
    delete this;
    return 0;
}
