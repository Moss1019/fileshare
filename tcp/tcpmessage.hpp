#pragma once

class MemoryInputStream;
class MemoryOutputStream;

enum TcpMessageType: unsigned
{
    HeartBeat = 0,
    FileList,
    FileContent,
    FileListRequest
};

class TcpMessage
{
private:
    TcpMessageType m_type;

    unsigned m_length;

    char *m_data = nullptr;
public:
    TcpMessage(const TcpMessage &other) = delete;

    TcpMessage operator=(const TcpMessage &other) = delete;

    TcpMessage();

    TcpMessage(TcpMessageType type, const char *data, unsigned length);

    ~TcpMessage();

    TcpMessageType type() const;

    unsigned length() const;

    const char *data() const;

    void writeTo(MemoryOutputStream &stream) const;

    void readFrom(MemoryInputStream &stream);
};
