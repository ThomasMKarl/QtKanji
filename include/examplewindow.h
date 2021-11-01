#ifndef EXAMPLEWINDOW_H
#define EXAMPLEWINDOW_H

#include "headers.h"

#include "mainwindow.h"

namespace QtKanji
{

class ExampleWindow : public QWidget
{
public:
  static ExampleWindow *createExampleWindow(DataHandler &dataHandler_, QWidget *parent = 0)
  {
    return new ExampleWindow{dataHandler_, parent};
  }

  DataHandler dataHandler{};

  ExampleWindow(const ExampleWindow &) = delete;
  ExampleWindow &operator=(const ExampleWindow &) = delete;
  ExampleWindow(ExampleWindow &&) = delete;
  ExampleWindow &operator=(ExampleWindow &&) = delete;
  ~ExampleWindow() = default;

private slots:
  void submitButtonClicked();
  void continueButtonClicked();

private:
  explicit ExampleWindow(DataHandler &dataHandler_, QWidget *parent = 0);

  void setExampleWindowLayout();
  void setButtonLayout();
  void update();
  void showFailure();
  void showSuccess();
  void showResult();
  void hideAll();

  unsigned int successes{0};
  unsigned int failures{0};

  QLineEdit displayFurigana{}, displayKanji{};
  QLabel Furigana{"Furigana:"}, Furigana2{}, English{"Translation: "}, English2{}, Kanji{"Kanji: "}, Kanji2{},
      Success{"Success!"}, Success2{}, Failure{"Failure!"}, Failure2{}, Rate{}, Rate2{}, cards{};
  QPushButton submitButton{"submit"}, continueButton{"continue"};
  QFont textfont{};
  QGridLayout layout{};

  Q_OBJECT
};

}; // namespace QtKanji

#endif
