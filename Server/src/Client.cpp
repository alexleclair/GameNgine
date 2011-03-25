#include "Client.h"

uint8_t Client::m_buffer[NGINE_SOCKET_BUFFER]; //Static

Client::Client() : ACE_Event_Handler(), m_stream(), m_session(m_stream) {
	//The class is created but the ACE_SOCK_Stream isn't accept()'ed by the Acceptor yet
	//See handle_accept() for proprer usage.
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

int Client::handle_accept() {
	//An ACE_SOCK_Stream has been set to our client instance.
	//Let's send a welcome packet to our new client.
	std::cout << "[Client] New client joined!\n";
	Packet welcomePacket;				  //Packet buffer
	welcomePacket.add<CMD>(PCKT_WELCOME); //Add PCKT_WELCOME CMD to buffer
	welcomePacket.send(m_stream);		  //Send using m_stream
	//Return 0 means it's accepted, -1 means the client is refused
	//When refused, the acceptor will close the stream and destroy this Client instance.
	return 0;
}

int Client::handle_input(ACE_HANDLE handle) {
    int received; //no need to initialize, will be set below anyway
	while ((received = m_session.socket.recv(&m_buffer, NGINE_SOCKET_BUFFER)) > 0) {
		m_session.buffer.append(m_buffer, received);
		std::cout << "Input: ";
		m_session.buffer.printHex(); 
		std::cout << "\n";
	}
	return (!received ? -1 : 0); //when !received, peer disconnected, return -1.
}

int Client::handle_close(ACE_HANDLE, ACE_Reactor_Mask) {
    delete this;
    return 0;
}
