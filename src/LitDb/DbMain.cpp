#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QDebug>


int main(int argc, char** argv)
{
   qDebug()<< QSqlDatabase::drivers();

   QSqlDatabase database = QSqlDatabase::addDatabase("QPSQL", "LayoutToolConn");
   database.setHostName("localhost");
   database.setUserName("postgres");
   database.setPassword("12345678");

   if(!database.open()) 
   { 
      qDebug() <<database.lastError();
      return -1;
   }
   qDebug() << "DbCreated";

   const QString username = "layouttooluser";
   const QString password = "12345678";

   QSqlQuery query(database);

   QString checkRoleExistenceQuery = QString("SELECT exists( select 1 FROM pg_roles WHERE rolname='%1');").arg(username);
   qDebug() << checkRoleExistenceQuery;
   if (query.exec(checkRoleExistenceQuery)) 
   {
      query.first();
      qDebug() << query.value(0).toBool();
      if (!query.value(0).toBool()) 
      {
         // Роль не существует, создайте ее
         QString createUserQuery = QString("CREATE ROLE %1 WITH LOGIN PASSWORD '%2' CREATEDB REPLICATION;").arg(username).arg(password);
         if (!query.exec(createUserQuery)) 
         {
            qDebug() << "Ошибка создания роли:" << query.lastError().text();
         }
   } 
   else 
   {
        qDebug() << "Роль уже существует.";
   }
   } 
   else 
   {
      qDebug() << "Ошибка при проверке существования роли:" << query.lastError().text();
   }


   return 0;

}