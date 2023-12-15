#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QStringList>
#include <QDebug>



/*
1.Установить postgresql
#!/bin/bash

# Обновление и установка PostgreSQL
sudo apt update
sudo apt install -y postgresql postgresql-contrib libpq

# Переключение на пользователя postgres и выполнение SQL команд
sudo -u postgres psql << EOF
CREATE ROLE layouttooluser WITH LOGIN PASSWORD '12345678';
CREATE DATABASE layouttooldb;

Параметризация создания базы данных в PostgreSQL позволяет настроить различные аспекты базы данных, такие как кодировка, сортировка, правила управления временем и другие. Вот несколько опций, которые можно добавить при создании базы данных:

ENCODING

Указывает кодировку для базы данных.
Пример: ENCODING 'UTF8'
LC_COLLATE

Определяет правила сортировки (например, для алфавитной сортировки).
Пример: LC_COLLATE 'en_US.utf8'
LC_CTYPE

Определяет классификацию символов (например, какие символы являются буквами).
Пример: LC_CTYPE 'en_US.utf8'
TABLESPACE

Указывает табличное пространство, в котором будут храниться файлы базы данных.
Пример: TABLESPACE mytablespace
OWNER

Устанавливает владельца базы данных.
Пример: OWNER myuser
TEMPLATE

Определяет шаблон базы данных, на основе которого будет создана новая база данных.
Пример: TEMPLATE template0
CONNECTION LIMIT

Ограничивает количество одновременных подключений к базе данных.
Пример: CONNECTION LIMIT 100

CREATE DATABASE layouttooldb WITH
    OWNER = layouttooluser,
    ENCODING = 'UTF8'
     TEMPLATE = template0
    LC_COLLATE = 'en_US.utf8'
    LC_CTYPE = 'en_US.utf8'
    CONNECTION LIMIT = 100;


GRANT ALL PRIVILEGES ON DATABASE layouttooldb TO layouttooluser;
EOF


Дебаг
\t - посмотреть таблицы
\du - список всех пользоователей
*/





int main(int argc, char** argv)
{
   qDebug()<< QSqlDatabase::drivers();

   QSqlDatabase database = QSqlDatabase::addDatabase("QPSQL", "LayoutToolConn");
   database.setHostName("localhost");
   database.setDatabaseName("layouttooldb");
   database.setUserName("layouttooluser");
   database.setPassword("newpassword");

   if(!database.open())
   {
      qDebug() << database.lastError();
      return -1;
   }

   //Запросы на создание и заполнение таблицы layout_file_extensions
   QSqlQuery query(database);
   auto execQueries = [&query](const QString Name, const QStringList& Queries)
   {
      if(Queries.empty()) { return true;}


      qDebug() << "Создание таблицы " << Name;
      for(const auto& it : Queries)
      {
         qDebug() << "Выполнение запроса\n" << it;
         if(!query.exec(it))
         {
            qDebug() << query.lastError();
            return false;
         }
      }
      return true;
   };

  const QStringList layout_file_ext_queries =
   {
      "CREATE TABLE IF NOT EXISTS layout_file_extensions ("
      "id SERIAL PRIMARY KEY, "
      "ext_name TEXT NOT NULL UNIQUE);",

      "CREATE UNIQUE INDEX IF NOT EXISTS idx_ext_name ON layout_file_extensions (ext_name);",

      "INSERT INTO layout_file_extensions (ext_name) VALUES ('gdsii'), ('cif'), ('msk') ON CONFLICT (ext_name) DO NOTHING;"
   };
   const QStringList layout_libraries_queries =
   {
      "CREATE TABLE IF NOT EXISTS layout_libraries ("
      "id SERIAL PRIMARY KEY, "
      "name TEXT NOT NULL, "
      "layout_file_ext_id INTEGER NOT NULL, "
      "created_at TIMESTAMP NOT NULL DEFAULT now(), "
      "FOREIGN KEY (layout_file_ext_id) REFERENCES layout_file_extensions(id) ON DELETE CASCADE);",

      "CREATE INDEX IF NOT EXISTS idx_layout_file_ext_id ON layout_libraries (layout_file_ext_id);"
   };

   const QStringList layout_files_queries =
   {
      "CREATE TABLE IF NOT EXISTS layout_files ("
      "id SERIAL PRIMARY KEY, "
      "name_alias TEXT NOT NULL, "
      "file_path TEXT NOT NULL UNIQUE, "
      "stbg_hash_hex TEXT, " // Убрано NOT NULL для гибкости
      "layout_file_ext_id INTEGER NOT NULL, "
      "library_id INTEGER NOT NULL, "
      "created_at TIMESTAMP NOT NULL DEFAULT now(), "
      "FOREIGN KEY (layout_file_ext_id) REFERENCES layout_file_extensions(id) ON DELETE CASCADE, "
      "FOREIGN KEY (library_id) REFERENCES layout_libraries(id) ON DELETE CASCADE);",

      "CREATE UNIQUE INDEX IF NOT EXISTS idx_name_alias ON layout_files (name_alias);"
   };

   const QStringList layout_signatures_queries =
   {
      "CREATE TABLE IF NOT EXISTS layout_signatures ("
      "id SERIAL PRIMARY KEY, "
      "layout_file_id INTEGER NOT NULL, "
      "file_path TEXT NOT NULL, "
      "stgb_hex_hash TEXT NOT NULL, "
      "created_at TIMESTAMP NOT NULL DEFAULT now(), "
      "FOREIGN KEY (layout_file_id) REFERENCES layout_files(id) ON DELETE CASCADE);"
   };

   const QStringList layout_matricies_queries =
   {
      "CREATE TABLE IF NOT EXISTS layout_matricies ("
      "id SERIAL PRIMARY KEY, "
      "layout_file_id INTEGER NOT NULL, "
      "file_path TEXT NOT NULL, "
      "stbg_hex_hash TEXT NOT NULL, "
      "matrix_size POINT NOT NULL, "
      "is_all_layout BOOLEAN NOT NULL DEFAULT FALSE, "
      "bounding_box BOX NOT NULL, "
      "created_at TIMESTAMP NOT NULL DEFAULT now(), "
      "FOREIGN KEY (layout_file_id) REFERENCES layout_files(id) ON DELETE CASCADE);"
   };

   execQueries("layout_file_extensions", layout_file_ext_queries);
   execQueries("layout_libraries", layout_libraries_queries);
   execQueries("layout_files", layout_files_queries);
   execQueries("layout_signatures", layout_signatures_queries);
   execQueries("layout_matricies", layout_matricies_queries);

   qDebug() << "DbCreated";
   return 0;

}