#include <iostream>
#include <fstream>
#include <vector>
#include <boost/shared_ptr.hpp>
//#include <algorithm>
//#include <ctype.h>

#include <pcreposix.h>
#include "parser.h"
#include "data.h"
#include <debug.hpp>

using namespace std;
using namespace common;
using namespace boost;

string Parser::getError(int err)
{
	switch(err)
	{
	case REG_BADRPT:
	return  "Invalid use of repetition operators such as using `*' as the first character.";
	case REG_BADBR:
	return  "Invalid use of back reference operator. ";
	case REG_EBRACE:
	return  "Un-matched brace interval operators. ";
	case REG_EBRACK:
	return  "Un-matched bracket list operators. ";
	case REG_ERANGE:
	return "Invalid use of the range operator, eg. the ending point of the range occurs prior to the starting point. ";
	case REG_ECTYPE:
	return  "Unknown character class name. ";
	case REG_ECOLLATE:
	return "Invalid collating element. ";
	case REG_EPAREN:
	return "Un-matched parenthesis group operators. ";
	case REG_ESUBREG:
	return  "Invalid back reference to a subexpression. ";
	case REG_EESCAPE:
	return  "Trailing backslash. ";
	case REG_BADPAT:
	return  "Invalid use of pattern operators such as group or list. ";
	case REG_ESIZE:
	return  "Compiled regular expression requires a pattern buffer larger than 64Kb. This is not defined by POSIX.2. ";
	case REG_ESPACE:
	return  "The regex routines ran out of memory.";
	default:
	return  "Unknown error";
	}
}




bool Parser::preg_match(const string& pattern,
                        string str,
                        vector<string>& matches,
                        bool all)
{

    const int MAX_TOKENS = 100;
    regex_t parsingRule;


    int res = pcreposix_regcomp(&parsingRule, pattern.c_str(), REG_EXTENDED);
    if (res != 0) 
    {
       error  << "ERROR pcreposix_regcomp: " << getError(res) << endl 
	     << "Pattern '" << pattern.c_str() << "'" << endl;
        return false;
    }


    regmatch_t match[MAX_TOKENS];// array for results of searching. 

	
    if(int res = pcreposix_regexec(&parsingRule, str.c_str(), MAX_TOKENS, match, 0))
    {
        if(res != REG_NOMATCH)
        {
            error << "ERROR pcreposix_regexec: " << getError(res) << endl 
                 << "String: '" << str.c_str() << "'" << endl
	         << "Pattern '" << pattern.c_str() << "'" << endl;
        }
        return false;
    }


    int start_ = -1;
    int end_ = -1;

    // Positions of the first and the last symbols of the found substring
    // are in the first element off the array 'match'. 
    // We don't need the match[0] because it contains
    // result of matching of the whole regexp, so start with match[1].

    int last_end = 0;   // end of the found substring
    bool found = false;

    for(int i = 1; i < MAX_TOKENS; i++)
    {
        const int start = match[i].rm_so; // beginning of the found substring
        const int end = match[i].rm_eo;   // end of the found substring

        /* last token */
        if(start == -1 && end == -1)
        {
            last_end = match[i-1].rm_eo;
            break;
        }

        /* inbound token */
        if (start >= start_ && end <= end_ && start != end_ ) 
             continue;

        start_ = start;
        end_ = end;

        if(end - start == 0)
        {
            matches.push_back("");
        }
        else
        {
            string res(str);
            res = res.substr(start, end - start);
            matches.push_back(res);
            cout << " >>>> " << res.c_str() << endl;
            found = true;
        }

    }


     if(!found)
     {
         cerr << "No matches found" << endl;
         regfree(&parsingRule);
         return false;
     }


     if(all) // if preg_match_all
     {
         str = str.substr(last_end, str.length() - last_end);
         preg_match(pattern, str, matches, true);
     }


     regfree(&parsingRule);

     return true;
}


bool Parser::preg_match(const string& pattern,
                        const string& str,
                        string& match)
{
    vector<string> res;
    if(!preg_match(pattern, str, res))
    {
        match = "";
        return false;
    }

    match = res[0];

    return true;

}





bool Parser::parseVar(const string& line, Variable& var)
{
    vector<string> res;
    if(!preg_match("^\\s*(\\w.*\\w)\\s+(\\w+)\\s*[;,=]", line, res))
    {
        return false;
    }

    var.type = res[0];
    var.name = res[1];

//cout << "var.name '" << var.name << "'" << endl;
//cout << "var.type '" << var.type << "'" << endl;

    return true;
}

bool Parser::parseFunc(const string& line, Function& func)
{

    vector<string> res;
    if(!preg_match("^\\s*(\\w.*\\w)\\s+(\\w+)\\s*(\\([^()]*\\))", line, res))
    {
        return false;
    }

    func.type   = res[0];
    func.name   = res[1];



    vector<string> args;
    preg_match("^\\s*[(,]\\s*([^,)]+)\\s*[,)]", res[2], args, true);


     for(uint i = 0; i < args.size(); i++)
     {
         args[i] += ";";
         cout << "res2: " << args[i] << endl;

         Variable var;


         if(parseVar(args[i], var))
         {
             cout << "begin" << endl;

             shared_ptr<Variable> v (new Variable);
             v->name = var.name;
             v->type = var.type;

             func.args.push_back(v);
//             func.args[i]->name = var.name;
//             func.args[i]->type = var.type;
             cout << "end" << endl;
         }
         else
         {
             cerr << "Unable to parse variable: '" << args[i] << "'" << endl;
             return false;
         }
     }

     

    return true;
}






