#ifndef PACKET_H
#define PACKET_H

#include <iostream> //TODO: temp printHex()
#include <string>
#include <stdint.h>

//TODO: We should inline some of theses functions.
//A assembly 'CALL' for theses small functions seems a little too much.
//I'm pretty sure Visual C++ will optimise this class but I'm not sure about all compilers.
//Hence in programming, coders are the rulers, I will make sure this class is inlined everytime :)
//I'm just too lazy to do it right now, ahah :]
class Packet {
public:
    //Constructors/destructors
    Packet(); //default constructor
    Packet(const Packet &obj); //copy constructor
    //std::string wrapper
    uint32_t    size();
    void        clear();
    void        erase(int pos, int nb);
	void	    append(uint8_t *str, int size);
    //Creating packets
    void	    addBool(bool cst);
    void	    addString(std::string &str);
    void		addString(const char  *str);
    //Reading packets
	bool	    eof();
    bool	    readBool();
    std::string readString();
	//Templates
	template <typename T>
    T read(){
        if((m_seek + sizeof(T)) > m_buffer.size()) return T(); //This is the same as return 0; but it will also work for when T is a class
        T val = *reinterpret_cast<T*>((uint8_t*)m_buffer.c_str()+m_seek);
        m_seek += sizeof(T);
        return val;
    }
    template <typename T>
    void add(T hex) {
        m_buffer.append(reinterpret_cast<char*>(&hex), sizeof(T));
    }
	//Processing packets
	std::string genPacket();
	//Advanced commands
    void        setSeek(uint32_t newSeek); //[Harmful]
    void        backSeek(uint32_t modSeek); //[Harmful]
    //Debugging
    void        printHex();
	void        printHexFromSeek();
private:
    std::string m_buffer;
    uint32_t    m_seek;
};

#endif //PACKET_H