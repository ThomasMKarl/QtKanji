#ifndef EXAMPLEWINDOW_H
#define EXAMPLEWINDOW_H

#include "headers.h"

#include "mainwindow.h"



namespace QtKanji {

class ExampleWindow : public QWidget
{
  Q_OBJECT

 public:
  static ExampleWindow* createExampleWindow(SharedData &dataHandler_,
	                                    unsigned int successes_,
	                                    unsigned int failures_,
	                                    QWidget *parent = 0)
    {return new ExampleWindow{dataHandler_, successes_, failures_, parent};}
  
  unsigned int successes{0};
  unsigned int failures{0};
  unsigned int randId{0};
  
  QLineEdit displayFurigana{}, displayKanji{};
  QLabel Furigana{"Furigana:"}, Furigana2{},
         English{"Translation: "}, English2{},
         Kanji{"Kanji: "}, Kanji2{},
         Success{"Success!"}, Success2{},
         Failure{"Failure!"}, Failure2{},
         Rate{}, Rate2{},
         cards{};
  QPushButton submitButton{"submit"}, continueButton{"continue"};
  QGridLayout layout{};

  SharedData dataHandler{};

 private slots:
  void   submitButtonClicked();
  void continueButtonClicked();

 private:
  explicit ExampleWindow(SharedData &dataHandler_,
	                 unsigned int successes_,
	                 unsigned int failures_,
	                 QWidget *parent = 0);
  void showFailure();
  void showSuccess();
};

};

#endif
