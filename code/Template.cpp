#include "Template.h"

Template::Template(std::string name) : name(name)
{
    
}

std::string Template::execute()
{
    //NOTE: The path should probably be able to be changed
    //      Possibly via config file
    FileLoader fileLoader("templates/"+name);
    std::string templateText = fileLoader.read();
    
    //Keep track of the first node
    Node* firstNode = new Node();
    //this is a moving node
    Node *currentNode = new Node();
    firstNode->nextNode = currentNode;
    
    char currentChar;
    int currentIndex = -1;
    while((currentChar = templateText[currentIndex++]) != 0)
    {
        switch(currentChar)
        {
            
            case '{':
            {
                Node *valueNode = new ValueNode();
                currentNode->nextNode = valueNode;
                currentNode = valueNode;
            }break;
            case '}':
            {
                Node *nextNode = new Node();
                currentNode->nextNode = nextNode;
                currentNode = nextNode;
            }break;
            
            default:
            {
                currentNode->value += currentChar;
            }break;
        }
        
    }
    
    std::string finalString = "";
    
    Node *node = firstNode;
    while((node = node->nextNode))
    {
        finalString += node->getValue(valueMap);
    }
    
    return finalString;
}

void Template::reset()
{
    valueMap.clear();
}

void Template::setValue(std::string key, const char *value)
{
    setValue(key, std::string(value));
}
void Template::setValue(const char *key, const char *value)
{
    setValue(std::string(key), std::string(value));
}
void Template::setValue(const char *key, std::string value)
{
    setValue(std::string(key), value);
}
void Template::setValue(std::string key, std::string value)
{
    valueMap.insert(make_pair(key, value));
}


std::string Node::getValue(std::map<std::string, std::string> &valueMap)
{
    return value;
}

std::string ValueNode::getValue(std::map<std::string, std::string> &valueMap)
{
    return valueMap[value];
    }