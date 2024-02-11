#pragma once

#include <string>
#include <vector>
#include <filesystem>

class MemoryInputStream;
class MemoryOutputStream;

struct ProbeInfo
{
    std::string path;

    bool isFile;

    int64_t lastWrite;

    void readFrom(MemoryInputStream &stream);

    void writeTo(MemoryOutputStream &stream) const;
};

typedef std::vector<ProbeInfo> ProbeList;

class DirectoryProbe
{
private:
    std::string m_directory{};

public:
    DirectoryProbe(const DirectoryProbe &other) = delete;

    DirectoryProbe operator=(const DirectoryProbe &other) = delete;

    DirectoryProbe(const std::string &directory);

    void getContent(ProbeList &) const noexcept;
};
