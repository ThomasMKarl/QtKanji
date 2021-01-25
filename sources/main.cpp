#include "mainwindow.h"
#include <QApplication>
#include "screen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon("../kanji_windows/kanji.ico"));
    w.show();

    return a.exec();
}
