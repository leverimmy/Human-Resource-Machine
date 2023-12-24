#include "widget.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    // 对当前窗口安装字体
    QFontDatabase::addApplicationFont(QStringLiteral(":/font/res/Silver.ttf"));
    Widget w;
    w.show();
    return a.exec();
}
