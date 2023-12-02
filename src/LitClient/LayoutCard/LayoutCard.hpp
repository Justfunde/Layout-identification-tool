#ifndef __LAYOUT_CARD_H__
#define __LAYOUT_CARD_H__

#include <QDialog>
#include <QLabel>
#include <QPushButton>

#include <QWidget>

#include <vector>

class LayoutCard : public QDialog
{
   Q_OBJECT
   signals:
   void
   AskForCrc(uint32_t Token, const QString& FileName, const QString& PathToSave = QString());

   void
   AskForLayoutMatrixGen(uint32_t Token, const QString& FileName);

   void
   AskForGeometrySignaturesGeneration(uint32_t Token, const QString& FileName);

   public:
   explicit
   LayoutCard(const QString& Name, QWidget* Parent = nullptr);

   private:

   QWidget*
   CreateInfoWidget();


   private:
   uint32_t token;
   const QString fName;
   QLabel* hashLbl;
};


#endif //!__LAYOUT_CARD_H__