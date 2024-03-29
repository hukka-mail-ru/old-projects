#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "data.h"

class Parser
{ 
    friend class TestParser;

 public:
    
    Parser() 
    {}

    ~Parser()
    {}

    bool parseFile(const std::string& fileName, File& file);

 private:

    Parser(const Parser&);

    Parser& operator=(const Parser&);

    bool preg_match(const std::string& pattern,
                    std::string str,
                    std::vector<std::string>& matches,
                    bool all = false);

    bool preg_match(const std::string& pattern,
                    const std::string& str,
                    std::string& match);

    bool trim(std::string& str);

    std::string getError(int err);


    bool defineEssence(const std::string& line, Essence essence);
    

    bool parseVar(const std::string& line, Variable& var);


    bool parseFunc(const std::string& line, Function& func);


};

#endif
