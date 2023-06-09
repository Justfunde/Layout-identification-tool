#include <QHBoxLayout>
#include <QFileDialog>

#include "FileBrowser.hpp"

FileBrowser::FileBrowser(
    Mode BrowserMode,
    QWidget* Parent)
    : QWidget(Parent)
    , currentMode(BrowserMode)
    , filterValidator("\\.[\\w\\d]{1,10}", QRegularExpression::PatternOption::CaseInsensitiveOption)
    , dirPathValidator("((?:[^/]*/)*)(.*)", QRegularExpression::PatternOption::CaseInsensitiveOption)
    , filePathValidator("/^[\\w+\\/]+$/", QRegularExpression::PatternOption::CaseInsensitiveOption)
    , label( new QLabel)
    , pathCb( new QComboBox)
    , browseBtn( new QPushButton("Выбрать"))
{
   QHBoxLayout* mainLayout = new QHBoxLayout(this);

   label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
   pathCb->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
   browseBtn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

   mainLayout->addWidget(label);
   mainLayout->addWidget(pathCb);
   mainLayout->addWidget(browseBtn);

   SetMode(currentMode);

   connect(pathCb, &QComboBox::currentTextChanged, this, &FileBrowser::PathChanged);
   connect(browseBtn, &QPushButton::clicked, this, &FileBrowser::OnBrowse);
}


QString
FileBrowser::GetPath() const
{
    return pathCb->currentText();
}

QStringList
FileBrowser::GetPathHistory() const
{
    QStringList ret;
    for(uint32_t i = 0, currentInd = pathCb->currentIndex(); i < pathCb->count(); ++i)
    {
        if(i == currentInd) { continue;}
        ret.push_back(pathCb->itemText(i));
    }
    return ret;
}


inline
FileBrowser::Mode
FileBrowser::GetMode() const
{
    return currentMode;
}

inline
const QString&
FileBrowser::GetFilter() const
{
    return filter;
}

inline
void
FileBrowser::SetFilter(
    QStringView FileType,
    const QStringList& FileExt)
{
    if(FileExt.empty()) { return;}

    for(QStringList::const_iterator it = FileExt.constBegin(); FileExt.end() != it; ++it)
    {
        if(!filterValidator.match(*it).hasMatch()) { return;}
    }

    QString resFilter;
    resFilter += FileType;
    resFilter += " (";
    resFilter += FileExt.join(", ");
    resFilter += ')';

    filter = std::move(resFilter);
}

void
FileBrowser::SetLabel(
    const QString& BrowserLabel)
{
    if(BrowserLabel.isEmpty()) { return;}
    label->setText(BrowserLabel);
}


inline
void
FileBrowser::SetMode(
    Mode NewMode) 
{
    currentMode = NewMode;
    switch(currentMode)
    {
        case Mode::dir:
        {
            label->setText("Путь до каталога:");
            break;
        }
        case Mode::fSave:
        case Mode::fLoad:
        {
            label->setText("Путь до файла:");
            break;
        }
        default: break;
    }
}

void
FileBrowser::SetPathHistory(
    const QStringList PathHistory)
{
    QRegularExpression& currRegex = (Mode::dir == currentMode) ? dirPathValidator : filePathValidator;

    for(QStringList::const_iterator iter = PathHistory.constBegin(); PathHistory.end() != iter; ++iter)
    {
        if(!currRegex.match(*iter).hasMatch())
        {
            return;
        }
    }
    pathCb->addItems(PathHistory);
}


void
FileBrowser::SetPath(
    const QString& Path)
{
    QRegularExpression& currRegex = (Mode::dir == currentMode) ? dirPathValidator : filePathValidator;
    if(!currRegex.match(Path).hasMatch()) { return;}
    pathCb->addItem(Path);
    pathCb->setCurrentText(Path);
}


void
FileBrowser::OnBrowse()
{
    QFileDialog fDialog;
    QString resPath;

    fDialog.setNameFilter(filter);
    fDialog.setViewMode(QFileDialog::Detail);
    
    switch (currentMode)
    {
        case Mode::dir:
        {
            fDialog.setFileMode(QFileDialog::FileMode::Directory);
            fDialog.setOption(QFileDialog::Option::ShowDirsOnly);
        }
        case Mode::fLoad:
        {
            fDialog.setFileMode(QFileDialog::FileMode::ExistingFile);
            fDialog.setOption(QFileDialog::Option::ReadOnly);
        }
        case Mode::fSave:
        {
            fDialog.setFileMode(QFileDialog::FileMode::AnyFile);
        }
    }

    connect(&fDialog, &QFileDialog::fileSelected, [&resPath](const QString& SelectedPath) { resPath = SelectedPath;});

    if(fDialog.exec())
    {
        pathCb->addItem(resPath);
    }
}