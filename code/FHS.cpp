#include <iostream>
#include <fstream>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <map>
#include <regex>

#include "Socket.cpp"
#include "HttpServer.cpp"
#include "Template.cpp"

WSADATA wsaData;

void handleIndex(HttpRequest* request, HttpResponse* response)
{
    response->sendBody("Index!");
}

void handleAbout(HttpRequest* request, HttpResponse* response)
{
    response->sendBody("about me page");
}

void handleTemplatePage(HttpRequest* request, HttpResponse* response)
{
    Template t("test.ft");
    response->sendBody(t.execute());
}

void handle404(HttpRequest* request, HttpResponse* response)
{
    response->sendStatus("404", "page not found");
    response->sendBody("404 - page not found!");
}


int main()
{
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    
    if(iResult != 0)
    {
        printf("WSAStartup has failed: %d\n", WSAGetLastError());
        std::cin.get();
        return 1;
    }
    
    HttpServer httpServer("8080");
    
    httpServer.addRegexPath({std::regex("^\/$"), &handleIndex});
    httpServer.addRegexPath({std::regex("^\/template$"), &handleTemplatePage});
    httpServer.addRegexPath({std::regex("^\/about$"), &handleAbout});
    httpServer.addRegexPath({std::regex("^\/.+"), &handle404});
    
    httpServer.start();
    
    //SOCKET client = createClientSocket("localhost", SERVER_PORT);
    std::cout << "Server shut down, press enter to continue..." << std::endl;
    std::cin.get();
}

