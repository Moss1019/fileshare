#include "tcpclient.hpp"

#ifdef __linux__
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

#include <cstring>

#include "tcpmessage.hpp"

#include "tcpsocket.hpp"
#include "../streams/memoryinputstream.hpp"
#include "../streams/memoryoutputstream.hpp"

TcpClient::TcpClient(const std::string &remoteAddr, short remotePort)
{
    m_sock = new TcpSocket(remoteAddr, remotePort, false);
    m_sock->connectToRemote();
}

TcpClient::~TcpClient()
{
    if(m_sock != nullptr)
    {
        delete m_sock;
        m_sock = nullptr;
    }
}

bool TcpClient::inError() const
{
    return m_inError;
}

const std::string &TcpClient::error() const
{
    return m_error;
}

void TcpClient::sendMessage(const TcpMessage &message)
{
    m_sock->sendMessage(message);
    if(m_sock->inError())
    {
        m_inError = true;
        m_error = m_sock->error();
    }
}

void TcpClient::receiveMessage(TcpMessage &message)
{
    m_sock->receiveMessage(message);
    if(m_sock->inError())
    {
        m_inError = true;
        m_error = m_sock->error();
    }
}
