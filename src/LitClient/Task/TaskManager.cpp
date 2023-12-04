#include "TaskManager.hpp"


TaskManager::TaskManager()
   : stopFlag(false)
   , loopThread(&TaskManager::ManagerEventPool, this)

{

}

TaskManager::~TaskManager()
{
   stopFlag.store(true);
   loopThread.join();
}


void 
TaskManager::ManagerEventPool()
{
   while(!stopFlag)
   {
      std::unique_lock lock(queueMutex);
      taskCv.wait(lock, [this] {return !taskQueue.empty() || stopFlag; });
      if(stopFlag)
      {
         emit CancellAll();
         return;
      }

   }
}

void
TaskManager::AddTask(Task* NewTask)
{
   if(!NewTask) { return;}
   std::unique_lock<std::mutex> l(queueMutex);
   taskQueue.push(NewTask);
   taskCv.notify_all();
}


bool
TaskManager::IsInQueue(Task* Task2Check) const
{
   if(!Task2Check) { return false;}

   std::unique_lock<std::mutex> l(queueMutex);
   if(taskQueue.empty()) { return false;}

   std::queue<Task*> toIterateQueue = taskQueue;

   while(!toIterateQueue.empty())
   {
      auto currTask = toIterateQueue.front();
      if(currTask == Task2Check) { return true;}
      toIterateQueue.pop();
   }

   return false;
}