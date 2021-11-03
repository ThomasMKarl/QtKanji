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

class MainWindow : public QDialog
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
  void furikanjiButtonClicked();
  void kanjifuriButtonClicked();
  void engjapButtonClicked();
  void japengButtonClicked();
  void printExampleButtonClicked();
  void printSignButtonClicked();
  void searchButtonClicked();
  void boxChecked();
  void randomizeChecked();
  void kanjiChecked();

private:
  explicit MainWindow(QWidget *parent = 0);

  void addButtonsToLayout();
  void addBoxesToLayout();
  void addDisplaysToLayout();
  void startFlashcardWindow(bool fromCardbox);
  void startFlashcardWindow(unsigned int ID);
  Error checkLimits(unsigned int lowerLimit, unsigned int upperLimit, unsigned int max);
  void displayErrorMessage(QtKanji::Error err);
  void hideErrors();

  bool randomize{false};
  bool kanji{false};
  bool fromFuriToKanji{true};
  bool fromJapToEng{true};

  std::unique_ptr<QTabWidget> tabWidget{};
  std::unique_ptr<QTabWidget> tabHadWidget{};
  QLineEdit displayLowerLimit{}, displayUpperLimit{}, displayLowerLection{}, displayUpperLection{}, search{};
  QLabel lowerLimit{"kanji from:"}, upperLimit{"kanji to:"}, dataFail{"file error!"}, cardboxFail{"no cards in box."},
      searchFail{"kanji not found in DB"}, cardboxLimitFail{"no cards within limits"}, lowerLection{"lection from:"},
      upperLection{"lection to:"}, cardboxes{"cardboxes:"};
  QPushButton signButton{"train kanji"}, exampleButton{"train examples"}, wordButton{"train words"},
      furikanjiButton{"ふりがな - 漢字"}, kanjifuriButton{"漢字 - ふりがな"}, engjapButton{"Eng. - Jap."},
      japengButton{"Jap. - Eng."}, printExampleButton{"print examples"}, printSignButton{"print kanji sorted"},
      searchButton{"search"};
  QCheckBox randomizeBox{"randomize"}, kanjiBox{"words with kanji"};
  QGridLayout layout{};
  Buttons cardboxKanjiButtons{NUMBER_OF_CARDBOX_BUTTONS};
  Buttons cardboxExampleButtons{NUMBER_OF_CARDBOX_BUTTONS};
  Buttons cardboxWordButtons{NUMBER_OF_CARDBOX_BUTTONS};
  QFont textfont{};
};

}; // namespace QtKanji

#endif
