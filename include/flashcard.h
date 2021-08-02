#ifndef FLASHCARD_H
#define FLASHCARD_H

#include "headers.h"

#include "include/mainwindow.h"
#include "include/examplewindow.h"



namespace QtKanji {

class Table;
  
class Flashcard : public QWidget
{
  Q_OBJECT

 public:
  static Flashcard* createFlashcardWindow(DataHandler &dataHandler_,
	                                  unsigned int successes_,
	                                  unsigned int failures_,
	                                  bool fromCardbox_,
	                                  int removeFlag_,
	                                  Boxes &boxes_,
	                                  QWidget *parent = 0)
    {return new Flashcard{dataHandler_,
			  successes_, failures_,
			  fromCardbox_, removeFlag_,
			  boxes_,
			  parent};}
  
  bool fromCardbox{false};
  int removeFlag{0};
  unsigned int randId{0};
  unsigned int successes{0};
  unsigned int failures{0};
  
  std::shared_ptr<DataHandler> dataHandler{};
  std::shared_ptr<Boxes> boxes{};

  std::unique_ptr<QLineEdit> displayId{}, displaySign{}, displayImi{}, displayKun{}, displayOn{};
  std::unique_ptr<QLabel> Id{}, Sign{}, Imi{}, Kun{}, On{}, Success{}, Failure{}, Success2{}, Failure2{}, Rate{}, Rate2{}, cards{};
  std::unique_ptr<QPushButton> submitButton{}, continueButton{}, addButton{}, removeButton{};
  std::unique_ptr<QGridLayout> layout{};

 private slots:
  void   submitButtonClicked();
  void continueButtonClicked();
  void      addButtonClicked();
  void   removeButtonClicked();

 private:
  explicit Flashcard(DataHandler &dataHandler_,
	             unsigned int successes_,
	             unsigned int failures_,
	             bool fromCardbox_,
	             int removeFlag_,
	             Boxes &boxes_,
	             QWidget *parent = 0);
  void showResult();
  void showSuccess();
  void showFailure();
  void setButtonLayout();
  void setFlashcardLayout();
  void drawFlashcard(std::vector<unsigned int> &list);
};

};

#endif
