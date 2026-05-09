#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFile>
#include <QDir>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 读取样式文件
    QFile qss(":/style/stylesheet.qss");
    if (qss.open(QFile::ReadOnly)) {
        qDebug("qss read success");
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }
    else {
        qDebug("qss read fail");
    }

    // 读取配置文件
    QString config_file = "config.ini";
    QString app_path = QApplication::applicationDirPath();
    // 不同系统分隔符不同，要用QDir接口
    QString config_path = QDir::toNativeSeparators(app_path + QDir::separator() + config_file);
    QSettings settings(config_path, QSettings::IniFormat);
    QString gate_host = settings.value("GateServer/host").toString();
    QString gate_port = settings.value("GateServer/port").toString();
    gate_url_prefix = "http://" + gate_host + ":" + gate_port;

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "IMClient-qt_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return QCoreApplication::exec();
}
