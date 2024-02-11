#include "tcpsocket.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <cstring>

#include "tcpmessage.hpp"
#include "../streams/memoryinputstream.hpp"
#include "../streams/memoryoutputstream.hpp"

#include <iostream>

TcpSocket::TcpSocket(const std::string &ipAddress, short port, bool nonBinding)
{
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &addr.sin_addr);
    std::memcpy(&m_addr, &addr, sizeof(sockaddr));
    m_addrLength = sizeof(sockaddr);
    m_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(nonBinding)
    {
        fcntl(m_sock, F_SETFL, O_NONBLOCK);
    }
}

TcpSocket::TcpSocket(int sock, bool nonBinding)
    :m_sock{sock}
{
    if(nonBinding)
    {
        fcntl(m_sock, F_SETFL, O_NONBLOCK);
    }
}

TcpSocket::~TcpSocket()
{
    shutdown(m_sock, SHUT_RDWR);
    close(m_sock);
}

bool TcpSocket::inError() const
{
    return m_inError;
}

const std::string &TcpSocket::error() const
{
    return m_error;
}

socklen_t TcpSocket::addrLength() const
{
    return m_addrLength;
}

const sockaddr &TcpSocket::addr() const
{
    return m_addr;
}

void TcpSocket::connectToRemote()
{
    auto res = connect(m_sock, &m_addr, m_addrLength);
    if(res == -1)
    {
        m_inError = true;
        m_error = "Could not connect to remote host";
    }
}

void TcpSocket::bindToLocal()
{
    auto res = bind(m_sock, &m_addr, m_addrLength);
    if(res == -1)
    {
        m_inError = true;
        m_error = "Could not bind to loca addres";
    }
    else
    {
        listen(m_sock, 2);
    }
}

int TcpSocket::acceptConnection(sockaddr &addr, socklen_t &addrLength)
{
    return accept(m_sock, &addr, &addrLength);
}

void TcpSocket::sendMessage(const TcpMessage &msg)
{
    MemoryOutputStream stream;
    msg.writeTo(stream);
    auto res = send(m_sock, stream.data(), stream.length(), 0);
}

unsigned TcpSocket::receiveMessage(TcpMessage &msg)
{
    const auto bufferSize = 1024 * 50;
    auto buffer = new char[bufferSize];
    auto isReading = true;
    MemoryOutputStream outStream;
    auto read = 0;
    auto totalRead = 0;
    while(isReading) 
    {
        read = recv(m_sock, buffer, bufferSize, 0);
        totalRead += read;
        if(read <= 0)
        {
            m_inError = true;
            m_error = "Remote connection closed";
            isReading = false;
            return totalRead;
        }
        else
        {
            if(read <= bufferSize)
            {
                isReading = false;
            }
            outStream.write(buffer, read);
        }
    }
    MemoryInputStream inStream(outStream.data(), outStream.length());
    msg.readFrom(inStream);
    delete[] buffer;
    return totalRead;
}
