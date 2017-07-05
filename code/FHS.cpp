#include "includes.h"

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
    
    FileLoader fileLoader("resources/"+resourcePath); 
    
    response->sendBody(fileLoader.read());
}

//NOTE: This could probably be handled by default. 
void handle404(HttpRequest* request, HttpResponse* response)
{
    response->sendStatus("404", "page not found");
    
    response->sendBody("404 - page not found! - "+request->info.path);
}


int main()
{
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

