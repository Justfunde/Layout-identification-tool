#ifndef __TASKS_H__
#define __TASKS_H__

#include <QRunnable>
#include <QObject>

#include "Workers/TokenGenerator.hpp"

using TaskId = std::size_t;



class ITask: public QObject, public QRunnable  
{
   Q_OBJECT

   signals:

   void
   TaskBeginRaw(TaskId, Token);

   void
   TaskProcessRaw(TaskId, Token, std::size_t);

   void
   TaskReadyRaw(TaskId, Token);

   void
   TaskBegin();

   void
   TaskProcess(std::size_t);

   void
   TaskReady();


   public:

   ITask(Token token, std::size_t Id, QObject* Parent = nullptr) : QObject(Parent), token(token), id(Id), isRunnable(true) 
   {
      connect(this, &ITask::TaskProcess,    this, [this](std::size_t Percent){emit TaskProcessRaw(this->id, this->token, Percent);}   );
      connect(this, &ITask::TaskReady,      this, [this]()                   {emit TaskReadyRaw(this->id, this->token); emit TaskProcess(100);}   );
      connect(this, &ITask::TaskBegin,      this, [this]()                   {emit TaskBeginRaw(this->id, this->token); emit TaskProcess(0);}   );
   } 

   bool
   IsRunnable() const { return isRunnable.load(); }

   Token
   GetToken() const { return token;}

   TaskId
   GetId() const { return id;}


   public slots:

   void Stop() { isRunnable.store(false); }

   private:
   
   std::atomic_bool isRunnable;
   const Token token;
   const TaskId id;
};



class Crc32GenTask : public ITask
{
   Q_OBJECT
   signals:
   void HashReady(const QByteArray& Bytea);

   public:
   Crc32GenTask(Token token, const QString& FileName) : ITask(token, 1), fileName(FileName) {}

   void run() override final;

   private:
   const QString fileName;
};

#endif //!__TASKS_H__