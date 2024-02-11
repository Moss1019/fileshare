#include "memoryinputstream.hpp"

#include <cstring>

MemoryInputStream::MemoryInputStream(const char *data, unsigned length)
    :m_length{length}
{
    m_data = new char[length];
    std::memcpy(m_data, data, length);
}

MemoryInputStream::~MemoryInputStream()
{
    if(m_data != nullptr)
    {
        delete[] m_data;
        m_data = nullptr;
    }
}

unsigned MemoryInputStream::remaining() const
{
    return m_length - m_currentPos;
}

unsigned MemoryInputStream::length() const
{
    return m_length;
}

void MemoryInputStream::read(int *data)
{
    read(data, sizeof(int));
}

void MemoryInputStream::read(unsigned *data)
{
    read(data, sizeof(unsigned));
}

void MemoryInputStream::read(long *data)
{
    read(data, sizeof(long));
}

void MemoryInputStream::read(bool *data)
{
    read(data, sizeof(bool));
}

void MemoryInputStream::read(unsigned long *data)
{
    read(data, sizeof(unsigned long));
}

void MemoryInputStream::read(void *data, unsigned length)
{
    std::memcpy(data, m_data + m_currentPos, length);
    m_currentPos += length;
}
