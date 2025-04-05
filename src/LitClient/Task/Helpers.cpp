#include <sstream>
#include <fstream>




#include "Helpers.hpp"



namespace Helpers
{

std::string
GetFileBinary(const std::string& FileName)
{
   std::string ret;
   if(FileName.empty()) { return ret;}

   std::ifstream f(FileName, std::ifstream::in | std::ifstream::binary);
   if(!f) { return ret;}

   std::stringstream ss;
   ss << f.rdbuf();
   ret = ss.str();
   return ret;
}


}