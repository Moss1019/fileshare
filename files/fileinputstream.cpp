#include "fileinputstream.hpp"

FileInputStream::FileInputStream(const std::string &filePath)
{
    m_stream.open(filePath, std::ios::in | std::ios::binary);
    if(m_stream.is_open()) 
    {
        m_stream.seekg(0, std::ios::end);
        m_length = m_stream.tellg();
        m_stream.seekg(0, std::ios::beg);
    }
}

FileInputStream::~FileInputStream()
{
    m_stream.close();
}

int FileInputStream::remaining() const noexcept
{
    return m_length - m_position;
}

void FileInputStream::read(char *data, unsigned length) noexcept
{
    m_stream.read(data, length);
    m_position = m_stream.tellg();
}
