#pragma once

#include "types.hpp"

#include <string>

class TcpSocket
{
private:
    socketType m_sock = -1;

    bool m_inError = false;

    sockaddr m_addr{};

    socklen_t m_addrLength = 0;

    std::string m_error;

public:
    TcpSocket(const TcpSocket &other) = delete;

    TcpSocket operator=(const TcpSocket &other) = delete;

    TcpSocket(const std::string &ipAddress, short port, bool nonBinding);

    TcpSocket(int sock, bool nonBinding);

    ~TcpSocket();

    bool inError() const;

    const std::string &error() const;

    socklen_t addrLength() const;

    const sockaddr &addr() const;

    void connectToRemote();

    void bindToLocal();

    int acceptConnection(sockaddr &addr, socklen_t &addrLength);

    void sendMessage(const TcpMessage &msg);

    unsigned receiveMessage(TcpMessage &msg);
};
