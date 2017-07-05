#include "FileLoader.h"

FileLoader::FileLoader(std::string name) : name(name)
{
    
}

std::string FileLoader::read()
{
    std::string result = "";
    
    //NOTE: Should template file be loaded on template creation and re-used? 
    //      Keeping it in execute allows changing the without restarting the program
    std::ifstream file(name);
    
    //Check to see if file is opened, read all the line and output
    if(file.is_open()){
        std::string line;
        while(getline(file, line))
        {
            result += line + "\n";
        }
        file.close();
    }
    else
    {
        return "";
    }
    return result;
}