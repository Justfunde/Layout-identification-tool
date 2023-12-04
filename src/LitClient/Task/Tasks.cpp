#include "Tasks.hpp"
#include "Helpers.hpp"
#include "Include/Crc32.h"

void 
Crc32GenTask::run() 
{
   auto fileBinary = Helpers::GetFileBinary(fileName.toStdString());
   if(fileBinary.empty()) { }

   const auto hash = Crc32::CalcHash(fileBinary.data(), fileBinary.size());
   auto hashBytea = QByteArray(reinterpret_cast<const char*>(&hash), sizeof(hash));
   emit HashReady(hashBytea);
}