#include "directoryprobe.hpp"

#include <chrono>

#include "../streams/memoryinputstream.hpp"
#include "../streams/memoryoutputstream.hpp"

void ProbeInfo::readFrom(MemoryInputStream &stream)
{
    std::size_t length = 0;
    stream.read(&length);
    auto buffer = new char[length];
    stream.read(buffer, length);
    path = std::string(buffer);
    delete[] buffer;
    stream.read(&isFile);
    stream.read(&lastWrite);
}

void ProbeInfo::writeTo(MemoryOutputStream &stream) const
{
    stream.write(path.length());
    stream.write(path.c_str(), path.length());
    stream.write(isFile);
    stream.write(lastWrite);
}

DirectoryProbe::DirectoryProbe(const std::string &directory)
    :m_directory{directory}
{

}

void DirectoryProbe::getContent(ProbeList &list) const noexcept
{
    for(const auto &entry: std::filesystem::directory_iterator(m_directory))
    {
        auto lastWriteTime = entry.last_write_time();
        auto lastWriteSeconds = std::chrono::time_point_cast<std::chrono::seconds>(lastWriteTime);
        list.push_back(ProbeInfo());
        list.back().path = entry.path();
        list.back().isFile = !entry.is_directory();
        list.back().lastWrite = lastWriteSeconds.time_since_epoch().count();
    }
}
