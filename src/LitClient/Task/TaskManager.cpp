#include "TaskManager.hpp"
#include "Tasks.hpp"

#include <algorithm>


TaskManager& TaskManager::Instance() 
{
   class TaskManagerWr
   {
      public:
      TaskManager m;
      QThread thread;
      std::atomic_bool inited = false;
   };
   static TaskManagerWr wr;
   if(!wr.inited.load())
   {
      wr.m.moveToThread(&wr.thread);
      wr.inited.store(true);
   }
   return wr.m;
}

TaskManager::TaskManager()
   : stopFlag(false)
   , loopThread(&TaskManager::ManagerEventPool, this)
{
   threadPool.setMaxThreadCount(16);
}


void TaskManager::StopTasks()
{
   std::unique_lock l(tpoolMutex);
   for(ITask* it : runningTasks)
   {
      it->Stop();
   }
}


TaskManager::~TaskManager()
{
   stopFlag.store(true);
   loopThread.join();
   StopTasks();
   threadPool.clear();
}


void 
TaskManager::ManagerEventPool()
{
   while(!stopFlag)
   {
      std::unique_lock queueLock(queueMutex);
      queueCv.wait(queueLock, [this]() {return !taskQueue.empty() || stopFlag; });
      if(stopFlag)
      {
         return;
      }
      ITask* task = taskQueue.front();
      taskQueue.pop();

      queueLock.unlock();

      std::unique_lock tpoolLock(tpoolMutex);
      if(threadPool.activeThreadCount() > 16)
      {
         tpoolCv.wait(tpoolLock, [this]() { return threadPool.activeThreadCount() < 16 || stopFlag; });   
         if(stopFlag) { return;}
      }
      threadPool.start(task);
      runningTasks.push_back(task);
   }
}


void
TaskManager::AddTask(ITask* NewTask)
{
   if(!NewTask) { return;}
   std::unique_lock<std::mutex> l(queueMutex);
   taskQueue.push(NewTask);
   connect(NewTask, &ITask::TaskBeginRaw, this,   &TaskManager::TaskStarted);
   connect(NewTask, &ITask::TaskProcessRaw, this, &TaskManager::TaskProcess);
   connect(NewTask, &ITask::TaskReadyRaw, this,   &TaskManager::TaskReady);

   queueCv.notify_all();
}


bool
TaskManager::IsInQueue(ITask* Task2Check) const
{
   if(!Task2Check) { return false;}

   std::unique_lock<std::mutex> l(queueMutex);
   if(taskQueue.empty()) { return false;}

   std::queue<ITask*> toIterateQueue = taskQueue;

   while(!toIterateQueue.empty())
   {
      auto currTask = toIterateQueue.front();
      if(currTask == Task2Check) { return true;}
      toIterateQueue.pop();
   }

   return false;
}

void 
TaskManager::CompleteTask(TaskId Id, Token Tok)
{
   if(!Id || !Tok) { return;}
   runningTasks.remove_if([Id, Tok](ITask* curr) { return curr->GetId() == Id && curr->GetToken() == Tok;});
   tpoolCv.notify_all();
}