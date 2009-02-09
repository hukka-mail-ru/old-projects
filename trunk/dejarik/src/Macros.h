#ifndef MACROS_H_
#define MACROS_H_

#include <boost/shared_ptr.hpp> 

#define TRY_BEGINS \
try \
{   

#define RETHROW(LOG) \
} \
catch(string& err) \
{ \
    string str = "["; \
    str += LOG; \
    str += "]->"; \
    str += err; \
    throw str; \
}

#define CLASSPTR(CLASS) \
typedef boost::shared_ptr<CLASS> CLASS##Ptr;

#endif /*MACROS_H_*/
