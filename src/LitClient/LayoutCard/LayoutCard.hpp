#ifndef __LAYOUT_CARD_H__
#define __LAYOUT_CARD_H__

#include <QDialog>

#include <vector>

class LayoutCard : public QDialog
{
   Q_OBJECT

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


   private:
};


#endif //!__LAYOUT_CARD_H__