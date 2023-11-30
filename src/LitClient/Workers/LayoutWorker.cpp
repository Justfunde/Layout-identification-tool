#include "LayoutWorker.hpp"

#include "Include/Crc32.h"

#include <QFile>

#include <fstream>
#include <sstream>

void
LayoutWorker::GetCrc(uint32_t Token, const QString& FileName, const QString& PathToSave)
{
   LayoutWorker::Status status = Status::Success;
   QByteArray hashBytea;
   try{
      auto fileBinary = GetFileBinary(FileName.toStdString());
      if(fileBinary.empty()) { }

      const auto hash = Crc32::CalcHash(fileBinary.data(), fileBinary.size());
      hashBytea = QByteArray(reinterpret_cast<const char*>(&hash), sizeof(hash));

      if(!PathToSave.isEmpty())
      {
         QFile f(PathToSave);
         if(!f.open(QIODevice::WriteOnly)) { }
         f.write(hashBytea);
         f.close();
      }

   }
   catch(...)
   {
      status = Status::Error;
   }
   emit CrcReady(Token, hashBytea, status);

}


void
LayoutWorker::GetLayoutMatrixGen(uint32_t Token, const QString& FileName)
{

}


void
LayoutWorker::GetGeometrySignaturesGeneration(uint32_t Token, const QString& FileName)
{


}

std::string
LayoutWorker::GetFileBinary(const std::string& FileName)
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