#pragma once

class Template
{
    private:
    std::string name;
    
    std::map<std::string, std::string> valueMap;
    public:
    Template(std::string name);
    
    void reset();
    
    void setValue(std::string key, const char *value);
    void setValue(const char *key, const char *value);
    void setValue(const char *key, std::string value);
    void setValue(std::string key, std::string value);
    std::string execute();
};

struct Node
{
    std::string value;
    Node* nextNode;
    
    virtual std::string getValue(std::map<std::string, std::string> &valueMap);
};

struct ValueNode: Node
{
    std::string getValue(std::map<std::string, std::string> &valueMap);
};