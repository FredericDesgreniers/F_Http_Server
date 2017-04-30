#pragma once

struct ConnectionInfo
{
    std::string host;
    std::string port;
};


class ClientSocket
{
    private:
    SOCKET socket;
    bool connectionOpen = false;
    
    public:
    ClientSocket(SOCKET socket);
    void readString(std::string&);
    void sendString(std::string str);
    
    bool isConnectionOpen();
    
    void close();
};

class ServerSocket
{
    private:
    SOCKET listeningSocket; //winsock
    ConnectionInfo cInfo;
    public:
    ServerSocket(ConnectionInfo);
    
    ClientSocket* listenForClient();
};



