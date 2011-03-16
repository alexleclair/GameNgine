#ifndef MAIN_H
#define MAIN_H

//Configuration (always first)
#include "config.h"
//ACE library
#include <ace/Event_Handler.h>
#include <ace/Log_Msg.h>
#include <ace/Reactor.h>
#include <ace/Signal.h>
#include <ace/streams.h>
#include <ace/Thread_Manager.h>
#include <ace/Auto_Ptr.h>
//Dependencies
#include "AcceptHandler.h"
#include <iostream>

//Functions
void setupSignals();
void printGraffiti();
void handle_signal(int signal);

#endif //MAIN_H