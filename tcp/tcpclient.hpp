#pragma once

#include <string>

#include "types.hpp"

class TcpSocket;

class TcpClient
{
private:
    TcpSocket *m_sock;

    bool m_inError;

    std::string m_error;

public:
    TcpClient(const TcpClient &other)  = delete;

    TcpClient operator=(const TcpClient &other) = delete;

    TcpClient(const std::string &remoteAddr, short remotePort);

    ~TcpClient();

    bool inError() const;

    const std::string &error() const;

    void sendMessage(const TcpMessage &message);

    void receiveMessage(TcpMessage &message);
};
