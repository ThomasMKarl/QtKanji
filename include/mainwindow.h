#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "headers.h"

#include "datahandler.h"
#include "examplewindow.h"
#include "flashcardwindow.h"
#include "hadamitzkywindow.h"
#include "table.h"

namespace QtKanji
{

class MainWindow : public QWidget
{
  Q_OBJECT

public:
  static MainWindow createMainWindow(QWidget *parent = 0)
  {
    return MainWindow{parent};
  }

  HadamitzkyWindow hadamitzkyWindow{};
  SharedBoxes boxes{};
  SharedTable table{};

  MainWindow(const MainWindow &) = delete;
  MainWindow &operator=(const MainWindow &) = delete;
  MainWindow(MainWindow &&) = delete;
  MainWindow &operator=(MainWindow &&) = delete;
  ~MainWindow();

private slots:
  void signButtonClicked();
  void cardboxButtonClicked();
  void exampleButtonClicked();
  void engjapButtonClicked();
  void japengButtonClicked();
  void printExampleButtonClicked();
  void printSignButtonClicked();
  void searchButtonClicked();
  void boxChecked();
  void randomizeChecked();

private:
  explicit MainWindow(QWidget *parent = 0);

  void addButtonsToLayout();
  void addBoxesToLayout();
  void addDisplaysToLayout();
  void startFlashcardWindow(bool fromCardbox);
  void startFlashcardWindow(unsigned int ID);
  Error checkLimits(unsigned int lowerLimit, unsigned int upperLimit);
  void displayErrorMessage(QtKanji::Error err);
  void hideErrors();

  bool randomize{false};
  bool fromEngToJap{true};

  QLineEdit displayLowerLimit{}, displayUpperLimit{}, search{};
  QLabel lowerLimit{"from:"}, upperLimit{"to:"}, dataFail{"file error!"}, cardboxFail{"no cards in box."},
      searchFail{"no such kanji in DB"}, cardboxLimitFail{"no cards within limits"};
  QPushButton signButton{"train kanji"}, cardboxButton{"cardbox"}, exampleButton{"train words"},
      engjapButton{"ふりがな - 漢字"}, japengButton{"漢字 - ふりがな"}, printExampleButton{"print examples"},
      printSignButton{"print kanji sorted"}, searchButton{"search"};
  QCheckBox randomizeBox{"randomize"};
  QGridLayout layout{};
  QFont textfont{};
};

}; // namespace QtKanji

#endif
