#ifndef __DB_H__
#define __DB_H__

#include <QSqlDatabase>

class DbInstaller
{

   public:

   static DbInstaller& GetInstance()
   {
      static DbInstaller inst;
      return inst;
   }

   private:

   bool
   CreateDatabase();

   bool
   CreateTables();

   void
   ExecQueries(
      const QStringList& Queries);


   private:
   QSqlDatabase dbIsntance;
};


#endif //!__DB_H__