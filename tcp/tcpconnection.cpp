#include "tcpconnection.hpp"

#include <iostream>

#include "tcpsocket.hpp"
#include "tcpmessage.hpp"
#include "../streams/memoryinputstream.hpp"
#include "../streams/memoryoutputstream.hpp"

TcpConnection::TcpConnection(socketType sock, receiveCallback callback)
    :m_callback{callback}
{
    m_sock = new TcpSocket(sock, false);
}

TcpConnection::~TcpConnection()
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
}

void TcpConnection::start()
{
    m_isRunning = true;
    m_worker = new std::thread(&TcpConnection::doWork, this);
}

void TcpConnection::stop()
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

bool TcpConnection::inError() const
{
    return m_inError;
}

bool TcpConnection::isRunning() const
{
    return m_isRunning;
}

const std::string &TcpConnection::error() const
{
    return m_error;
}

void TcpConnection::doWork()
{
    while(m_isRunning)
    {
        TcpMessage msg;
        auto bytesRead = m_sock->receiveMessage(msg);
        if(m_sock->inError())
        {
            m_inError = true;
            m_error = m_sock->error();
            m_isRunning = false;
        }
        else if(bytesRead > 0)
        {
            m_callback(msg, m_sock);
        }
    }
}
