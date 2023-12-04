#ifndef __LAYOUT_CARD_H__
#define __LAYOUT_CARD_H__

#include <QDialog>
#include <QLabel>
#include <QPushButton>

#include <QWidget>

#include <vector>
#include <memory>

#include "LayoutWidget/LayoutWidget.hpp"
#include "Workers/TokenGenerator.hpp"


class LayoutCard : public QDialog
{
   Q_OBJECT
   signals:
   void
   AskForCrc(Token Token, const QString& FileName, const QString& PathToSave = QString());

   void
   AskForLayoutMatrixGen(Token Token, const QString& FileName);

   void
   AskForGeometrySignaturesGeneration(Token Token, const QString& FileName);

   public:
   explicit
   LayoutCard(const QString& Name, QWidget* Parent = nullptr);



   private:

   QWidget*
   CreateInfoWidget();


   private:

   Token token;
   const QString fName;
   QLabel* hashLbl;


   //layout view
   LayoutWidget* layoutView;
};


#endif //!__LAYOUT_CARD_H__