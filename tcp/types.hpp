#pragma once

#ifdef __linux__
#include <sys/socket.h>
typedef int socketType;
#endif

class TcpSocket;
class TcpMessage;

typedef void (*receiveCallback)(TcpMessage &msg, TcpSocket *remoteConnection);
