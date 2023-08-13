#ifndef __TYPES_H__
#define __TYPES_H__

#include <cstdint>
#include <string>


#define STRINGIZE(x) #x
#define TO_STRING(x) STRINGIZE(x)

#define GET_LINE_STR std::string("LINE: " TO_STRING(__LINE__) " FILE: " __FILE__)

#define CREATE_EXCEPTION_STR() ("Exception happened on " + GET_LINE_STR + ":\n")


#endif //!__TYPES_H__