#ifndef __TASK_MANAGER_H__
#define __TASK_MANAGER_H__

#include <QObject>
#include <QThreadPool>
#include <QMutex>
#include <QMutexLocker>
#include <QRunnable>
#include <QString>

#include <atomic>
#include <condition_variable>
#include <thread>
#include <queue>
#include <list>

#include "Workers/TokenGenerator.hpp"

class ITask;


using TaskId = std::size_t;

class TaskManager : public QObject
{
   Q_OBJECT
   public:
   signals:
   void TaskStarted(TaskId, Token);

   void TaskProcess(TaskId, Token, std::size_t Percent);

   void TaskReady(TaskId, Token);

   void CancellAll();


   private:
   TaskManager();
   ~TaskManager();

   public:


   static
   TaskManager& Instance() 
   {
      static TaskManager manager;
      return manager;
   }

   void
   AddTask(ITask* NewTask);

   bool
   IsInQueue(ITask* Task2Check) const;

   private:
   void ManagerEventPool();

   void StopTasks();

   private:
   void CompleteTask(TaskId Id, Token Tok);


   private:
   //loop
   std::atomic_bool stopFlag;
   std::thread loopThread;
   std::condition_variable queueCv;
   std::condition_variable tpoolCv;

   QThreadPool threadPool;
   mutable std::mutex tpoolMutex;
   mutable std::mutex queueMutex;
   std::queue<ITask*> taskQueue;

   std::list<ITask*> runningTasks;
};



#endif //!__TASK_MANAGER_H__