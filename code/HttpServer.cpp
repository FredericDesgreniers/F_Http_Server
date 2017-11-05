#include "HttpServer.h"

WSADATA wsaData;

HttpRequest::HttpRequest(HttpRequestInfo info) : info(info)
{
    
}

std::vector<std::string> HttpRequestInfo::getVectorPath()
{
    std::stringstream ss;
    ss.str(path);
    std::string item;
    
    std::vector<std::string> result;
    
    while(std::getline(ss, item, '/'))
    {
        result.push_back(item);
    }
    
    return result;
}

HttpResponse::HttpResponse(ClientSocket* destination) : destination(destination)
{
}

HttpResponse::~HttpResponse()
{
    delete destination;
}

void HttpResponse::addHeader(Header header)
{
    headers.push_back(header);
}

void HttpResponse::sendStatus(std::string code, std::string message)
{
    //This is the standard http header
    //TODO add way to change version, but default version should stay 1.1
    destination->sendString("HTTP/1.1 "+code+" "+message+"\r\n");
    statusSent = true;
}

void HttpResponse::sendHeaders()
{
    //Always send status before headers
    //If not sent, send it before sending the headers.
    //Default is 200 OK since request is assume to be furfiled if we send headers
    if(!statusSent)
    {
        sendStatus("200", "OK");
    }
    
    std::string headerString = "";
    //header format is always "key: value \r\n"
    for(Header header : headers)
    {
        headerString += header.name + ": " + header.value + "\r\n";
    }
    headerString += "\n";
    
    destination->sendString(headerString);
    headersSent = true;
}

void HttpResponse::sendBody(std::string body)
{
    //Always send headers if we haven't sent them before sending the body
    //this also will send status if not sent.
    if(!headersSent)
    {
        sendHeaders();
    }
    
    destination->sendString(body);
}

void HttpResponse::close()
{
    destination->close();
}

HttpServer::HttpServer(std::string port)
{
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    
    if(iResult != 0)
    {
        printf("WSAStartup has failed: %d\n", WSAGetLastError());
        std::cin.get();
        exit(EXIT_FAILURE);
    }
    
    serverSocket = new ServerSocket({"localhost", port});
}

void HttpServer::start()
{
    
    ClientSocket* client;
    
    while((client = serverSocket->listenForClient()) != nullptr)
    {
        std::thread t(&HttpServer::createRequest, this, client);
        
        //We detatch the thread so it can safely be freed from memory
        //Not doing this would require us to keep the thread object
        //for the duration of the request, which would need to be
        //cleaned up after. 
        t.detach();
    }
    
}

void HttpServer::createRequest(ClientSocket* client)
{
    //The first thing sent to the server is the request string
    //which contains info such as headers, path, method
    std::string requestString;
    client->readString(requestString);
    
    //Use this stream to traverse the request string
    std::stringstream iss(requestString);
    int i=0;
    
    std::string line; 
    
    //This first line is http version, method and path
    std::getline(iss, line);
    
    //following code splits the string at whitespace
    //and then pushes onto a vector
    std::stringstream httpSS(line);
    std::string httpSeg;
    std::vector<std::string> httpArgs;
    while(std::getline(httpSS, httpSeg, ' '))
    {
        httpArgs.push_back(httpSeg);
    }
    
    //This means their wasn't anything in the first line
    //We assume this isn't an http request
    if(httpArgs.size() == 0)
    {
        delete client;
        return;
    }
    
    //If first line isn't empty, map args
    std::string requestType = httpArgs.at(0);
    std::string requestPath = httpArgs.at(1);
    std::string requestVersion = httpArgs.at(2);
    
    std::transform(requestType.begin(), requestType.end(), requestType.begin(), ::tolower);
    
    HttpRequest* request = new HttpRequest({(requestType == "get")? GET:POST, requestPath, requestVersion});
    
    //Get all the headers line by line
    //each header only spans 1 line and is split into 
    //key: value
    while(std::getline(iss, line))
    {
        std::stringstream headerSS(line);
        std::string value;
        std::string key;
        //Get string up to first :, which should be the key
        getline(headerSS, key , ':');
        
        //Get string to to the next \n, which should be the value
        getline(headerSS, value);
        
        if (key == "\r")
        {
            break;
        }
        
        //If it's not blank, add it to the request headers
        if(key.length() > 0)
        {
            request->headers[key] = Header{key, value}; 
        }
        else
        {
            break;
        }
        
    }
    
    std::stringstream bodyStream;
    
    while(getline(iss, line))
    {
        bodyStream << line;
    }
    
    request->body = bodyStream.str();
    
    processRequest(request, new HttpResponse(client));
    
    
}

void HttpServer::processRequest(HttpRequest* request, HttpResponse* response)
{
    //TODO add way to properly log
    std::cout << "Processing "<< getRequestTypeString(request->info.type) << " response for path " << request->info.path << std::endl;
    
    //response->addHeader({"Transfer-Encoding", "chunked"});
    
    //These are the default assumed headers
    //We should properly find a more elegant way
    //of doing this. Their should be a way to disable this
    response->addHeader({"Connection", "close"});
    response->addHeader({"Content-Type","text/html; charset=UTF-8"});
    response->addHeader({"Content-Encoding","UTF-8"});
    
    //Go through the paths.
    //The earlier a path gets added, the more priority it has
    //We only allow 1 path to be used per request, so we break
    //as soon as we find a valid one
    for(Path p : paths)
    {
        if(std::regex_match(request->info.path, p.regex))
        {
            p.func(request, response);
            break;
        }
    }
    
    response->close();
    //Once response is closed, their should be no reason to keep
    //the request and the response.
    delete request;
    delete response;
}

void HttpServer::addRegexPath(Path path)
{
    paths.push_back(path);
}