#pragma once

class Template
{
    private:
    std::string name;
    
    public:
    Template(std::string name);
    
    std::string execute();
};