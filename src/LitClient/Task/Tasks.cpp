#include "Tasks.hpp"
#include "Helpers.hpp"

extern "C"
{
   #include "Include/Hash/Crc32.h"
}

void
Crc32GenTask::run()
{
   auto fileBinary = Helpers::GetFileBinary(fileName.toStdString());
   if(fileBinary.empty()) { }
   emit TaskBegin();
   const auto hash = Crc32CalcHash(fileBinary.data(), fileBinary.size());
   auto hashBytea = QByteArray(reinterpret_cast<const char*>(&hash), sizeof(hash));
   emit HashReady(hashBytea);
   emit TaskReady();
}