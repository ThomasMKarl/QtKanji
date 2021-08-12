#ifndef FLASHCARDWINDOW_H
#define FLASHCARDWINDOW_H

#include "headers.h"

#include "mainwindow.h"
#include "examplewindow.h"



namespace QtKanji {

class FlashcardWindow : public QWidget
{
  Q_OBJECT

 public:
  static FlashcardWindow* createFlashcardWindow(SharedData &dataHandler_,
					        SharedBoxes &boxes_,
					        SharedTable &table_,
					        bool fromCardbox_,
					        int removeFlag_,
	                                        unsigned int successes_,
	                                        unsigned int failures_,
	                                        QWidget *parent = 0)
    {return new FlashcardWindow{dataHandler_,
			        boxes_,
			        table_,
			        fromCardbox_, removeFlag_,
			        successes_, failures_,
			        parent};}
  static FlashcardWindow* createFlashcardWindow(SharedData &dataHandler_,
					        SharedTable &table_,
	                                        unsigned int ID,
	                                        QWidget *parent = 0)
    {return new FlashcardWindow{dataHandler_,
			        table_,
			        ID,
			        parent};}
  
  bool fromCardbox{false};
  int removeFlag{0};
  unsigned int randId{0};
  unsigned int successes{0};
  unsigned int failures{0};

  QLineEdit
    displaySign{},
    displayImi{},
    displayKun{},
    displayOn{};
  QLabel
    Sign{"漢字:"},
    Imi{"いみ:"}, Kun{"くん:"}, On{"おん:"},
    Success{"Success!"}, Failure{"Failure!"},
    Success2{}, Failure2{},
    Rate{}, Rate2{},
    cards{};
  QPushButton
    submitButton{"submit"},
    addButton{"add to cardbox"},
    continueButton{"continue"},
    removeButton{"remove from cardbox"};
  QGridLayout layout{};

  SharedData dataHandler{};
  SharedBoxes boxes{};
  SharedTable table{};

 private slots:
  void   submitButtonClicked();
  void continueButtonClicked();
  void      addButtonClicked();
  void   removeButtonClicked();

 private:
  explicit FlashcardWindow(SharedData &dataHandler_,
		           SharedBoxes &boxes_,
		           SharedTable &table_,
		           bool fromCardbox_,
	                   int removeFlag_,
	                   unsigned int successes_,
	                   unsigned int failures_,
	                   QWidget *parent = 0);
  explicit FlashcardWindow(SharedData &dataHandler_,
		           SharedTable &table_,
	                   unsigned int ID,
	                   QWidget *parent = 0);
  void showSuccess();
  void showFailure();
  void setButtonLayout();
  void setFlashcardLayout();
};
  
};

#endif
