#include "Template.h"

Template::Template(std::string name) : name(name)
{
    
}

std::string Template::execute()
{
    std::string result = "";
    
    //NOTE: Should template file be loaded on template creation and re-used? 
    //      Keeping it in execute allows changing the without restarting the program
    std::ifstream templateFile(name);
    
    //Check to see if file is opened, read all the line and output
    if(templateFile.is_open()){
        std::string line;
        while(getline(templateFile, line))
        {
            result += line + "\n";
        }
        templateFile.close();
    }
    else
    {
        return "No file found for " + name;
    }
    return result;
}