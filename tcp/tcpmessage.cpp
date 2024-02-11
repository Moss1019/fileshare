#include "tcpmessage.hpp"

#include <cstring>

#include "../streams/memoryinputstream.hpp"
#include "../streams/memoryoutputstream.hpp"

TcpMessage::TcpMessage() 
{

}

TcpMessage::TcpMessage(TcpMessageType type, const char *data, unsigned length)
    :m_type{type}, m_length{length}
{
    if(length > 0)
    {
        m_data = new char[length];
        std::memcpy(m_data, data, length);
    }
}

TcpMessage::~TcpMessage()
{
    if(m_data != nullptr)
    {
        delete[] m_data;
    }
}

TcpMessageType TcpMessage::type() const
{
    return m_type;
}

unsigned TcpMessage::length() const
{
    return m_length;
}

const char *TcpMessage::data() const
{
    return m_data;
}

void TcpMessage::writeTo(MemoryOutputStream &stream) const
{
    stream.write(m_type);
    stream.write(m_length);
    if(m_length > 0)
    {
        stream.write(m_data, m_length);
    }
}   

void TcpMessage::readFrom(MemoryInputStream &stream)
{
    unsigned underlaying = 0;
    stream.read(&underlaying);
    m_type = static_cast<TcpMessageType>(underlaying);
    stream.read(&m_length);
    if(m_length > 0)
    {
        m_data = new char[m_length];
        stream.read(m_data, m_length);
    }
}