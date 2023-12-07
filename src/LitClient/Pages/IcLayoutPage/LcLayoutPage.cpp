#include <QProcess>
#include <QStringList>
#include <QHBoxLayout>
#include <QListView>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QMessageBox>

#include <QDebug>
#include <QDir>

#include "LcLayoutPage.hpp"


#include "LayoutCard/LayoutCard.hpp"

#include "Include/LayoutReader.hpp"
#include <string_view>


constexpr std::string_view path = "/home/justfunde/work/Layout-identification-tool/src/Common/Libs/LayoutReader/Test/MskFiles/2LABA.MSK";

LcLayoutPage::LcLayoutPage(QWidget* Parent)
   : QWidget(Parent)
   , tb(new QToolBar)
   , mainW(new QWidget)
   , splitter(new QSplitter)
   , layoutNamesView(new QTreeView)
   , layoutViewWidget(new LayoutWidget)
   , layoutFilesPathModel(new QFileSystemModel)
{
   tb->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
   mainW->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

   const QString path = "/home/justfundeer/work/Layout-identification-tool/src/LitClient/Test/LayoutFiles";
   if(!QDir(path).exists())
   {
      return;
   }

   layoutFilesPathModel->setRootPath(path);


   layoutNamesView->setModel(layoutFilesPathModel);
   layoutNamesView->setRootIndex(layoutFilesPathModel->index(path));
   layoutNamesView->hideColumn(1);
   layoutNamesView->hideColumn(2);
   layoutNamesView->hideColumn(3);


   splitter->addWidget(layoutNamesView);
   splitter->addWidget(layoutViewWidget);
   splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

   splitter->setSizes({100,500});
   splitter->setStretchFactor(0, 2);
   splitter->setStretchFactor(1, 0);


   QVBoxLayout* mainLayout = new QVBoxLayout(this);
   mainLayout->addWidget(tb);
   mainLayout->addWidget(splitter);
   //mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

   auto act = tb->addAction(QIcon(":imgs/Plus.ico"),"Добавить файл");

   connect(act, &QAction::triggered, this, &LcLayoutPage::OnAddLayout);
   connect(
      layoutNamesView->selectionModel(),
      &QItemSelectionModel::currentChanged,
      [this](const QModelIndex& Current)
      {
         if(Current.isValid() && !layoutFilesPathModel->isDir(Current))
         {
            layoutViewWidget->setFile(layoutFilesPathModel->filePath(Current).toStdString());
         }
      });
   connect(
      layoutNamesView, 
      &QTreeView::doubleClicked, 
      this, 
      [this](const QModelIndex& Current)
      {
         if(Current.isValid() && !layoutFilesPathModel->isDir(Current))
         {
            emit AskForCardExec(layoutFilesPathModel->filePath(Current));
         }
      });
   connect(this, &LcLayoutPage::AskForCardExec, this, &LcLayoutPage::ExecCard);
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
}

void
LcLayoutPage::ExecCard(const QString& Path)
{
   if(Path.isEmpty()) { return;}

   LayoutCard* card = new LayoutCard(Path);
   card->setModal(false);
   card->show();
   connect(card, &QObject::destroyed, card, &QObject::deleteLater);
}