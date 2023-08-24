#include <QProcess>
#include <QStringList>
#include <QWindow>
#include <QHBoxLayout>
#include <QListView>
#include <QFileDialog>
#include <QMessageBox>

#include <QDebug>

#include "LcLayoutPage.hpp"
#include "Include/LayoutReader.hpp"

constexpr std::string_view path = "/home/justfunde/work/Layout-identification-tool/src/Common/Libs/LayoutReader/Test/MskFiles/2LABA.MSK";

LcLayoutPage::LcLayoutPage(QWidget* Parent)
   : QWidget(Parent)
   , tb(new QToolBar)
   , mainW(new QWidget)
   , splitter(new QSplitter)
   , layoutNamesView(new QListView())
   , layoutNamesModel(new QStringListModel)
   , layoutViewWidget(new LayoutWidget)
{
   tb->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
   mainW->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

   layoutNamesView->setModel(layoutNamesModel);

   QStringList nms  = {
      "/home/justfunde/work/Layout-identification-tool/src/Common/Libs/LayoutReader/Test/MskFiles/dd2.MSK",
      "/home/justfunde/work/Layout-identification-tool/src/Common/Libs/LayoutReader/Test/MskFiles/4.MSK"
   };

   layoutNamesModel->setStringList(nms);


   splitter->addWidget(layoutNamesView);
   splitter->addWidget(layoutViewWidget);
   
   splitter->setSizes({100,500});

   QVBoxLayout* mainLayout = new QVBoxLayout(this);
   mainLayout->addWidget(tb);
   mainLayout->addWidget(splitter);
   mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

   auto act = tb->addAction(QIcon(":imgs/Plus.ico"),"Добавить файл");

   connect(act, &QAction::triggered, this, &LcLayoutPage::OnAddLayout);
   connect(
      layoutNamesView->selectionModel(),
      &QItemSelectionModel::currentChanged,
      [this](const QModelIndex& Current)
      {
         if(Current.isValid())
         {
            layoutViewWidget->setFile(Current.data(Qt::DisplayRole).toString().toStdString());
         }
      });
}


bool
LcLayoutPage::IsLayoutFile(
   const QString& Path)
{
   if(Path.isEmpty()) { return false; }

   bool ret = false;
   LayoutReader* reader = GetReader(Path.toStdWString());
   if(nullptr == reader) { return false; }

   lds::LayoutData data;
   ret = reader->Read(&data);
   FreeReader(reader);
   return ret;
}

void
LcLayoutPage::OnAddLayout()
{
   const QString path = QFileDialog::getOpenFileName(
      this,
      "Открыть файл топологии",
      "/home/justfunde/work/Layout-identification-tool/src/Common/Libs/LayoutReader/Test",
      "Layout files (*.msk *.gdsii)",
      nullptr,
      QFileDialog::Option::ReadOnly);

   ImportLayoutFile(path);
}

void
LcLayoutPage::ImportLayoutFile(
   const QString& Path)
{
   if(!IsLayoutFile(Path))
   {
      QMessageBox::warning(
         this,
         "Ошибка при чтении файла топологии",
         "Не удалось проверить корректность файла топологии!\n Файл не будет добавлен в базу данных!");
         return;
   }
   names.append(Path);

   layoutNamesModel->setStringList(names);
}