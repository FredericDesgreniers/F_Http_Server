#include "Template.h"

Template::Template(std::string name) : name(name)
{
    
}

std::string Template::execute()
{
    FileLoader fileLoader("templates/"+name);
    return fileLoader.read();
}