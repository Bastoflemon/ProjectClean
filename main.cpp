#include "HomePage.h"

#include <QApplication>
#include <QSettings>
#include <QTranslator>

QString readSettings(QString appPath)
{
    // 从ini读取设置
    QString path = appPath + "/ProjectClean.ini";
    QSettings settings(path, QSettings::IniFormat);
    QString re = settings.value("Language", "CN").toString();
    return re;
}
QTranslator* trans = NULL;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    trans = new QTranslator;
    QString path = QApplication::applicationDirPath() + "/Language";
    QString curLang = readSettings(path); //读取语言设置
    trans->load( path + "/ProjectClean_" + curLang + ".qm");
    a.installTranslator(trans);
    HomePage w;
    w.show();
    return a.exec();
}

