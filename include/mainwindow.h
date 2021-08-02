#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "headers.h"

#include "datahandler.h"
#include "examplewindow.h"
#include "flashcard.h"
#include "table.h"



namespace QtKanji {
class MainWindow : public QWidget
{
  Q_OBJECT

 public:
  static MainWindow createMainWindow(QWidget *parent = 0) {return MainWindow{parent};}

  bool examplesAreToRandomize() {return randomize;}

  std::unique_ptr<QLineEdit> displayLowerLimit{}, displayUpperLimit{};
  std::unique_ptr<QLabel> lowerLimit{}, upperLimit{}, dataFail{}, cardboxFail{};
  std::unique_ptr<QPushButton> signButton{}, cardboxButton{}, exampleButton{}, engjapButton{}, japengButton{}, printButton{};
  std::shared_ptr<QCheckBox> randomizeBox{};
  std::unique_ptr<QGridLayout> layout{};
  std::shared_ptr<Boxes> boxes{};
  std::shared_ptr<DataHandler> dataHandler{};

 private slots:
  void    signButtonClicked();
  void cardboxButtonClicked();
  void exampleButtonClicked();
  void  engjapButtonClicked();
  void  japengButtonClicked();
  void   printButtonClicked();
  void           boxChecked();
  void     randomizeChecked();

 private:
  explicit MainWindow(QWidget *parent = 0);
  void startFlashcardWindow(bool fromCardbox);
  void startExampleWindow();
  void printExamples();

  bool randomize{false};
};

template <typename W>
void showResult(W &window)
{
  double ratio =
    100.0*window.successes /
    (window.failures + window.successes);

  window.layout = std::make_unique<QGridLayout>();
    
    window.Success  =
      std::make_unique<QLabel>("Successes:", &window);
    window.Success2 =
      std::make_unique<QLabel>(QString::number(window.successes), &window);
    window.Failure  =
      std::make_unique<QLabel>("Failures:", &window);
    window.Failure2 =
      std::make_unique<QLabel>(QString::number(window.failures), &window);
    window.Rate     =
      std::make_unique<QLabel>("Success Rate:", &window);
    window.Rate2    =
      std::make_unique<QLabel>(QString::number(ratio) + "%", &window);

    window.cards = std::make_unique<QLabel>
      ("Number of cards in box: "
       + QString::number(window.dataHandler->indexInCardbox.size()),
       &window);
    
    if(window.dataHandler->indexInCardbox.empty())
      window.cards->setStyleSheet("color: green");
    else
      window.cards->setStyleSheet("color: red");

    window.layout->addWidget(window.Success.get(),  0,0);
    window.layout->addWidget(window.Failure.get(),  1,0);
    window.layout->addWidget(window.Success2.get(), 0,1);
    window.layout->addWidget(window.Failure2.get(), 1,1);
    window.layout->addWidget(window.Rate.get(),     2,0);
    window.layout->addWidget(window.Rate2.get(),    2,1);
    window.layout->addWidget(window.cards.get(),    3,0);

    window.Success2->setStyleSheet("color: green");
    window.Failure2->setStyleSheet("color: red");
    if(ratio >= 85)
      window.Rate2->setStyleSheet("color: green" );
    if(ratio < 85 && ratio >= 50)
      window.Rate2->setStyleSheet("color: yellow");
    if(ratio < 50 && ratio >  15)
      window.Rate2->setStyleSheet("color: orange");
    if(ratio <= 15)
      window.Rate2->setStyleSheet("color: red"   );

    window.setLayout(window.layout.get());
    window.setWindowTitle("QtKanji Result");
    window.setWindowIcon(QIcon("kanji.ico"));
}

};

#endif

