#include <QStringList>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include <stdexcept>

#include "Db.hpp"

namespace
{
   namespace DbConnParameters
   {
      const QString driver = "QPSQL";
      const QString connectionName =  "LayoutToolConn";
      const QString hostName = "localhost";
      const QString databaseName = "layouttooldb";
      const QString databaseName = "layouttooluser";
      const QString password = "Aah293bj";
   }
   namespace CreateTableQueries
   {
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
   }
};


bool
DbInstaller::CreateTables()
{
   try
   {
      const QString tableCreationTemplate = "Cоздание таблицы %1 ...";

      qInfo() << "Создание таблиц началось...";

      qInfo() << tableCreationTemplate.arg("layout_file_extensions");
      ExecQueries(CreateTableQueries::layout_file_ext_queries);

   }
}


void
DbInstaller::ExecQueries(
   const QStringList& Queries)
{
   QSqlQuery query(dbIsntance);
   for(const auto& it : Queries)
   {
      qDebug() << "Выполнение запроса\n" << it;
      if(!query.exec(it))
      {
         throw std::runtime_error(query.lastError().text().toStdString());
      }
   }
}