#include "widget.h"

#include <QApplication>

#include<QFontDatabase>
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    // int fontId = QFontDatabase::addApplicationFont(QStringLiteral(":/font/res/Silver.ttf"));
    // QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    // qDebug()<<"fontfamilies:"<<fontFamilies;
    // fontId = QFontDatabase::addApplicationFont(QStringLiteral(":/font/res/IPix.ttf"));
    // fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    // qDebug()<<"fontfamilies:"<<fontFamilies;
    QFontDatabase::addApplicationFont(QStringLiteral(":/font/res/Silver.ttf"));
    Widget w;
    w.show();
    return a.exec();
}
