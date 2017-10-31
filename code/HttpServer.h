#pragma once

//Header stores a name, value string pair
struct Header
{
    std::string name;
    std::string value;
};

enum HttpRequestType
{
    POST, GET
};

std::string getRequestTypeString(HttpRequestType type)
{
    switch(type)
    {
        case GET:
        {
            return "GET";
        }
        case POST:
        {
            return "POST";
        }
    };
};

//HttpRequestInfo stores information about  http requests
struct HttpRequestInfo
{
    HttpRequestType type;
    std::string path;
    std::string version;
    
    //NOTE: Path things should be pulled out from here
    std::vector<std::string> getVectorPath();
};

//HttpRequest is used to keep track of http requests
class HttpRequest
{
    public:
    HttpRequest(HttpRequestInfo info);
    
    HttpRequestInfo info;
    
    std::map<std::string, Header> headers;
    
    
    std::string body;
};

//HttpResponse is used to send an http response following an http request
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
    
    //Sends status if not already sent
    void sendHeaders();
    
    //Sends headers and status if not already sent
    void sendBody(std::string body);
    
    //properly close the request
    void close();
    
};

//Path is used to match a regex path with a function that is triggered on request
struct Path
{
    std::regex regex;
    void (*func)(HttpRequest*, HttpResponse*);
};

//HttpServer keeps track of connections and requests
class HttpServer
{
    private:
    ServerSocket* serverSocket;
    
    //creates a request from a client socket
    void createRequest(ClientSocket* client);
    
    //process a request given an HttpRequest and a HttpResponse to write too
    void processRequest(HttpRequest* request, HttpResponse* response);
    
    std::vector<Path> paths;
    
    public:
    HttpServer(std::string  port);
    
    //Start the http server
    //Ideally should have set all path and information prior 
    void start();
    
    //Adds a regex path for request matching
    void addRegexPath(Path path);
};