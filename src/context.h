#ifndef CONTEXT_H
#define CONTEXT_H

#include <QObject>
#include <QDebug>
#include <QScreen>
#include <QPoint>
#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QSettings>
#include <QIcon>
#include <QStringList>
#include <QString>
#include <QProcess>
#include <QWindow>
#include <QColor>
#include <QRgb>
#include <QDir>
#include <QDrag>
#include <QMimeData>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QLocale>
#include <QList>
#include <QLibrary>
#include <QFunctionPointer>
#include <QMatrix>
#include <QSize>
#include <QSettings>
#include <QBuffer>
//#include <QCryptographicHash>
//#include <QTextStream>
//#include <QByteArray>

#include "xlibutil.h"


class Context : public QObject, public Xlibutil
{

    Q_OBJECT
    Q_PROPERTY(QString basepath READ basePath NOTIFY basepathChanged)


public:
    Context();
    Q_INVOKABLE QStringList addLauncher(QString app);
    Q_INVOKABLE int mouseX();
    Q_INVOKABLE int mouseY();
    Q_INVOKABLE void exec(QString pro);
    Q_INVOKABLE void execFromPid(QString pid);
    Q_INVOKABLE void killFromPid(QString pid);
    Q_INVOKABLE void minimize(int win);
    Q_INVOKABLE void active(int pid);
    Q_INVOKABLE void manyMinimizes(QString pidname);
    Q_INVOKABLE void minimizes(int win);
    Q_INVOKABLE int isMinimized(QString wmclass);
    Q_INVOKABLE void manyActives(QString pidname);
    Q_INVOKABLE void actives(int pid);
    Q_INVOKABLE void xActive(int id);
    //Q_INVOKABLE void changeThemeColor(QString rgb);
    Q_INVOKABLE int isActive(QString pidname);
    Q_INVOKABLE QStringList applications();
    Q_INVOKABLE void dragDrop(QString icone, QString app);
    Q_INVOKABLE void addDesktopFile(Window win, QString desktopFile);
    Q_INVOKABLE QString defaultIcon();
    Q_INVOKABLE QString userName();
    Q_INVOKABLE QString windowName(int window);
    Q_INVOKABLE void windowActive(int window);
    Q_INVOKABLE void windowClose(int window);
    Q_INVOKABLE QList<int> windowsBywmclass(QString wmclass);
    Q_INVOKABLE void showMoreWindows(int winId, int h);
    //Q_INVOKABLE void libraryVoidLoad(int arg, QString args, QString funcName, QString pluginName);
    //Q_INVOKABLE int libraryIntLoad(int arg, QString args, QString funcName, QString pluginName);
    //Q_INVOKABLE QString libraryQStringLoad(int arg, QString args, QString funcName, QString pluginName);
    //Q_INVOKABLE void libraryVoidLoad(QString funcName, QString pluginName);
    //Q_INVOKABLE int libraryIntLoad(QString funcName, QString pluginName);
    //Q_INVOKABLE QString libraryQStringLoad(QString funcName, QString pluginName);
    Q_INVOKABLE QStringList plugins();
    Q_INVOKABLE QString windowFocused();
    Q_INVOKABLE int windowFocusedId(QString wmclass);
    Q_INVOKABLE void execute(QString app, QString wclass);
    Q_INVOKABLE void windowMap(int win);
    Q_INVOKABLE QStringList getAllFixedLaunchers();
    Q_INVOKABLE void fixedLauncher(QString name, QString launchers, int remove);
    Q_INVOKABLE QString getAllWindows();
    Q_INVOKABLE QString crop(QString img);
    Q_INVOKABLE QString imagePerfil();
    Q_INVOKABLE QString fontName();
    Q_INVOKABLE void gtkThemeChangeDetail(QString color);
    Q_INVOKABLE int modified();
    Q_INVOKABLE QString getTheme();
    Q_INVOKABLE void setTheme(QString theme);
    Q_INVOKABLE QString getBrightness();
    Q_INVOKABLE void setTransparent(int arg);
    Q_INVOKABLE int getTransparent();
    Q_INVOKABLE QString blurEffect(QVariant obj, int screeshot = 1);
    Q_INVOKABLE void windowMove(QVariant obj, int x, int y, int w, int h);
    QPixmap getIconByClass(QString winId, QString wmclass);
    QString defaultIconApplications;
    QString basepath;
    int execWindowPid;
    QString desktopFile;

protected:
    Window windowId(int pid);

signals:
    void basepathChanged();

private:
    //void changeColorBtnMin(QRgb color);
    //void changeColorBtnMax(QRgb color);
    //void changeColorBtnClose(QRgb color);
    void changeColor(int w, int h, QRgb color, QString s);
    void changeColorBtnMenu(QRgb color);
    QImage imageOverlay(const QImage& baseImage, const QImage& overlayImage);
    QString launcherFix(QString pro);
    QScreen *screen = QApplication::screens().at(0);
    Display *display;
    QString defaultIconTheme;
    QString defaultPrefixTheme;
    QStringList prefix;
    QString absoluteIconPath;
    QString gtkTheme;
    QString homePath;
    bool noDisplay;
    bool mixHicolor;
    int transparent;
    QString font;
    QString theme;
    QString brightness;

    QString basePath() const {
        return this->basepath;
    }
};

#endif // CONTEXT_H
