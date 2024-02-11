#pragma once

#include <string>
#include <fstream>

class FileInputStream
{
private:
    std::fstream m_stream;

    unsigned m_length = 0;

    unsigned m_position = 0;

public:
    FileInputStream(const FileInputStream &other) = delete;

    FileInputStream operator=(const FileInputStream &other) = delete;

    FileInputStream(const std::string &filePath);

    ~FileInputStream();

    int remaining() const noexcept;

    void read(char *data, unsigned length) noexcept;
};