#include "Headers/SettingsPages.hpp"
#include "Headers/SettingsHandler.hpp"

//_______________________________________COMMON_PAGE_______________________________________

CommonPage::CommonPage(
    QWidget* Parent)
    : ISettingPageWidget(Parent)
    , fileBrowser(new FileBrowser(FileBrowser::Mode::dir))
{
    fileBrowser->SetLabel("Путь до библиотеки:");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(fileBrowser);
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding));
}

void
CommonPage::InitPage()
{
    fileBrowser->SetPathHistory(SettingsHandler::Common::GetLibPathHistory());
    qDebug() << SettingsHandler::Common::GetCurrentLibPath();
    fileBrowser->SetPath(SettingsHandler::Common::GetCurrentLibPath());
}

void 
CommonPage::Commit()
{
    
    SettingsHandler::Common::SetLibPathHistory(fileBrowser->GetPathHistory());
    SettingsHandler::Common::SetCurrentLibPath(fileBrowser->GetPath());
}


NameAnalysisPage::NameAnalysisPage(
    QWidget* Parent)
{

}


void
NameAnalysisPage::InitPage()
{

}


SquareAnalysisPage::SquareAnalysisPage(
    QWidget* Parent)
{

}


void
SquareAnalysisPage::InitPage()
{
    
}

PolyCntAnalysisPage::PolyCntAnalysisPage(
    QWidget* Parent)
{

}

void
PolyCntAnalysisPage::InitPage()
{
    
}


MutualControlAnalysisPage::MutualControlAnalysisPage(
    QWidget* Parent)
{

}


void
MutualControlAnalysisPage::InitPage()
{
    
}

BruteForcePolyAnalisysPage::BruteForcePolyAnalisysPage(
    QWidget* Parent)
{

}

void
BruteForcePolyAnalisysPage::InitPage()
{
    
}


BitmapZondingPage::BitmapZondingPage(
    QWidget* Parent)
{

}

void
BitmapZondingPage::InitPage()
{
    
}