#ifndef __TASKS_H__
#define __TASKS_H__

#include <QRunnable>
#include <QObject>

#include "Workers/TokenGenerator.hpp"

class Task: public QObject, public QRunnable  
{
   Q_OBJECT

   public:

   Task(Token token, QObject* Parent = nullptr) : QObject(Parent), token(token), isRunnable(false) { } 

   bool
   IsRunnable() const { return isRunnable.load(); }


   public slots:

   void Stop() { isRunnable.store(false); }

   protected:
   std::atomic_bool isRunnable;
   private:
   const Token token;
};


class Crc32GenTask : public Task
{
   Q_OBJECT
   signals:
   void HashReady(const QByteArray& Bytea);

   public:
   Crc32GenTask(Token token, const QString& FileName) : Task(token), fileName(FileName) {}

   void run() override final;


   private:
   const QString fileName;
};

#endif //!__TASKS_H__