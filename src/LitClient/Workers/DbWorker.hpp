#ifndef __DB_WORKER_H__
#define __DB_WORKER_H__

#include <QObject>
#include <QThread>

class DbWorker : public QObject
{
   Q_OBJECT

   public:


   protected:

   void
   InitDb();


   QThread workerThread;
};

#endif //!__DB_WORKER_H__