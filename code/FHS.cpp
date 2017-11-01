#include "includes.h"
#include <filesystem>
#include <experimental/filesystem> 
#include <direct.h>
namespace fs = std::experimental::filesystem;

void handleIndex(HttpRequest* request, HttpResponse* response)
{
    std::stringstream fileListString;
    
    std::string path = "files";
    for(auto &p : fs::directory_iterator(path))
    {
        fileListString << p << "\n";
    }
    response->addHeader({"Content-Disposition","inline"});
    response->sendBody(fileListString.str());
}


//NOTE: This could probably be handled by default. 
void handleFile(HttpRequest* request, HttpResponse* response)
{
    std::string outputPath = "files/"+request->info.path.substr(1);
    
    if(outputPath.find(":") != std::string::npos || outputPath.find("..") != std::string::npos)
    {
        response->sendStatus("204", "Non secure path");
        response->sendBody("Non secure path\n");
        return;
    }
    
    switch(request->info.type)
    {
        case GET:
        {
            std::ifstream fileToRead;
            fileToRead.open(outputPath);
            if(fileToRead.is_open())
            {
                std::stringstream fileBody;
                std::string line;
                while(getline(fileToRead, line))
                {
                    fileBody << line;
                }
                response->addHeader({"Content-Disposition","attachement"});
                response->sendBody(fileBody.str()+"\n");
            }
            else
            {
                response->sendStatus("404", "File not found...");
                response->sendBody("404\n");
            }
            
            
        }break;
        
        case POST:
        {
            response->sendBody("Putting file "+request->body + " at " + request->info.path + "\n");
            
            std::ofstream fileToWrite;
            
            _mkdir("files");
            
            fileToWrite.open(outputPath);
            fileToWrite << request->body;
            fileToWrite.close();
        }break;
    }
}


void handleAbout(HttpRequest* request, HttpResponse* response)
{
    response->sendBody(getRequestTypeString(request->info.type) + "about me page");
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


int main()
{
    HttpServer httpServer("8080");
    
    httpServer.addRegexPath({std::regex("^\/$"), &handleIndex});
    httpServer.addRegexPath({std::regex("^\/.+"), &handleFile});
    
    httpServer.start();
    
    //SOCKET client = createClientSocket("localhost", SERVER_PORT);
    std::cout << "Server shut down, press enter to continue..." << std::endl;
    std::cin.get();
}

