#include "tcpserver.hpp"

#ifdef __linux__
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

#include <cstring>
#include <algorithm>

#include <iostream>

#include "tcpsocket.hpp"
#include "tcpconnection.hpp"

TcpServer::TcpServer(receiveCallback m_callback)
    :m_callback{m_callback}
{
    m_sock = new TcpSocket("127.0.0.1", 8080, true);
    m_sock->bindToLocal();
}

TcpServer::~TcpServer() 
{
    stop();
    if(m_worker != nullptr) 
    {
        delete m_worker;
        m_worker = nullptr;
    }
    if(m_sock != nullptr)
    {
        delete m_sock;
        m_sock = nullptr;
    }
    for(auto client = m_clients.begin(); client != m_clients.end(); ++client)
    {
        delete (*client);
    }
}

bool TcpServer::inError() const
{
    return m_sock->inError();
}

const std::string &TcpServer::error() const
{
    return m_sock->error();
}

void TcpServer::start() 
{
    if(!m_isRunning) 
    {
        m_isRunning = true;
        m_worker = new std::thread(&TcpServer::doWork, this);
    }
}

void TcpServer::stop()
{
    m_isRunning = false;
    if(m_worker != nullptr) 
    {
        if(m_worker->joinable())
        {
            m_worker->join();
        }
    }
}

void TcpServer::doWork()
{
    while(m_isRunning) 
    {
        sockaddr remoteHost;
        socklen_t length;
        auto connection = m_sock->acceptConnection(remoteHost, length);
        if(connection > -1)
        {
            auto newConnection = new TcpConnection(connection, m_callback);
            newConnection->start();
            m_clients.push_back(newConnection);
        }
        for(auto client = m_clients.begin(); client != m_clients.end(); ++client)
        {
            if(!(*client)->isRunning()) 
            {
                delete (*client);
                m_clients.erase(client);
                --client;
            }
        }
        m_clients.erase(std::remove_if(m_clients.begin(), 
            m_clients.end(),
            [](const TcpConnection *entry) { return !entry->isRunning(); }),
            m_clients.end());
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}
