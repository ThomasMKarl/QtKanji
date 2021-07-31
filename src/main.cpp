#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QtKanji::MainWindow mainWindow = QtKanji::MainWindow::createMainWindow();
    mainWindow.setWindowIcon(QIcon("kanji.ico"));
    mainWindow.show();

    return app.exec();
}
