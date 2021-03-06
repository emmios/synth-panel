#include "context.h"



Context::Context()
{
    //QProcess process;
    //process.start("xfconf-query -c xsettings -p /Net/IconThemeName");
    //process.waitForFinished();
    //MacBuntu-OS
    //this->defaultIconTheme = process.readLine().replace("\n", "");

    this->homePath = QDir::homePath();

    //QSettings gtkThemePath(this->homePath + "/.config/gtk-3.0/settings.ini", QSettings::IniFormat);
    //gtkThemePath.beginGroup("Settings");
    //this->gtkTheme = gtkThemePath.value("gtk-theme-name").toString();
    //gtkThemePath.endGroup();

    QString path = this->homePath + "/.config/synth/panel/";
    QSettings settings(path + "settings.conf", QSettings::NativeFormat);
    this->defaultIconTheme = settings.value("iconTheme").toString();
    this->noDisplay = settings.value("showNoIcon").toBool();
    this->mixHicolor = settings.value("mixHicolor").toBool();
    this->font = settings.value("font").toString();
    this->theme = settings.value("theme").toString();
    this->brightness = settings.value("brightness").toString();
    this->transparent = settings.value("transparent").toInt();

    if (this->defaultIconTheme.isEmpty()) this->defaultIconTheme = "hicolor";

    QStringList localPrefix;
    localPrefix << "/usr/share/icons/";
    localPrefix << this->homePath + "/.local/share/icons/";
    localPrefix << this->homePath + "/.icons/";

    for (QString local : localPrefix)
    {
        QFileInfo info(local + this->defaultIconTheme + "/index.theme");

        if (info.exists())
        {
            this->absoluteIconPath = local;
            break;
        }
    }

    this->prefix << "/apps/48/";
    this->prefix << "/mimes/48/";
    this->prefix << "/mimetypes/48/";
    this->prefix << "/places/48/";
    this->prefix << "/status/48/";
    this->prefix << "/devices/48/";
    this->prefix << "/web/48/";
    this->prefix << "/emblems/48/";
    this->prefix << "/categories/48/";
    this->prefix << "/actions/48/";

    this->prefix << "/apps/scalable/";
    this->prefix << "/scalable/apps/";
    this->prefix << "/status/scalable/";
    this->prefix << "/mimetypes/scalable/";
    this->prefix << "/mimes/scalable/";
    this->prefix << "/devices/scalable/";
    this->prefix << "/places/scalable/";
    this->prefix << "/web/scalable/";
    this->prefix << "/emblems/scalable/";
    this->prefix << "/categories/scalable/";
    this->prefix << "/actions/scalable/";

    this->prefix << "/48x48/apps/";
    this->prefix << "/48x48/scalable/";
    this->prefix << "/48x48/status/";
    this->prefix << "/48x48/mimes/";
    this->prefix << "/48x48/mimetypes/";
    this->prefix << "/48x48/devices/";
    this->prefix << "/48x48/places/";
    this->prefix << "/48x48/web/";
    this->prefix << "/48x48/emblems/";
    this->prefix << "/48x48/categories/";
    this->prefix << "/48x48/actions/";

    this->defaultIconApplications = this->defaultIcon();
}

void Context::windowMove(QVariant obj, int x, int y, int w, int h)
{
    if (!obj.isNull())
    {
        QObject *_obj = obj.value<QObject *>();
        if (_obj)
        {
            QWindow *win = qobject_cast<QWindow *>(_obj);
            this->xwindowMove(win->winId(), x, y, w, h);
        }
    }
}

QString Context::blurEffect(QVariant obj, int screeshot)
{
    Window desktop = -1;

    if (screeshot == 1)
    {
        unsigned long items;
        Window *list = this->xwindows(&items);
        for (int i = 0; i < items; i++)
        {
            QString type = this->xwindowType(list[i]);
            if (type == "_NET_WM_WINDOW_TYPE_DESKTOP")
            {
                desktop = list[i];
                break;
            }
        }
    }
    else
    {
        desktop = 0;
    }

    if (desktop != -1)
    {
        QPixmap map;
        QScreen *screen = QGuiApplication::primaryScreen();
        if (!obj.isNull())
        {
            QObject *_obj = obj.value<QObject *>();
            if (_obj)
            {
                QWindow *win = qobject_cast<QWindow *>(_obj);
                map = screen->grabWindow(desktop, win->x(), win->y(), win->width(), win->height());
            }
        }
        if (!map.isNull())
        {
            QBuffer buffer;
            buffer.open(QIODevice::ReadWrite);
            map.save(&buffer, "jpg");
            const QByteArray bytes = buffer.buffer();
            buffer.close();
            QString base64("data:image/jpg;base64,");
            base64.append(QString::fromLatin1(bytes.toBase64().data()));
            return base64;
        }
    }
    return "";
}

void Context::setTransparent(int arg)
{
    QSettings settings(this->homePath + "/.config/synth/panel/settings.conf", QSettings::NativeFormat);
    settings.setValue("transparent", arg);
}

int Context::getTransparent()
{
    QSettings settings(this->homePath + "/.config/synth/panel/settings.conf", QSettings::NativeFormat);
    int tr = settings.value("transparent").toInt();
    return tr;
}

void Context::setTheme(QString theme)
{
    QSettings settings(this->homePath + "/.config/synth/panel/settings.conf", QSettings::NativeFormat);
    settings.setValue("theme", theme);
}

QString Context::fontName()
{
    return this->font;
}

QString Context::getTheme()
{
    return this->theme;
}

QString Context::getBrightness()
{
    return this->brightness;
}

/*
int Context::libraryIntLoad(int arg, QString args, QString funcName, QString pluginName)
{
    QLibrary library(this->basepath + "/plugins/" + pluginName);
    int num;

    if (library.load())
    {
        typedef int (*func)(int i, QString s);
        func f = (func)library.resolve(funcName.toUtf8());

        if (f);
            num = f(arg, args);
    }

    return num;
}

int Context::libraryIntLoad(QString funcName, QString pluginName)
{
    QLibrary library(this->basepath + "/plugins/" + pluginName);
    int num;

    if (library.load())
    {
        typedef int (*func)();
        func f = (func)library.resolve(funcName.toUtf8());

        if (f);
            num = f();
    }

    return num;
}

QString Context::libraryQStringLoad(int arg, QString args, QString funcName, QString pluginName)
{
    QLibrary library(this->basepath + "/plugins/" + pluginName);
    QString text;

    if (library.load())
    {
        typedef QString (*func)(int i, QString s);
        func f = (func)library.resolve(funcName.toUtf8());

        if (f);
            text = f(arg, args);
    }

    return text;
}

QString Context::libraryQStringLoad(QString funcName, QString pluginName)
{
    QLibrary library(this->basepath + "/plugins/" + pluginName);
    QString text;

    if (library.load())
    {
        typedef QString (*func)();
        func f = (func)library.resolve(funcName.toUtf8());

        if (f);
            text = f();
    }

    return text;
}

void Context::libraryVoidLoad(int arg, QString args, QString funcName, QString pluginName)
{
    QLibrary library(this->basepath + "/plugins/" + pluginName);

    if (library.load())
    {
        typedef void (*func)(int i, QString s);
        func f = (func)library.resolve(funcName.toUtf8());

        if (f);
            f(arg, args);
    }
}

void Context::libraryVoidLoad(QString funcName, QString pluginName)
{
    QLibrary library(this->basepath + "/plugins/" + pluginName);

    if (library.load())
    {
        QFunctionPointer func = library.resolve(funcName.toUtf8());

        if (func);
            func();
    }
}
*/

QImage Context::imageOverlay(const QImage& baseImage, const QImage& overlayImage)
{
    QImage imageWithOverlay = QImage(baseImage.size(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&imageWithOverlay);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(imageWithOverlay.rect(), Qt::transparent);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, baseImage);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, overlayImage);

    painter.end();

    return imageWithOverlay;
}

/*
void Context::changeColorBtnMenu(QRgb color)
{
    QImage img(QDir::homePath() + "/.themes/dinamic-color/xfwm4/novos/menu-active2.xpm", "xpm");
    img.setColor(0, color);
    img.save(QDir::homePath() + "/.themes/dinamic-color/xfwm4/menu-active.xpm", "xpm");
    img.save(QDir::homePath() + "/.themes/dinamic-color/xfwm4/menu-inactive.xpm", "xpm");
}

void Context::changeColorBtnMin(QRgb color)
{
    QImage bg(QDir::homePath() + "/.themes/dinamic-color/xfwm4/novos/Min-transparent.xpm", "xpm");
    bg.setColor(0, color);

    QImage overlay(QDir::homePath() + "/.themes/dinamic-color/xfwm4/novos/Min.png", "png");
    QImage img = this->imageOverlay(bg, overlay);

    img.save(QDir::homePath() + "/.themes/dinamic-color/xfwm4/hide-active.xpm", "xpm");
    img.save(QDir::homePath() + "/.themes/dinamic-color/xfwm4/hide-inactive.xpm", "xpm");

    QColor cor;
    int r, g, b;
    cor = bg.color(0);

    r = cor.red();
    g = cor.green();
    b = cor.blue();

    bool red, green, blue;

    if (cor.red() > cor.green() && cor.red() > cor.blue()) red = true;
    if (cor.green() > cor.red() && cor.green() > cor.blue()) green = true;
    if (cor.blue() > cor.red() && cor.blue() > cor.green()) blue = true;

    if (cor.red() > 30 && cor.red() < 225 && red) r += 30;
    if (cor.green() > 30 && cor.green() < 225 && green) g += 30;
    if (cor.blue() > 30 && cor.blue() < 225 && blue) b += 30;

    bg.setColor(0, qRgb(r, g, b));
    img = this->imageOverlay(bg, overlay);

    img.save(QDir::homePath() + "/.themes/dinamic-color/xfwm4/hide-prelight.xpm", "xpm");
    img.save(QDir::homePath() + "/.themes/dinamic-color/xfwm4/hide-pressed.xpm", "xpm");
}

void Context::changeColorBtnMax(QRgb color)
{
    QImage bg(QDir::homePath() + "/.themes/dinamic-color/xfwm4/novos/Max-transparent.xpm", "xpm");
    bg.setColor(0, color);

    QImage overlay(QDir::homePath() + "/.themes/dinamic-color/xfwm4/novos/Max.png", "png");
    QImage img = this->imageOverlay(bg, overlay);

    img.save(QDir::homePath() + "/.themes/dinamic-color/xfwm4/maximize-active.xpm", "xpm");
    img.save(QDir::homePath() + "/.themes/dinamic-color/xfwm4/maximize-inactive.xpm", "xpm");

    QColor cor;
    int r, g, b;
    cor = bg.color(0);

    r = cor.red();
    g = cor.green();
    b = cor.blue();

    bool red, green, blue;

    if (cor.red() > cor.green() && cor.red() > cor.blue()) red = true;
    if (cor.green() > cor.red() && cor.green() > cor.blue()) green = true;
    if (cor.blue() > cor.red() && cor.blue() > cor.green()) blue = true;

    if (cor.red() > 30 && cor.red() < 225 && red) r += 30;
    if (cor.green() > 30 && cor.green() < 225 && green) g += 30;
    if (cor.blue() > 30 && cor.blue() < 225 && blue) b += 30;

    bg.setColor(0, qRgb(r, g, b));
    img = this->imageOverlay(bg, overlay);

    img.save(QDir::homePath() + "/.themes/dinamic-color/xfwm4/maximize-prelight.xpm", "xpm");
    img.save(QDir::homePath() + "/.themes/dinamic-color/xfwm4/maximize-pressed.xpm", "xpm");
    img.save(QDir::homePath() + "/.themes/dinamic-color/xfwm4/maximize-toggled-prelight.xpm", "xpm");
    img.save(QDir::homePath() + "/.themes/dinamic-color/xfwm4/maximize-toggled-pressed.xpm", "xpm");
}

void Context::changeColorBtnClose(QRgb color)
{
    QImage bg(QDir::homePath() + "/.themes/dinamic-color/xfwm4/novos/Close-transparent.xpm", "xpm");
    bg.setColor(0, color);

    QImage overlay(QDir::homePath() + "/.themes/dinamic-color/xfwm4/novos/Close.png", "png");
    QImage img = this->imageOverlay(bg, overlay);

    img.save(QDir::homePath() + "/.themes/dinamic-color/xfwm4/close-active.xpm", "xpm");
    img.save(QDir::homePath() + "/.themes/dinamic-color/xfwm4/close-inactive.xpm", "xpm");

    QColor cor;
    int r, g, b;
    cor = bg.color(0);

    r = cor.red();
    g = cor.green();
    b = cor.blue();

    bool red, green, blue;

    if (cor.red() > cor.green() && cor.red() > cor.blue()) red = true;
    if (cor.green() > cor.red() && cor.green() > cor.blue()) green = true;
    if (cor.blue() > cor.red() && cor.blue() > cor.green()) blue = true;

    if (cor.red() > 30 && cor.red() < 225 && red) r += 30;
    if (cor.green() > 30 && cor.green() < 225 && green) g += 30;
    if (cor.blue() > 30 && cor.blue() < 225 && blue) b += 30;

    bg.setColor(0, qRgb(r, g, b));
    img = this->imageOverlay(bg, overlay);

    img.save(QDir::homePath() + "/.themes/dinamic-color/xfwm4/close-prelight.xpm", "xpm");
    img.save(QDir::homePath() + "/.themes/dinamic-color/xfwm4/close-pressed.xpm", "xpm");
}
*/

void Context::showMoreWindows(int winId, int h)
{
    this->xreservedSpace(winId, h);
}

void Context::changeColor(int w, int h, QRgb color, QString s)
{
    QImage img(w, h, QImage::Format_RGB888);

    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {
            img.setPixel(x, y, color);
        }
    }

    img.save(s, "xpm");
}

/*
void Context::changeThemeColor(QString rgb)
{
    QColor cor(rgb);
    QRgb color = qRgb(cor.red(), cor.green(), cor.blue());

    this->changeColor(20, 24, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/title-1-active.xpm");
    this->changeColor(20, 24, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/title-2-active.xpm");
    this->changeColor(20, 24, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/title-3-active.xpm");
    this->changeColor(20, 24, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/title-4-active.xpm");
    this->changeColor(20, 24, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/title-5-active.xpm");

    this->changeColor(20, 24, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/title-1-inactive.xpm");
    this->changeColor(20, 24, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/title-2-inactive.xpm");
    this->changeColor(20, 24, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/title-3-inactive.xpm");
    this->changeColor(20, 24, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/title-4-inactive.xpm");
    this->changeColor(20, 24, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/title-5-inactive.xpm");

    this->changeColor(2, 25, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/top-left-active.xpm");
    this->changeColor(2, 25, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/top-left-inactive.xpm");
    this->changeColor(2, 25, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/top-right-active.xpm");
    this->changeColor(2, 25, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/top-right-inactive.xpm");

    this->changeColor(4, 16, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/left-active.xpm");
    this->changeColor(4, 16, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/left-inactive.xpm");
    this->changeColor(4, 16, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/right-active.xpm");
    this->changeColor(4, 16, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/right-inactive.xpm");

    this->changeColor(3, 4, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/bottom-active.xpm");
    this->changeColor(3, 4, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/bottom-inactive.xpm");

    this->changeColor(2, 2, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/bottom-right-inactive.xpm");
    this->changeColor(2, 2, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/bottom-right-active.xpm");
    this->changeColor(2, 2, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/bottom-left-inactive.xpm");
    this->changeColor(2, 2, color, QDir::homePath() + "/.themes/dinamic-color/xfwm4/bottom-left-active.xpm");

    this->changeColorBtnClose(color);
    this->changeColorBtnMax(color);
    this->changeColorBtnMin(color);
    this->changeColorBtnMenu(color);

    QProcess process;
    process.start("xfconf-query -c xfwm4 -p /general/theme");
    process.waitForFinished();
    QString theme = process.readLine().replace("\n", "");
    process.close();

    if (theme == "dinamic-color")
    {
        process.start("xfconf-query -c xfwm4 -p /general/theme -s tmp");
        process.waitForFinished();
        process.close();

        process.start("xfconf-query -c xfwm4 -p /general/theme -s dinamic-color");
        process.waitForFinished();
        process.close();
    }
}
*/

QString Context::launcherFix(QString exec)
{
    QStringList list;
    list << "google-chrome-stable" << "google-chrome";
    list << "gimp-2.8" << "gimp";
    list << "gimp-2.9" << "gimp";
    list << "gimp-2.10" << "gimp";
    //list << "TelegramDesktop" << "Telegram";

    for(int i = 1; i <= list.length(); i++)
    {
        if (list.at(i - 1) == exec && i % 2 == 1)
        {
            return list.at(i);
        }
    }

    return exec;
}

void Context::killFromPid(QString pid)
{
    QProcess *process = new QProcess();
    process->start("kill " + pid);
}

void Context::execFromPid(QString pid)
{
    QProcess *process = new QProcess();
    process->start("ps -o cmd "+ pid);
    process->waitForReadyRead();
    QString command = QString(process->readAll()).split("\n").at(1);
    process->close();

    process->start(command);
}


void Context::exec(QString pro)
{
    QString list;

    if (pro.contains("%"))
    {
        list = pro.split(" %")[0];
    }
    else
    {
        list = pro;
    }

    QProcess *process = new QProcess();
    process->start(list);
    //process->waitForReadyRead();
    //this->execWindowPid = process->pid();
    //this->desktopFile = app;
//    Window id = this->windowId(process->pid());
//    this->addDesktopFile(id, app);
}

void Context::execute(QString app, QString wclass)
{
    QSettings settings(app);
    settings.beginGroup("Desktop Entry");

    if (app.contains("%"))
    {
        app = app.split(" %")[0];
    }
    else
    {
        app = app;
    }

    settings.endGroup();

    QProcess *process = new QProcess();
    if (app.isEmpty())
    {
        process->start(wclass);
    }
    else
    {
        process->start(app);
    }
}

void Context::windowMap(int win)
{
    Display *d = XOpenDisplay(0);
    XMapSubwindows(d, (Window)win);
    XResizeWindow(d, (Window)win, 1000, 600);
    XMoveResizeWindow(d, (Window)win, 30, 30, 1000, 600);
    XFlush(d);
    XCloseDisplay(d);
    //this->xwindowMap(win);
    //this->xactive(win);
}

void Context::addDesktopFile(Window win, QString desktopFile)
{
    if (!desktopFile.isEmpty()) {
        this->xaddDesktopFile(win, desktopFile.remove("file://"));
    }
}

QList<int> Context::windowsBywmclass(QString wmclass)
{
    //Display *display = QX11Info::display();
    Display *display = XOpenDisplay(0);
    QList<int> list;

    if (display)
    {
        Atom atom = XInternAtom(display, "_NET_CLIENT_LIST", True);
        Atom actual_type;
        int actual_format;
        unsigned long nitems;
        unsigned long bytes_after;
        unsigned char *prop;

        int status = XGetWindowProperty(display, DefaultRootWindow(display), atom, 0, LONG_MAX, False, AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);

        Window *lists = (Window *)((unsigned long *)prop);

        for (int i = 0; i < nitems; i++)
        {
            if (wmclass == QString(this->xwindowClass(lists[i])).toLower())
            {
                list.append(lists[i]);
            }
        }

        XFree(prop);
    }

    XFlush(display);
    XCloseDisplay(display);

    return list;
}

QString Context::windowFocused()
{
    return this->xgetWindowFocused();
}

int Context::windowFocusedId(QString wmclass)
{
    //return this->xgetWindowFocusedId();

    Display *display = XOpenDisplay(0);

    if (display)
    {
        Atom atom = XInternAtom(display, "_NET_CLIENT_LIST", True);
        Atom actual_type;
        int actual_format;
        unsigned long nitems;
        unsigned long bytes_after;
        unsigned char *prop;

        //4096 / 4
        int status = XGetWindowProperty(display, DefaultRootWindow(display), atom, 0, LONG_MAX, False, AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);

        Window *lists = (Window *)((unsigned long *)prop);

        for (int i = 0; i < nitems; i++)
        {
            if (this->xisActive(wmclass))
            {
                XFree(prop);
                XFlush(display);
                XCloseDisplay(display);
                return 1;
            }
        }

        XFree(prop);
    }

    XFlush(display);
    XCloseDisplay(display);

    return 0;
}

QString Context::windowName(int window)
{
    return QString(this->xwindowName(window));
}

void Context::windowActive(int window)
{
    this->xactive(window);
}

void Context::windowClose(int window)
{
    this->xwindowClose(window);
}

QString Context::defaultIcon()
{
    QStringList defaultIcons;
    defaultIcons << "application-default-icon.png";
    defaultIcons << "application-default-icon.svg";
    defaultIcons << "default-application.png";
    defaultIcons << "default-application.svg";

    for (QString icon : defaultIcons)
    {
        QString iconPath;
        for (QString prefix : this->prefix)
        {
            iconPath = this->absoluteIconPath + this->defaultIconTheme + prefix + icon;
            QFile info(iconPath);

            if (info.exists())
            {
                return iconPath;
            }
        }
    }

    return "/usr/share/synth/synth-panel/default.svg";
}

QString Context::getAllWindows()
{
    QString arrayCreate;
    unsigned long _items;
    Window *list = this->xwindows(&_items);
    Display* d = XOpenDisplay(0);

    for (int i = 0; i < _items; i++)
    {
        QString type = QString(this->xwindowType(list[i]));
        if (type == "_NET_WM_WINDOW_TYPE_NORMAL" || type == "_KDE_NET_WM_WINDOW_TYPE_OVERRIDE")
        {
            int status;
            unsigned long nitems;
            QString name = this->xwindowName(list[i]);
            QString wclass = QString(this->xwindowClass(list[i])).toLower();

            if (wclass != "unknow")
            {
                if (wclass != "Neon_Panel")
                {
                    arrayCreate += "|@|" + name + "=#=" + wclass + "=#=" + QString::number((int)list[i])  + "=#=" + QString::number((int)this->xwindowPid(list[i]))  + '=#=' + QString((char *)this->windowProperty(d, list[i], "_OB_APP_CLASS", &nitems, &status));
                }
            }
        }
    }

    //XFree(d);
    XCloseDisplay(d);

    return arrayCreate;
}

QStringList Context::getAllFixedLaunchers()
{
    QString path = this->homePath + "/.config/synth/panel/launchers.conf";
    QSettings settings(path, QSettings::NativeFormat);
    QStringList launchers;

    for (QString key : settings.allKeys())
    {
        launchers.append(settings.value(key).toString());
    }

    return launchers;
}

void Context::fixedLauncher(QString name, QString launchers, int remove)
{
    QString path = this->homePath + "/.config/synth/panel/launchers.conf";
    QSettings settings(path, QSettings::NativeFormat);
    if (remove == 0)
    {
        settings.setValue(name, launchers);
    }
    else
    {
        settings.remove(name);
    }
}

QStringList Context::addLauncher(QString app)
{
    QStringList list, nitems;
    app = app.replace("file://", "");
    QFileInfo f(app);

    if (f.suffix() == "desktop")
    {
        QString nome, icone, tmp, exec, wmclass, hasIcon;
        QStringList categories;
        bool noDisplay = false;
        hasIcon = "true";
        QLocale locale;
        QSettings settings(app, QSettings::NativeFormat);
        settings.setIniCodec("UTF-8");
        settings.beginGroup("Desktop Entry");

        nome = settings.value("Name[" + locale.name() + "]").toString();
        if (nome.isEmpty()) nome = settings.value("Name").toString();

        tmp = settings.value("Icon").toString();
        exec = settings.value("Exec").toString();
        noDisplay = settings.value("NoDisplay").toBool();
        categories = settings.value("Categories").toStringList();
        wmclass = settings.value("StartupWMClass").toString();
        settings.endGroup();

        if (noDisplay)
        {
            list << "" << "" << "" << "" << "false";
            return list;
        }

        if (wmclass.isEmpty())
        {
            if (exec.contains(" "))
            {
                wmclass = exec.split(" ")[0];
            }
            else
            {
                wmclass = exec;
            }

            if (wmclass.contains("/"))
            {
                nitems = wmclass.split("/");
                wmclass = nitems[nitems.length() - 1];
            }

            wmclass = this->launcherFix(wmclass);
        }


        if (!tmp.startsWith("/"))
        {
            QStringList exts;
            exts << ".svg" << ".png" << ".xpm";

            for (QString ext : exts)
            {
                bool stop = false;

                for (QString prefix : this->prefix)
                {
                    icone = this->absoluteIconPath + this->defaultIconTheme + prefix + tmp + ext;
                    f.setFile(icone);

                    if (f.exists())
                    {
                        stop = true;
                        break;
                    }
                    else
                    {
                         icone = "";
                    }
                }

                if (stop) break;
            }

            if (icone.isEmpty())
            {
                QStringList finalPaths;

                if (this->mixHicolor)
                {
                    finalPaths << this->homePath + ".local/share/icons/hicolor/48x48/apps/";
                    finalPaths << "/usr/share/icons/hicolor/48x48/apps/";
                }

                finalPaths << this->homePath + ".local/share/icons/";
                finalPaths << "/usr/share/pixmaps/";

                for (QString path : finalPaths)
                {
                    bool stop = false;

                    for (QString ext : exts)
                    {
                        icone = path + tmp + ext;
                        f.setFile(icone);

                        if (f.exists())
                        {
                            stop = true;
                            break;
                         }
                         else
                         {
                            icone = "";
                         }
                    }

                    if (stop) break;
                }
            }

            if (icone.isEmpty())
            {
                icone = this->defaultIconApplications;
                hasIcon = "false";
            }

            list << nome << "file://" + icone << exec << wmclass.toLower() << hasIcon;
        }
        else
        {
            if (tmp.contains(".ico"))
            {
                tmp = this->defaultIconApplications;
                hasIcon = "false";
            }
            else if (tmp == "/")
            {
                tmp = this->defaultIconApplications;
                hasIcon = "false";
            }

            list << nome << "file://" + tmp << exec << wmclass.toLower() << hasIcon;
        }
    }
    else
    {
        list << "" << "" << "" << "" << "false";
    }

    return list;
}

QPixmap Context::getIconByClass(QString winId, QString wmclass)
{
    QString path;
    QStringList exts;

    exts << ".svg" << ".png" << ".xpm";

    path = this->xwindowLauncher(winId.toInt());

    if (path.isEmpty())
    {
        for (QString p : wmclass.split(","))
        {
            path = "/usr/share/applications/" + p + ".desktop";
            QFileInfo info(path);
            if (info.exists()) break;
        }
    }

    QPixmap pixel;
    QString icon, tmp;
    QSettings settings(path, QSettings::NativeFormat);
    settings.beginGroup("Desktop Entry");
    icon = settings.value("Icon").toString();
    settings.endGroup();

    icon = this->launcherFix(icon);

    if (!icon.startsWith("/"))
    {
        for (QString ext : exts)
        {
            bool stop = false;

            for (QString prefix : this->prefix)
            {
                tmp = this->absoluteIconPath + this->defaultIconTheme + '/' + prefix + '/' + icon + ext;
                QFileInfo info(tmp);

                if (info.exists())
                {
                    icon = tmp;
                    stop = true;
                    break;
                }
            }

            if (stop) break;
        }

        if (icon.isEmpty())
        {
            foreach (QString ext, exts)
            {
                icon = "/usr/share/icons/hicolor/48x48/apps/" + icon + ext;
                QFileInfo info(icon);
                if (info.exists()) break;
            }
        }

        if (icon.isEmpty())
        {
            foreach (QString ext, exts)
            {
                icon = "/usr/share/pixmaps/" + icon + ext;
                QFileInfo info(icon);
                if (info.exists()) break;
            }
        }
    }

    pixel.load(icon);
    pixel = pixel.scaled(24, 24, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    return pixel;
}


Window Context::windowId(int pid)
{
    int pro_id;
    Window winId;
    int status;
    unsigned long *desktop;
    unsigned long size;

    Window *client_list = this->xwindows(&size);

    if (client_list != NULL)
    {
        for (int i = 0; i < size; i++)
        {
            Window tmpId = client_list[i];
            pro_id = this->xwindowPid(tmpId);

            if (pro_id == pid)
            {
                winId = tmpId;
                break;
            }
        }
    }

    XFree(client_list);
    return winId;
}

int Context::isMinimized(QString wmclass)
{

        unsigned long nitems;

        Window *lists = this->xwindows(&nitems);

        for (int i = 0; i < nitems; i++)
        {
            if (wmclass == QString(this->xwindowClass(lists[i])))
            {
                if ((const char *)this->xwindowState(lists[i]) == "_NET_WM_STATE_HIDDEN")
                {
                    XFree(lists);
                    return 1;
                }
            }
        }

    XFree(lists);

    return 0;
}

void Context::manyMinimizes(QString pidname)
{
    this->xminimizeByClass(pidname);
}

void Context::minimizes(int pid)
{
    Window id = this->xwindowID(pid);

    if (id)
    {
        this->xminimizeByClass(QString(this->xwindowClass(id)));
    }
}

void Context::minimize(int pid)
{
    Window id = this->xwindowID(pid);

    if (id)
    {
        this->xminimize(id);
    }
}

void Context::manyActives(QString pidname)
{
    this->xactiveByClass(pidname);
}

void Context::actives(int pid)
{
    Window id = this->xwindowID(pid);

    if (id)
    {
        this->xactiveByClass(QString(this->xwindowClass(id)));
    }
}

void Context::active(int pid)
{
    Window id = this->xwindowID(pid);

    if (id)
    {
       this->xactive(id);
    }
}

void Context::xActive(int id)
{
    this->xactive(id);
}


int Context::isActive(QString pidname)
{
    return this->xisActive(pidname);
}

int Context::mouseX()
{
    QPoint Mouse = QCursor::pos(this->screen);
    return Mouse.x();
}

int Context::mouseY()
{
    QPoint Mouse = QCursor::pos(this->screen);
    return Mouse.y();
}

QStringList Context::plugins()
{
    QStringList list;

    QDir dir(this->basepath + "/plugins");
    QFileInfoList filelist = dir.entryInfoList(QDir::Dirs | QDir::NoDot | QDir::NoDotAndDotDot);

    for (int i = 0; i < filelist.length(); i++)
    {
        QLocale locale;
        QSettings settings(dir.path() + "/" + filelist.at(i).fileName() + "/config", QSettings::NativeFormat);
        settings.setIniCodec("UTF-8");
        settings.beginGroup("AcessSpeed");

        QString cname = settings.value("Name[" + locale.name() + "]").toString();
        if (cname.isEmpty()) cname = settings.value("Name").toString();

        if (!cname.isEmpty())
            list << cname + ";" + settings.value("Icon").toString() + ";" + settings.value("Qml").toString() + ";" + settings.value("Lib").toString()+ ";" + settings.value("MoreSettingsss").toString();
        settings.endGroup();
    }

    return list;
}

int Context::modified()
{
    int response = 0;
    int i = 0;
    QStringList _modified;
    _modified << "modifiedapp" << "modifiedlocal";
    QStringList appsPath;
    appsPath << "/usr/share/applications" << this->homePath + "/.local/share/applications";

    QSettings settings(this->homePath + "/.config/Synth/panel/settings.txt", QSettings::NativeFormat);

    for (QString path : appsPath)
    {
        QFileInfo info(path);
        QDateTime datetime = info.lastModified();
        //QString hash = QCryptographicHash::hash(datetime.toString().toUtf8(), QCryptographicHash::Md5).toHex();
        QString hash = datetime.toString();
        QString sHash = settings.value(_modified.at(i)).toString();

        if (sHash != hash)
        {
            response = 1;
            settings.setValue(_modified.at(i), hash);
        }

        i++;
    }

    return response;
}

QStringList Context::applications()
{
    QStringList list, tmp, appsPath;

    appsPath << "/usr/share/applications/" << this->homePath + "/.local/share/applications/";

    for (QString path : appsPath)
    {
        QDir dir(path);
        QFileInfoList filelist = dir.entryInfoList(QDir::Files);

        for (int i = 0; i < filelist.length(); i++)
        {
            tmp = this->addLauncher(filelist.at(i).filePath());
            bool exist = false;

            foreach (QString l, list)
            {
                if (tmp.at(0) == l.split(";")[0])
                {
                    exist = true;
                }
            }

            if (!exist)
            {
                if (!this->noDisplay)
                {
                    if (tmp.at(0) != "" && tmp.at(4) == "true")
                    {
                        list << tmp.at(0) + ";" + tmp.at(1) + ";" + tmp.at(2) + ";" + filelist.at(i).filePath();
                        exist = false;
                    }
                }
                else
                {
                    if (tmp.at(0) != "")
                    {
                        list << tmp.at(0) + ";" + tmp.at(1) + ";" + tmp.at(2) + ";" + filelist.at(i).filePath();
                        exist = false;
                    }
                }
            }
        }
    }

    return list;
}

void Context::dragDrop(QString icone, QString app)
{
    QMimeData *mimeData = new QMimeData;
    QList<QUrl> url;

    url.append(QUrl(app));
    mimeData->setUrls(url);

    QImage img(icone, icone.split(".").at(1).toUtf8());
    QImage image = img.scaled(48, 48, Qt::KeepAspectRatio);

    QPixmap pixel;
    pixel.convertFromImage(image);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixel);

    QPoint p;
    p.setX(pixel.width() / 2);
    p.setY(pixel.height() / 2);
    drag->setHotSpot(p);

    //drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
    drag->exec(drag->defaultAction());
}

QString Context::userName()
{
    return qgetenv("USER").toUpper();
}

QString Context::crop(QString img)
{
    QImage srcImg(img.split("file://")[1]);
    srcImg = srcImg.scaled(QSize(24, 24), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    srcImg.save("/tmp/launcher.png", "PNG");
    return "file:///tmp/launcher.png";
}

QString Context::imagePerfil()
{
    QString img = "/usr/share/synth/users/" + qgetenv("USER").toLower() + ".jpg";
    QFile file(img);

    if (!file.exists()) {
        return "file:///usr/share/synth/users/default.jpg";
    }

    return "file://" + img;
}

void Context::gtkThemeChangeDetail(QString color)
{
    QString pathTheme = this->homePath + "/.themes/" + this->gtkTheme;
    QString gtk2_0, gtk3_0, gtk3_20;

    //gtk 2.0
    gtk2_0 = pathTheme + "/gtk-2.0/gtkrc";

    QFile r_gtk2_0(gtk2_0);
    QFile w_gtk2_0(gtk2_0);

    if (r_gtk2_0.open(QFile::ReadOnly))
    {
        QString content, selected_bg;

        content = r_gtk2_0.readAll();
        selected_bg = content.split("selected_bg_color:")[1];
        selected_bg = selected_bg.split("\\n")[0];
        content = content.replace("selected_bg_color:" + selected_bg, "selected_bg_color:" + color);

        r_gtk2_0.close();

        if (w_gtk2_0.open(QFile::WriteOnly))
        {
            w_gtk2_0.write(content.toUtf8());
            w_gtk2_0.close();
        }
    }

    //gtk 3.0
    gtk3_0 = pathTheme + "/gtk-3.0/gtk.css";

    QFile r_gtk3_0(gtk3_0);
    QFile w_gtk3_0(gtk3_0);

    if (r_gtk3_0.open(QFile::ReadOnly))
    {
        QString content, selected_bg, theme_selected_bg_color;

        content = r_gtk3_0.readAll();
        selected_bg = content.split("@define-color selected_bg_color ")[1];
        selected_bg = selected_bg.split(";")[0];
        content = content.replace("@define-color selected_bg_color " + selected_bg, "@define-color selected_bg_color " + color);

        theme_selected_bg_color = content.split("@define-color selected_bg_color ")[1];
        theme_selected_bg_color = theme_selected_bg_color.split(";")[0];
        content = content.replace("@define-color theme_selected_bg_color " + theme_selected_bg_color, "@define-color theme_selected_bg_color " + color);

        r_gtk3_0.close();

        if (w_gtk3_0.open(QFile::WriteOnly))
        {
            w_gtk3_0.write(content.toUtf8());
            w_gtk3_0.close();
        }
    }

    //gtk 3.20
    gtk3_20 = pathTheme + "/gtk-3.20/gtk.css";

    QFile r_gtk3_20(gtk3_20);
    QFile w_gtk3_20(gtk3_20);

    if (r_gtk3_20.open(QFile::ReadOnly))
    {
        QString content, selected_bg, theme_selected_bg_color;

        content = r_gtk3_20.readAll();
        selected_bg = content.split("@define-color selected_bg_color ")[1];
        selected_bg = selected_bg.split(";")[0];
        content = content.replace("@define-color selected_bg_color " + selected_bg, "@define-color selected_bg_color " + color);

        theme_selected_bg_color = content.split("@define-color selected_bg_color ")[1];
        theme_selected_bg_color = theme_selected_bg_color.split(";")[0];
        content = content.replace("@define-color theme_selected_bg_color " + theme_selected_bg_color, "@define-color theme_selected_bg_color " + color);

        r_gtk3_20.close();

        if (w_gtk3_20.open(QFile::WriteOnly))
        {
            w_gtk3_20.write(content.toUtf8());
            w_gtk3_20.close();
        }
    }
}
