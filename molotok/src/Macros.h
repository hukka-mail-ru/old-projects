#ifndef MACROS_H_
#define MACROS_H_

#include <iostream> 
#include <sstream> 
#include <fstream>
#include <stdexcept>
#include <boost/shared_ptr.hpp> 

#define FLOAT_UNDEFINED 666.0

#define CLASSPTR(name) \
  class name; \
  typedef boost::shared_ptr< name > name##Ptr; \


#define TRY_BEGINS \
try \
{ \


#define TRY_RETHROW \
} \
catch(std::runtime_error& err) \
{ \
    std::ostringstream os; \
    os << "[" << __FUNCTION__ << "]->" << err.what(); \
    throw runtime_error(os.str()); \
}


#define TRY_CATCH \
} \
catch(std::runtime_error& err) \
{ \
std::cout << "EXCEPTION in " << __FUNCTION__ << std::endl << "TRACE: "<< err.what() << std::endl;\
std::cerr << "EXCEPTION in " << __FUNCTION__ << std::endl << "TRACE: "<< err.what() << std::endl;\
std::ofstream out("err.txt");\
out << "EXCEPTION in " << __FUNCTION__<< std::endl << "TRACE: "<< err.what()<< std::endl;\
}
/*
std::cout << "EXCEPTION in " << __FUNCTION__ << std::endl << \
             "TRACE: "<< err.what() << std::endl << std::endl;\
std::cerr << "EXCEPTION in " << __FUNCTION__ << std::endl << \
             "TRACE: "<< err.what() << std::endl << std::endl;\
std::ofstream out("err.txt");\
    out << "EXCEPTION in " << __FUNCTION__ << std::endl << \
    "TRACE: "<< err.what() << std::endl << std::endl;\             
  
}  */


#define SHOW_FUNCTION_NAME \
std::cout << std::endl  << __FUNCTION__ << std::endl; \

#endif /*MACROS_H_*/
