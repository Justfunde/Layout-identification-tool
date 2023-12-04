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

class Task;


using TaskId = std::size_t;

class TaskManager : public QObject
{
   Q_OBJECT
   public:
   signals:
   void TaskStarted(TaskId, Token);

   void TaskProcess(TaskId, Token, uint32_t Percent);

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
   AddTask(Task* NewTask);

   bool
   IsInQueue(Task* Task2Check) const;

   private:
   void ManagerEventPool();

   private:
   //loop
   std::atomic_bool stopFlag;
   std::thread loopThread;
   std::condition_variable taskCv;

   QThreadPool threadPool;
   mutable std::mutex taskMutex;
   mutable std::mutex queueMutex;
   std::queue<Task*> taskQueue;

   std::list<Task*> runningTasks;
};



#endif //!__TASK_MANAGER_H__