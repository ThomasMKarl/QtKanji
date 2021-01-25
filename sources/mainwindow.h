#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include <QString>
#include <QApplication>
#include <QPainter>
#include <QCheckBox>
#include <iostream>
#include <string>
#include <ctime>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private slots:
    void      signButtonClicked();
    void containerButtonClicked();
    void        exButtonClicked();
    void    engjapButtonClicked();
    void    japengButtonClicked();
    void             boxChecked();

    void start_flashcard_window();
    void start_example_window();

private:
    QLineEdit *displayLL, *displayUL;
    QLabel *LL, *UL;
    QPushButton *signButton, *containerButton, *exButton, *engjapButton, *japengButton;
    QGridLayout mLayout;
    QFont textfont;
    QLabel *dataFail, *cbFail;
    QCheckBox *imibox, *kunbox, *onbox;
    std::vector<unsigned int> numbers;
    std::vector<unsigned int> cardbox;

    unsigned int mLL; //lower limit
    unsigned int mUL; //upper limit
    int container_flag;
    bool hideimi = false;
    bool hidekun = false;
    bool hideon  = false;

    std::string direction = "engjap";

    unsigned int failures = 0;
};

//std::vector<std::string> explode3(const std::string, const std::string);
//std::vector<unsigned int> convert_string(std::vector<std::string>);

#endif

