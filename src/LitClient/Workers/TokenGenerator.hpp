#ifndef __TOKEN_GENERATOR_H__
#define __TOKEN_GENERATOR_H__

#include <cstddef>

using Token = std::size_t;

namespace TokenGenerator
{
   inline
   Token
   GetNewToken()
   {
      static Token t;
      return t++;
   }
}


#endif //!__TOKEN_GENERATOR_H__