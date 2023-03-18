/**
 * @file FileBrowser.hpp
 * @author Mikhail Kotlyarov  (m.kotlyarov@elvis.ru)
 * @brief File browser widget class definition
 * @version 0.1
 * @date 2023-03-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __FILE_BROWSER_H__
#define __FILE_BROWSER_H__

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QStringList>
#include <QRegularExpression>

/**
 * @class FileBrowser
 * @brief Widget for file or dir browsing
 */
class FileBrowser: public QWidget
{
    private:

    public:

    /*! enum for workmode*/
    enum class Mode
    {
        fSave, ///< Save file mode
        fLoad, ///< Load file mode
        dir    ///< 
    };

    /**
     * @brief Construct a new File Browser object
     * 
     * @param BrowserMode Work mode
     * @param Parent Parent widget ptr
     */
    explicit
    FileBrowser(
        Mode BrowserMode,
        QWidget* Parent = nullptr);

    /**
     * @brief Get the Path
     * 
     * @return QString 
     */
    QString
    GetPath() const;

    /**
     * @brief Get workmode
     * 
     * @return Mode 
     */
    Mode
    GetMode() const;

    /**
     * @brief Get t
     * 
     * @return const QString& 
     */
    const QString&
    GetFilter() const;


    /**
     * @brief Set workmode
     * 
     * @param NewMode 
     */
    void
    SetMode(
        Mode NewMode);

    /**
     * @brief Set filter
     * 
     * @param FileType Type (ex. 'Image')
     * @param FileExt  List of ext (ex. {"*.img", "*.txt"})
     */
    void
    SetFilter(
        QStringView FileType,
        const QStringList& FileExt);

    void
    SetLabel(
        const QString& BrowserLabel);

    void
    SetPath(
        const QString& Path);

    signals:

    void
    PathChanged(
        const QString&);

    private slots:

    /**
     * @brief Button browse action
     * 
     */
    void OnBrowse();


    private:
    Mode         currentMode;
    QString      filter;
    QRegularExpression filterValidator;

    QLabel*      label;
    QComboBox*   pathCb;
    QPushButton* browseBtn;
};


#endif //!__FILE_BROWSER_H__