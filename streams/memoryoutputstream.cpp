#include "memoryoutputstream.hpp"

#include <cmath>
#include <cstring>

MemoryOutputStream::MemoryOutputStream()
{
    reallocate(32);
}

MemoryOutputStream::~MemoryOutputStream()
{
    if(m_data != nullptr)
    {
        delete[] m_data;
        m_data = nullptr;
    }
}

unsigned MemoryOutputStream::length() const
{
    return m_length;
}

const char *MemoryOutputStream::data() const
{
    return m_data;
}

void MemoryOutputStream::reset()
{
    m_length = 0;
}

void MemoryOutputStream::write(int data)
{
    write(&data, sizeof(int));
}

void MemoryOutputStream::write(unsigned data)
{
    write(&data, sizeof(unsigned));
}

void MemoryOutputStream::write(long data)
{
    write(&data, sizeof(long));
}

void MemoryOutputStream::write(bool data)
{
    write(&data, sizeof(bool));
}

void MemoryOutputStream::write(unsigned long data)
{
    write(&data, sizeof(unsigned long));
}

void MemoryOutputStream::write(const void *data, unsigned length)
{
    auto newLength = m_length + length;
    if(newLength > m_capacity) 
    {
        reallocate(std::max(newLength, m_capacity * 2));
    }
    std::memcpy(m_data + m_length, data, length);
    m_length = newLength;
}

void MemoryOutputStream::reallocate(unsigned length)
{
    m_capacity = length;
    auto buffer = new char[length];
    if(m_data != nullptr)
    {
        std::memcpy(buffer, m_data, m_length);
    }
    m_data = buffer;
}
