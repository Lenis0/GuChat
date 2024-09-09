#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QStyleFactory>
#include "global.h"

int main(int argc, char* argv[]) {
    // 永不缩放 100%
    // QApplication::setAttribute(Qt::AA_Use96Dpi);

    // 使用高分辨率的位图（可选）
    // QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    // // 向下取整的缩放 124% 150%也会变为100%
    // QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
    //     Qt::HighDpiScaleFactorRoundingPolicy::Floor);

    QApplication a(argc, argv);
    // // 获取所有可用的样式名称
    // QStringList styles = QStyleFactory::keys();
    // // 遍历并打印样式名称
    // foreach (const QString &style, styles) {
    //     qDebug() << style;
    // }

    a.setStyle(QStyleFactory::create("windowsvista")); // 切换风格
    // 设置具体样式
    QFile qss(":/style/stylesheet.qss");
    if (qss.open(QFile::ReadOnly)) {
        qDebug("qss open success");
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    } else {
        qDebug("qss open failed");
    }

    // 获取当前应用程序的路径
    QString app_path = QCoreApplication::applicationDirPath();
    // 拼接文件名
    QString fileName = "config.ini";
    QString config_path = QDir::toNativeSeparators(app_path + QDir::separator() + fileName);
    QSettings settings(config_path, QSettings::IniFormat); // 读取ini文件
    QString gate_host = settings.value("GateServer/Host").toString();
    QString gate_port = settings.value("GateServer/Port").toString();
    gate_url_prefix = "http://" + gate_host + ":" + gate_port;

    MainWindow w;
    w.show();
    return a.exec();
}
