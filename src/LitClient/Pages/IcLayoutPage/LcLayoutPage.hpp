#ifndef __LC_LAYOUT_PAGE_H__
#define __LC_LAYOUT_PAGE_H__

#include <QWidget>
#include <QToolBar>
#include <QSplitter>
#include <QString>
#include <QFileSystemModel>
#include <QTreeView>

#include "LayoutWidget/LayoutWidget.hpp"


class LcLayoutPage : public QWidget
{
   Q_OBJECT

   public:

   LcLayoutPage(QWidget* Parent = nullptr);


   public slots:

   void
   OnAddLayout();

   void
   ImportLayoutFile(
      const QString& Path);

   bool
   IsLayoutFile(
      const QString& Path);

   private:
   QToolBar* tb;
   QWidget* mainW;
   QSplitter* splitter;

   QTreeView* layoutNamesView;
   QFileSystemModel* layoutFilesPathModel;

   LayoutWidget* layoutViewWidget;

};


#endif //!__LC_LAYOUT_PAGE_H__