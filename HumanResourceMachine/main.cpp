#include "widget.h"

#include <QApplication>

#include<QFontDatabase>
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    int fontId = QFontDatabase::addApplicationFont(QStringLiteral(":/font/data/Silver.ttf"));
//    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
//    qDebug()<<"fontfamilies:"<<fontFamilies;
//    if (fontFamilies.size() > 0)
//    {
//        QFont font;
//        font.setFamily(fontFamilies[0]);
//        a.setFont(font);
//    }
    Widget w;
    w.show();
    return a.exec();
}
