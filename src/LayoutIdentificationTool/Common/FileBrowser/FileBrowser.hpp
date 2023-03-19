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
    Q_OBJECT
    public:
    signals:

    void
    PathChanged(
        const QString& NewPath);

    public:

    /*! enum for workmode*/
    enum class Mode
    {
        fSave, ///< Save file mode
        fLoad, ///< Load file mode
        dir    ///< Dir mode
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
     * @brief Get filter
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

    /**
     * @brief Set label of browser
     * 
     * @param BrowserLabel 
     */
    void
    SetLabel(
        const QString& BrowserLabel);

    /**
     * @brief Set current path
     * 
     * @param Path 
     */
    void
    SetPath(
        const QString& Path);

    /**
     * @brief Set path history
     * 
     * @param PathHistory 
     */
    void
    SetPathHistory(
        const QStringList PathHistory);




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
    QRegularExpression dirPathValidator;
    QRegularExpression filePathValidator;

    QLabel*      label;
    QComboBox*   pathCb;
    QPushButton* browseBtn;
};


#endif //!__FILE_BROWSER_H__