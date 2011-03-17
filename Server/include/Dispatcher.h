#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <new>
#include <iostream>
#include <ace/Time_Value.h>
#include "Session.h"

class Dispatcher {
public:
	int test();
	//Instance created statically
	inline static Dispatcher*	getInstance(Session& session) {
		m_session = &session;
		return m_instance;
	}
protected:
	Dispatcher(); //protected constructor
	~Dispatcher(); //protected destructor
private:
	static Dispatcher* m_instance;
	static Session*	   m_session;
	static int		   m_return;
};

#endif //DISPATCHER_H