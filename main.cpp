#include "sliderlistwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStringList sss = { "123", "234", "345", "456", "567", "678", "789", "890", "901", "012" };

    QSize size(400, 100);
    size = QSize(100, 400);
    SliderListWidget w(sss, size, Qt::Vertical);
    w.show();
    return a.exec();
}
