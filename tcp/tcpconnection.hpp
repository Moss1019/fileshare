#pragma once

#include <thread>
#include <string>

#include "types.hpp"

class TcpSocket;

class TcpConnection 
{
private:
    TcpSocket *m_sock = nullptr;

    std::thread *m_worker = nullptr;

    bool m_isRunning = false;

    bool m_inError = false;

    std::string m_error;

    receiveCallback m_callback;

    void doWork();

public:
    TcpConnection(const TcpConnection &other) = delete;

    TcpConnection operator=(const TcpConnection &other) = delete;

    TcpConnection(socketType sock, receiveCallback callback);

    ~TcpConnection();

    void start();

    void stop();

    bool inError() const;

    bool isRunning() const;

    const std::string &error() const;
};
