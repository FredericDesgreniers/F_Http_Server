#include "Template.h"

Template::Template(std::string name) : name(name)
{
    
}

std::string Template::execute()
{
    //NOTE: The path should probably be able to be changed
    //      Possibly via config file
    FileLoader fileLoader("templates/"+name);
    return fileLoader.read();
}