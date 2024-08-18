#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>
#include <QFile>

int main(int argc, char *argv[]) {
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
    MainWindow w;
    w.show();
    return a.exec();
}
