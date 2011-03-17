#include "Dispatcher.h"

Dispatcher* Dispatcher::m_instance = new Dispatcher(); //Allocation is checked in main() at startup
Session*    Dispatcher::m_session  = NULL; //Is changing dynamically
int			Dispatcher::m_return   = 0;

Dispatcher::Dispatcher() {
}

Dispatcher::~Dispatcher() {
}

int Dispatcher::test() {
	char buffer[1024] = {0}; //temporary
	if (m_session->socket.recv(&buffer, 1024) == 0) {
		return -1;
    }
	std::cout << "Input: " << buffer << "\n";
		
	unsigned char response = 'K';
	ACE_Time_Value connTimeout(30);
	if (m_session->socket.send_n(&response, sizeof(response), &connTimeout) != 1) {
		std::cerr << "[Dispatcher] Failed to send test byte." << std::endl;
	}
	return 0;
}