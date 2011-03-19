#include "Packet.h"

Packet::Packet() {
    m_seek = 0;
    m_buffer = "";
}

//! Erase uint8_ts from the uint8_t array
/**
* @param pos position yourself somewhere in the buffer
* @param nb the amount of uint8_ts to delete from that position
*/
void Packet::erase(int pos, int nb) {
    m_buffer.erase(pos, nb);
}

//! Special overloaded constructor
/** Is only created/used/returned in/by (ZSocket::)socket_read()
* @param array are the uint8_ts read from the socket
* @param size are the amount of uint8_ts read from the socket
*/
Packet::Packet(uint8_t *new_array, unsigned short size) {
    if (size > 0) { //dont perform actions if size is negative or equals to zero
        const unsigned char *temp_array;
        temp_array = reinterpret_cast<const unsigned char*>(new_array);
        m_buffer.append(reinterpret_cast<const char*>(temp_array),size);
		m_seek = 0;
    }
}

//! Copy constructor, nothing surnatural here.
Packet::Packet(const Packet &obj) {
    m_buffer = obj.m_buffer;
    m_seek = obj.m_seek;
}

//destructor
Packet::~Packet() {
}

//! Check if the seek is as the end of the uint8_t array
/**
* @return
* TRUE if we are at the EndOfFile and there's nothing else to read \n
* FALSE if we are not at the EndOfFile and there is still something to read \n
*/
bool Packet::eof() {
    if (m_seek == size()) {
        return true; //the pointer is at the end
    } else {
        return false; //the pointer isnt at the end
    }
}

//! This function append a uint32_t and a const char* to the uint8_t array
void Packet::addString(const char *str) {
    std::string buffer = str;
    add<uint32_t>((int)buffer.size());
    m_buffer.append(buffer);
}

//! This function append a uint32_t and std::string to the uint8_t array
void Packet::addString(std::string str) {
    add<uint32_t>((int)str.size());
    m_buffer.append(str);
}

//! Clear the uint8_t array entirely
void Packet::clear() {
    m_buffer.clear();
    m_seek = 0;
}

bool Packet::readBool() {
    if (read<uint8_t>() == 1) {
        return true;
    } else {
        return false;
    }
}

void Packet::addBool(bool cst) {
    if (cst) {
        add<uint8_t>(1); //true=1
    } else {
        add<uint8_t>(0); //false=0
    }
}

//! Return the size of the uint8_t array
uint32_t Packet::size() {
    return (uint32_t)m_buffer.size();
}

//! Wrapper to use the uint8_t array is it really was an array @return uint8_t
uint8_t Packet::operator[](int i) {
    return (uint8_t)m_buffer[i];
}

//! [Harmful] This function returns a reference to the uint8_t array
std::string& Packet::getRefBuffer() {
    return m_buffer;
}

//! [Harmful] This function append another uint8_tArray to the uint8_t array
void Packet::appendPacket(Packet &obj) {
    m_buffer.append(obj.getRefBuffer());
}

//! Generate a packet @return a uint8_tArray of a generated packet header + the uint8_t array buffer
std::string Packet::genStringPacket() {
    //Packet begining [uint8_t]
    std::string output = "~";

    //Packet size [uint32_t]
    uint32_t size = m_buffer.size();
    output.append(reinterpret_cast<char*>(&size), sizeof(uint32_t));

    //Packet content
    output.append(m_buffer);

    return output;
}

// Tombana: Use the templated read function!!!!
//Or use the right operators instead of the _pow stuff.
//uint32_t Packet::readuint32_t() {
//	uint32_t val=0;
//	val |= ( (uint8_t)m_buffer[m_seek+3] & 0xff ) << 24;
//	val |= ( (uint8_t)m_buffer[m_seek+2] & 0xff ) << 16;
//	val |= ( (uint8_t)m_buffer[m_seek+1] & 0xff ) << 8;
//	val |= ( (uint8_t)m_buffer[m_seek] & 0xff );
//	m_seek+=4;
//	return val;
//}

//! Read a 'string' from the uint8_t array
/** Actually it reads a \e _size uint32_t, then reads \e _size amount of chars and put them into a std:string
@return std::string
*/
std::string Packet::readString() {
    std::string output;
    unsigned int length = read<uint32_t>();
    output.append(m_buffer.substr(m_seek, length));
    m_seek+=length;
    return output;
}

//! [Harmful] Position the seek to somewhere else in the uint8_t array
void Packet::setSeek(unsigned int newSeek) {
    m_seek = newSeek;
}

//! [Harmful] Modify sightly the position of the seek in the uint8_t array
void Packet::backSeek(unsigned int modSeek) {
    m_seek -= modSeek;
}

//! [Debugging] Prints (human-readable) the uint8_t array
void Packet::printHex() {
    unsigned int size = m_buffer.size();
    for (unsigned int i=0; i<size; i++) {
        if (i == size-1) {
            printf("%02X", (uint8_t) m_buffer.at(i));
        } else {
            printf("%02X ", (uint8_t) m_buffer.at(i));
        }
    }
}

//! [Debugging] Prints (human-readable) the uint8_t array
void Packet::printHexFromSeek() {
    unsigned int size = m_buffer.size();
    for (unsigned int i=m_seek; i<size; i++) {
        if (i == size-1) {
            printf("%02X", (uint8_t) m_buffer.at(i));
        } else {
            printf("%02X ", (uint8_t) m_buffer.at(i));
        }
    }
}