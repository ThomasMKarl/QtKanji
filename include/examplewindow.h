#ifndef EXAMPLEWINDOW_H
#define EXAMPLEWINDOW_H

#include "headers.h"

#include "mainwindow.h"



namespace QtKanji {

class ExampleWindow : public QWidget
{
  Q_OBJECT

 public:
  static ExampleWindow* createExampleWindow(SharedData &dataHandler_, QWidget *parent = 0)
    {return new ExampleWindow{dataHandler_, parent};}
  
  unsigned int successes{0};
  unsigned int failures{0};
  
  QLineEdit displayFurigana{}, displayKanji{};
  QLabel Furigana{"Furigana:"}, Furigana2{},
         English{"Translation: "}, English2{},
         Kanji{"Kanji: "}, Kanji2{},
         Success{"Success!"}, Success2{},
         Failure{"Failure!"}, Failure2{},
         Rate{}, Rate2{},
         cards{};
  QPushButton submitButton{"submit"}, continueButton{"continue"};
  QFont textfont{};
  QGridLayout layout{};

  SharedData dataHandler{};

  ExampleWindow(const ExampleWindow&) = delete;
  ExampleWindow& operator=(const ExampleWindow&) = delete;
  ExampleWindow(ExampleWindow&&) = delete;
  ExampleWindow& operator=(ExampleWindow&&) = delete;
  ~ExampleWindow() = default;

 private slots:
  void   submitButtonClicked();
  void continueButtonClicked();

 private:
  explicit ExampleWindow(SharedData &dataHandler_, QWidget *parent = 0);
  void setExampleWindowLayout();
  void setButtonLayout();
  void update();
  void showFailure();
  void showSuccess();
};

};

#endif
