#pragma once

class FileLoader
{
    private:
    std::string name;
    
    public:
    FileLoader(std::string name);
    
    std::string read();
};