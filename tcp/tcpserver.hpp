#pragma once

#include <string>
#include <thread>
#include <vector>

#include "types.hpp"

class TcpSocket;

class TcpConnection;

class TcpServer
{
private:
    bool m_isRunning = false;

    TcpSocket *m_sock = nullptr;

    std::thread *m_worker = nullptr;

    std::vector<TcpConnection *> m_clients;

    receiveCallback m_callback;

    void doWork();
public:
    TcpServer(const TcpServer &other) = delete;

    TcpServer operator=(const TcpServer &other) = delete;

    TcpServer(receiveCallback m_callback);

    ~TcpServer();

    bool inError() const;

    const std::string &error() const;

    void start();

    void stop();
};