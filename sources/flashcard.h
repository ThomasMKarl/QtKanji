#ifndef _FLASHCARD_
#define _FLASHCARD_

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

class Flashcard : public QWidget
{
    Q_OBJECT

 public:
    Flashcard(unsigned int, unsigned int, std::string, std::vector<unsigned int>, unsigned int, unsigned int, std::vector<unsigned int>, int, int, bool, bool, bool, QWidget *parent = 0);
    ~Flashcard() = default;

 private slots:
    void submitButtonClicked();
    void   contButtonClicked();
    void    addButtonClicked();
    void    remButtonClicked();

 private:
    int mContFlag, mRemFlag = 0;

    QLineEdit *displayId, *displaySign, *displayImi, *displayKun, *displayOn;
    QLabel *Id, *Sign, *Imi, *Kun, *On, *Success, *Failure, *Success2, *Failure2, *Rate, *Rate2, *cards;
    QPushButton *submitButton, *contButton, *addButton, *remButton;
    std::string truedataSign, dataSign, truedataImi, dataImi, truedataKun, dataKun, truedataOn, dataOn;

    unsigned int randDel, mLL, mUL, randId;
    unsigned int mSuccesses;
    unsigned int mFailures;

    std::string mDirection;
    std::vector<std::string> dataImivec, truedataImivec, dataOnvec, truedataOnvec, dataKunvec, truedataKunvec;
    std::vector<unsigned int> mNumbers;
    std::vector<unsigned int> mCardbox;
    QFont textfont;

    bool mHideimi = false; 
    bool mHidekun = false;
    bool mHideon  = false;
};

std::vector<std::string> explode(const std::string &delimiter, const std::string &str);

template<typename T>
bool contains(std::vector<T> v, T x);

template<typename T>
bool one_in_vec(std::vector<T> v1, std::vector<T> v2);

template<typename T>
bool all_in_vec(std::vector<T> v1, std::vector<T> v2);

#endif
