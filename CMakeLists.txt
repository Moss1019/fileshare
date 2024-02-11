cmake_minimum_required(VERSION 3.22)
project(fileshare)

set (CMAKE_CXX_STANDARD 20)

add_executable(fileshare
    main.cpp
    tcp/tcpserver.cpp
    tcp/tcpclient.cpp
    tcp/tcpsocket.cpp
    tcp/tcpmessage.cpp
    tcp/tcpconnection.cpp
    files/directoryprobe.cpp
    files/fileinputstream.cpp
    files/fileoutputstream.cpp
    streams/memoryinputstream.cpp
    streams/memoryoutputstream.cpp
)
