#include "EQClusterViewer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EQClusterViewer w;
    w.show();

    return a.exec();
}
