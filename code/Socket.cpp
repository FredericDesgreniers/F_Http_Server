#define SERVER_PORT "27015"
#define BUF_LEN 512
#include "Socket.h"

ServerSocket::ServerSocket(ConnectionInfo cInfo) : cInfo(cInfo)
{
    
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    
    //Socket settings
    //TODO add way to change this, possible via a config you pass
    //     to the socket
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    
    int getaddrinfoResult = getaddrinfo(cInfo.host.c_str(), cInfo.port.c_str(), &hints, &result);
    
    if(getaddrinfoResult != 0)
    {
        printf("getaddrinfo failed: %d\n", getaddrinfoResult);
        return;
    }
    
    SOCKET listenSocket = INVALID_SOCKET; 
    //create the listening socket that will eventually wait for clients
    listeningSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    
    if(listeningSocket == INVALID_SOCKET)
    {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        return;
    }
    
    int bindResult = bind(listeningSocket, result->ai_addr, (int)result->ai_addrlen);
    
    if(bindResult == SOCKET_ERROR)
    {
        printf("socket bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listeningSocket);
        return;
    }
    
    //we no longer need addrinfo since socket is set up already
    freeaddrinfo(result);
    
}

ClientSocket* ServerSocket::listenForClient()
{
    if(listen(listeningSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("Listen failed with error: %ld\n", WSAGetLastError());
        closesocket(listeningSocket);
        return nullptr;
    }
    
    SOCKET client  = INVALID_SOCKET;
    
    if(((client  = accept(listeningSocket, NULL, NULL))!=INVALID_SOCKET))
    {
        //std::thread* serverlet = new std::thread(serverletThread, ClientSocket);
        return new ClientSocket(client);
    }
    
    if(client == INVALID_SOCKET)
    {
        printf("Accept failed: %d\n", WSAGetLastError());
        closesocket(listeningSocket);
        return nullptr;
    }
    
    return nullptr;
    
}


ClientSocket::ClientSocket(SOCKET socket) : socket(socket)
{
    connectionOpen = true;
}

void ClientSocket::readString(std::string& string)
{
    char rcvbuf[BUF_LEN];
    int result, sendResult;
    int rcvbuflen = BUF_LEN;
    
    result = recv(socket, rcvbuf, rcvbuflen, 0);
    //if result > 0, result is length
    if(result > 0)
    {
        string = std::string(rcvbuf);
        return;
    }
    else if(result == 0) //result == 0 means connection closed
    {
        //printf("Closing connection...\n");
    }
    else //undefined, faillure
    {
        printf("SERVER recv failed: %d\n", WSAGetLastError());
    }
    
    //if it gets here it means closed or error, both result
    //in connectionOpen to be false
    connectionOpen = false;
}

void ClientSocket::sendString(std::string str)
{
    //Send string to destination
    int sendResult = send(socket, str.c_str(), str.length(), 0);
    
    if(sendResult == SOCKET_ERROR)
    {
        printf("Send failed %ld\n", WSAGetLastError());
        //we assume an error in the send means the connection is closed. 
        //NOTE: Is that always true?
        connectionOpen = false;
        return;
    }
}

bool ClientSocket::isConnectionOpen()
{
    return connectionOpen;
}

void ClientSocket::close()
{
    shutdown(socket, SD_SEND);
    //Once we tell the connection we are done sending data, 
    //we read throught the rest of the data in order to "clean up"
    //For example, not cleaning up the http connection will result
    //in a connectiuon reset and faillure
    std::string s;
    while(connectionOpen)
    {
        readString(s);
        //std::cout << s << std::endl;
    }
    closesocket(socket);
}
