#ifndef __LAYOUT_WORKER_H__
#define __LAYOUT_WORKER_H__

#include <QObject>

#include <string>

class LayoutWorker : public QObject
{
   Q_OBJECT

   enum class Status
   {
      Success,
      Error
   };

   void
   CrcReady(uint32_t Token, const QByteArray& Crc32, LayoutWorker::Status Status);

   public:

   explicit LayoutWorker(QObject* Parent = nullptr) : QObject(Parent) {}

   void
   GetCrc(uint32_t Token, const QString& FileName, const QString& PathToSave = QString());

   void
   GetLayoutMatrixGen(uint32_t Token, const QString& FileName);

   void
   GetGeometrySignaturesGeneration(uint32_t Token, const QString& FileName);

   private:
   std::string
   GetFileBinary(const std::string& FileName);

};


#endif //!__LAYOUT_WORKER_H__