#ifndef MAIN_H
#define MAIN_H

//Configuration (always first)
#include "config.h"
//ACE library
#include <ace/Event_Handler.h>
#include <ace/Reactor.h>
#include <ace/Signal.h>
#include <ace/streams.h>
#include <ace/Thread_Manager.h>
#include <ace/Auto_Ptr.h>
#include <ace/SOCK_Stream.h>
//Dependencies
#include "Acceptor.h"
#include <iostream>
#include "Session.h"

//Functions
void setupSignals();
void printGraffiti();
void handle_signal(int signal);

#endif //MAIN_H