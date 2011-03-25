#include "Packet.h"

Packet::Packet() {
    m_seek = 0;
    m_buffer = "";
}

Packet::Packet(const Packet &obj) {
    m_buffer = obj.m_buffer;
    m_seek = obj.m_seek;
}

void Packet::send(ACE_SOCK_Stream& sock) {
	if (sock.send_n(genPacket().c_str(), m_buffer.size()+sizeof(SIZE_PCKT)) == -1) { //+ the `size` header's size
		std::cerr << "[Packet] Failed to send packet." << std::endl;
	}
}

void Packet::erase(int pos, int nb) {
    m_buffer.erase(pos, nb);
}

bool Packet::eof() {
    return (m_seek == size());
}

void Packet::addString(const char *str) {
    add<SIZE_STRING>(strlen(str));
    m_buffer.append(str);
}

void Packet::addString(std::string &str) {
    add<SIZE_STRING>(str.size());
    m_buffer.append(str);
}

void Packet::clear() {
    m_buffer.clear();
    m_seek = 0;
}

bool Packet::readBool() {
    return (read<uint8_t>() == 1);
}

void Packet::addBool(bool cst) {
    if (cst) {
        add<uint8_t>(1); //true=1
    } else {
        add<uint8_t>(0); //false=0
    }
}

void Packet::append(uint8_t *str, int size) {
	m_buffer.append(reinterpret_cast<char*>(str), size);
}

SIZE_PCKT Packet::size() {
	return static_cast<SIZE_PCKT>(m_buffer.size());
}

std::string Packet::genPacket() {
    std::string output;
	SIZE_PCKT size = m_buffer.size();
	output.append(reinterpret_cast<char*>(&size), sizeof(SIZE_PCKT));
	output.append(m_buffer);
    return output;
}

std::string Packet::readString() {
    std::string output;
    uint32_t length = read<uint32_t>();
    output.append(m_buffer.substr(m_seek, length));
    m_seek += length;
    return output;
}

void Packet::setSeek(uint32_t newSeek) {
    m_seek = newSeek;
}

void Packet::backSeek(uint32_t modSeek) {
    m_seek -= modSeek;
}

void Packet::printHex() {
    SIZE_PCKT size = m_buffer.size();
    for (uint32_t i=0; i<size; i++) {
        if (i == size-1) {
            printf("%02X", (uint8_t) m_buffer.at(i));
        } else {
            printf("%02X ", (uint8_t) m_buffer.at(i));
        }
    }
}

void Packet::printHexFromSeek() {
    SIZE_PCKT size = m_buffer.size();
    for (uint32_t i=m_seek; i<size; i++) {
        if (i == size-1) {
            printf("%02X", (uint8_t) m_buffer.at(i));
        } else {
            printf("%02X ", (uint8_t) m_buffer.at(i));
        }
    }
}