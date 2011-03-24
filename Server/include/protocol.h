#ifndef PROTOCOL_H
#define PROTOCOL_H

//Protocol types
typedef uint32_t     SIZE_PCKT;
typedef uint32_t     SIZE_STRING;
typedef uint8_t      CMD;

//Protocol opcodes
#define PCKT_WELCOME 0x01
#define PCKT_AUTH	 0x02

#endif //PROTOCOL_H