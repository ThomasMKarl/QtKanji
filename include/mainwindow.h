#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "headers.h"

#include "datahandler.h"
#include "examplewindow.h"
#include "flashcardwindow.h"
#include "hadamitzkywindow.h"
#include "table.h"



namespace QtKanji {
  
class MainWindow : public QWidget
{
  Q_OBJECT

 public:
  static MainWindow createMainWindow(QWidget *parent = 0) {return MainWindow{parent};}

  bool examplesAreToRandomize() {return randomize;}

  QLineEdit displayLowerLimit{}, displayUpperLimit{}, search{};
  QLabel
    lowerLimit{"from:"},
    upperLimit{"to:"},
    dataFail{"file error!"},
    cardboxFail{"no cards in box."};
  QPushButton
    signButton{"train kanji"},
    cardboxButton{"cardbox"},
    exampleButton{"train words"},
    engjapButton{"ふりがな - 漢字"},
    japengButton{"漢字 - ふりがな"},
    printButton{"print examples"},
    searchButton{"search"};
  QGridLayout layout{};
  QCheckBox randomizeBox{"randomize"};

  HadamitzkyWindow hadamitzkyWindow{};
  SharedBoxes boxes{};
  SharedTable table{};
  SharedData dataHandler{};

 private slots:
  void    signButtonClicked();
  void cardboxButtonClicked();
  void exampleButtonClicked();
  void  engjapButtonClicked();
  void  japengButtonClicked();
  void   printButtonClicked();
  void  searchButtonClicked();
  void           boxChecked();
  void     randomizeChecked();

 private:
  explicit MainWindow(QWidget *parent = 0);
  void addButtonsToLayout();
  void addBoxesToLayout();
  void addDisplaysToLayout();
  void startFlashcardWindow(bool fromCardbox);
  void startFlashcardWindow(unsigned int ID);
  void startExampleWindow();
  void printExamples();
  void searchKanji();

  bool randomize{false};
};

template <typename W>
void showResult(W &window)
{
  double ratio =
    100.0*window.successes /
    (window.failures + window.successes);
  
  window.Success.setText("Successes:");
  window.Success2.setText(QString::number(window.successes));
  window.Failure.setText("Failures:");
  window.Failure2.setText(QString::number(window.failures));
  window.Rate.setText("Success Rate:");
  window.Rate2.setText(QString::number(ratio) + "%");


  window.cards.setText("Number of cards in box: "
                       + QString::number(window.dataHandler->indexInCardbox.size()));
    
  if(window.dataHandler->indexInCardbox.empty())
    window.cards.setStyleSheet("color: green");
  else
    window.cards.setStyleSheet("color: red");

  window.layout.addWidget(&window.Success,  0,0);
  window.layout.addWidget(&window.Failure,  1,0);
  window.layout.addWidget(&window.Success2, 0,1);
  window.layout.addWidget(&window.Failure2, 1,1);
  window.layout.addWidget(&window.Rate,     2,0);
  window.layout.addWidget(&window.Rate2,    2,1);
  window.layout.addWidget(&window.cards,    3,0);

  window.Success2.setStyleSheet("color: green");
  window.Failure2.setStyleSheet("color: red");
  if(ratio >= 85)
    window.Rate2.setStyleSheet("color: green" );
  if(ratio < 85 && ratio >= 50)
    window.Rate2.setStyleSheet("color: yellow");
  if(ratio < 50 && ratio >  15)
    window.Rate2.setStyleSheet("color: orange");
  if(ratio <= 15)
    window.Rate2.setStyleSheet("color: red"   );

  window.setLayout(&window.layout);
  window.setWindowTitle("QtKanji Result");
  window.setWindowIcon(QIcon("kanji.ico"));
}

};

#endif

