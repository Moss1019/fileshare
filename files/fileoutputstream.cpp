#include "fileoutputstream.hpp"

FileOutputStream::FileOutputStream(const std::string filePath)
{
    m_stream.open(filePath, std::ios::out | std::ios::binary);
}

FileOutputStream::~FileOutputStream()
{
    m_stream.close();
}

void FileOutputStream::write(const char *data, unsigned length) noexcept
{
    m_stream.write(data, length);
    m_length += length;
}