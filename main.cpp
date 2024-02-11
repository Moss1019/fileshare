
#include <cstring>
#include <iostream>

#include "tcp/tcpsocket.hpp"
#include "tcp/tcpserver.hpp"
#include "tcp/tcpclient.hpp"
#include "tcp/tcpmessage.hpp"
#include "files/directoryprobe.hpp"
#include "files/fileinputstream.hpp"
#include "files/fileoutputstream.hpp"
#include "streams/memoryinputstream.hpp"
#include "streams/memoryoutputstream.hpp"

void flatten(const ProbeList &list, ProbeList &infos)
{
    for(const auto &entry: list)
    {
        if(entry.isFile) 
        {
            infos.push_back(entry);
        }
        else
        {
            DirectoryProbe subProbe(entry.path);
            ProbeList subList;
            subProbe.getContent(subList);
            flatten(subList, infos);
        }
    }
}

receiveCallback c = [](TcpMessage &msg, TcpSocket *remoteConnection) -> void
{
    switch (msg.type())
    {
    case HeartBeat: 
    {
        remoteConnection->sendMessage(msg);
        break;
    }
    case FileList:
    {
        MemoryInputStream stream(msg.data(), msg.length());
        ProbeList files;
        std::size_t number = 0;
        stream.read(&number);
        for(auto i = 0; i < number; ++i)
        {
            files.push_back(ProbeInfo{});
            files.back().readFrom(stream);
        }
        DirectoryProbe localDirectory("/home/moss/Projects/file-share/test2");
        ProbeList localFiles;
        localDirectory.getContent(localFiles);
        for(auto &remoteFile: files) 
        {
            std::cout << remoteFile.path << "\n";
        }
        TcpMessage res(TcpMessageType::HeartBeat, nullptr, 0);
        remoteConnection->sendMessage(res);
    }
    default:
    {
        remoteConnection->sendMessage(msg);
        break;
    }
    }
};

int main(int argc, char const *argv[])
{
    auto isServer = atoi(argv[1]) == 1;
    if(isServer) 
    {
        TcpServer server(c);
        server.start();
        auto isRunning = true;
        if(server.inError())
        {
            std::cout << server.error() << "\n";
            server.stop();
            isRunning = false;
        }
        std::string input;
        while(isRunning)
        {
            std::cin >> input;
            if(input == "-q")
            {
                isRunning = false;
            }
        }
        server.stop();
    }
    else
    {
        DirectoryProbe probe("/home/moss/Projects/file-share/test");
        ProbeList list;
        probe.getContent(list);
        std::vector<ProbeInfo> files;
        flatten(list, files);
        TcpClient client("127.0.0.1", 8080);

        MemoryOutputStream stream;
        stream.write(files.size());
        for(const auto &file: files)
        {
            file.writeTo(stream);
        }

        TcpMessage msg(TcpMessageType::FileList, stream.data(), stream.length());
        client.sendMessage(msg);
        client.receiveMessage(msg);
        
        if(client.inError())
        {
            std::cout << client.error() << "\n";
        }

        std::cout << msg.type() << "\n";
    }
    return 0;
}
