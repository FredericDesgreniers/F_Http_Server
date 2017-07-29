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
    static int counter = 0;
    static Template t("test.ft");
    
    t.reset();
    
    t.setValue("counter", std::to_string(counter++));
    
    response->sendBody(t.execute());
}

//NOTE: static resources shouldn't be handled by the user since it's the same each time
void handleStaticResources(HttpRequest* request, HttpResponse* response)
{
    std::vector<std::string> parts = request->info.getVectorPath();
    
    //This whole path thing should be handled by the http server. 
    //This would get tedious if used for a lot of different things
    
    std::string resourcePath;
    
    for(int i=2; i < parts.size()-1; i++)
    {
        resourcePath += parts[i]+"/";
    }
    
    resourcePath += parts[parts.size()-1];
    
    //Should probably make a resource loader that defaults to /resources
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

