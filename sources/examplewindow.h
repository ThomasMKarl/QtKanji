#ifndef EXAMPLEWINDOW_H
#define EXAMPLEWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QApplication>
#include <QString>
#include <QDesktopWidget>
#include <QStyle>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>

class Example : public QWidget
{
    Q_OBJECT

 public:
    Example(std::vector<std::string>, std::vector<std::string>, std::vector<std::string>, std::vector<unsigned int>, std::string, unsigned int, unsigned int, QWidget *parent = 0);
    ~Example() = default;

 private slots:
    void submitButtonClicked();
    void contButtonClicked();

 private:
    QLineEdit *displayFuri, *displayKan;
    QLabel *Id, *Furi, *Furi2, *Eng, *Eng2, *Kan, *Kan2, *Success, *Failure, *Success2, *Failure2, *Rate, *Rate2;
    QPushButton *submitButton, *contButton;
    QGridLayout mLayout;
    QFont textfont;

    unsigned int randDel, randId;
    unsigned int mSuccesses;
    unsigned int mFailures;

    std::vector<std::string> dataFuri, truedataFuri, dataKan, truedataKan, dataEng;
    std::string mDirection;
    std::vector<unsigned int> mNumbers;
};

std::vector<std::string> explode2(const std::string &delimiter, const std::string &str);

template<typename T>
bool contains2(std::vector<T> v, T x);

template<typename T>
bool all_in_vec2(std::vector<T> v1, std::vector<T> v2);

#endif // EXAMPLEWINDOW_H

