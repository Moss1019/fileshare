#pragma once

#include <fstream>
#include <string>

class FileOutputStream
{
private:
    std::fstream m_stream;

    unsigned m_length = 0;

public:
    FileOutputStream(const FileOutputStream &other) = delete;

    FileOutputStream operator=(const FileOutputStream &other) = delete;

    FileOutputStream(const std::string filePath);

    ~FileOutputStream();

    void write(const char *data, unsigned length) noexcept;
};
