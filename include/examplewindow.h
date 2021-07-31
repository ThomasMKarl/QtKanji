#ifndef EXAMPLEWINDOW_H
#define EXAMPLEWINDOW_H

#include "headers.h"

#include "mainwindow.h"



namespace QtKanji {
class Example : public QWidget
{
  Q_OBJECT

 public:
  static Example* createCardboxWindow(DataHandler &dataHandler_,
	                              unsigned int successes_,
	                              unsigned int failures_,
	                              QWidget *parent = 0)
    {return new Example{dataHandler_, successes_, failures_, parent};}
  
  std::unique_ptr<QLineEdit> displayFurigana{}, displayKanji{};
  std::unique_ptr<QLabel>
    Id{},
    Furigana{}, Furigana2{},
    English{}, English2{},
    Kanji{}, Kanji2{},
    Success{}, Success2{},
    Failure{}, Failure2{},
    Rate{}, Rate2{},
    cards{};
  std::unique_ptr<QPushButton> submitButton{}, continueButton{};
  std::unique_ptr<QGridLayout> layout{};

  unsigned int randId{0};
  unsigned int successes{0};
  unsigned int failures{0};

  std::shared_ptr<DataHandler> dataHandler{};

 private slots:
  void   submitButtonClicked();
  void continueButtonClicked();

 private:
  explicit Example(DataHandler &dataHandler_,
	           unsigned int successes_,
	           unsigned int failures_,
	           QWidget *parent = 0);
  void showFailure();
  void showSuccess();
};

};

#endif
