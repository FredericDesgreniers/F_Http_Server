#include "Template.h"

Template::Template(std::string name) : name(name)
{
    
}

std::string Template::execute()
{
    std::string result = "";
    
    std::ifstream templateFile("templates/"+name);
    if(templateFile.is_open()){
        std::string line;
        while(getline(templateFile, line))
        {
            result += line + "\n";
        }
        templateFile.close();
    }
    return result;
}