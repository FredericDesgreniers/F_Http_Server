#pragma once

struct Header
{
    std::string name;
    std::string value;
};

struct HttpRequestInfo
{
    std::string type;
    std::string path;
    std::string version;
    
    std::vector<std::string> getVectorPath();
};

class HttpRequest
{
    public:
    HttpRequest(HttpRequestInfo info);
    
    HttpRequestInfo info;
    
    std::map<std::string, Header> headers;
    
};

class HttpResponse
{
    ClientSocket* destination;
    
    std::vector<Header> headers;
    
    bool statusSent = false;
    bool headersSent = false;
    
    public:
    HttpResponse(ClientSocket* destination);
    
    ~HttpResponse();
    
    void addHeader(Header header);
    
    void sendStatus(std::string code, std::string message);
    
    void sendHeaders();
    
    void sendBody(std::string body);
    
    void close();
};

struct Path
{
    std::regex regex;
    void (*func)(HttpRequest*, HttpResponse*);
};

class HttpServer
{
    private:
    ServerSocket* serverSocket;
    
    void createRequest(ClientSocket* client);
    
    void processRequest(HttpRequest* request, HttpResponse* response);
    
    std::vector<Path> paths;
    
    public:
    HttpServer(std::string  port);
    
    void start();
    
    void addRegexPath(Path path);
};