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
    Template t("templates/test.ft");
    
    response->sendBody(t.execute());
}

//NOTE: static resources shouldn't be handled by the user since it's the same each time
void handleStaticResources(HttpRequest* request, HttpResponse* response)
{
    std::vector<std::string> parts = request->info.getVectorPath();
    
    std::string resourcePath;
    
    for(int i=2; i < parts.size()-1; i++)
    {
        resourcePath += parts[i]+"/";
    }
    
    resourcePath += parts[parts.size()-1];
    
    //This works since templates aren't dynamic at the moment
    //Should switch to something else once templates don't
    //simply load a file as is. 
    //Or should static resources be dynamic? 
    Template resourceTemplate("resources/"+resourcePath); 
    
    response->sendBody(resourceTemplate.execute());
}

//NOTE: This could probably be handled by default. 
void handle404(HttpRequest* request, HttpResponse* response)
{
    response->sendStatus("404", "page not found");
    
    response->sendBody("404 - page not found! - "+request->info.path);
}


int main()
{
    //TODO: Move somewhere else, possibly in httpServer constructor
    //      aka should be handled by default
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
    httpServer.addRegexPath({std::regex("^\/static\/.+"), &handleStaticResources});
    httpServer.addRegexPath({std::regex("^\/.+"), &handle404});
    
    httpServer.start();
    
    //SOCKET client = createClientSocket("localhost", SERVER_PORT);
    std::cout << "Server shut down, press enter to continue..." << std::endl;
    std::cin.get();
}

