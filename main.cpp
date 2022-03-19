#include "LyricSlicer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif
    a.setStyle("Fusion");
    LyricSlicer w;
    w.show();
    return a.exec();
}
