#ifndef FLASHCARDWINDOW_H
#define FLASHCARDWINDOW_H

#include "headers.h"

#include "mainwindow.h"
#include "examplewindow.h"
#include "hadamitzkydata.h"



namespace QtKanji {

class FlashcardWindow : public QWidget
{
  Q_OBJECT

 public:
  static FlashcardWindow* createFlashcardWindow(bool randomize_,
					        bool fromCardbox_,
                                                DataHandler dataHandler_,
					        SharedBoxes &boxes_,
					        SharedTable &table_,                                             
	                                        QWidget *parent = 0)
    {return new FlashcardWindow{randomize_,
			        fromCardbox_,
                                std::move(dataHandler_),
			        boxes_,
			        table_,
			        parent};}

  static FlashcardWindow* createFlashcardWindow(unsigned int ID, QWidget *parent = 0)
    {return new FlashcardWindow{ID, parent};}

  QLineEdit displaySign{};

  DataHandler dataHandler{};
  SharedBoxes boxes{};
  SharedTable table{};

  FlashcardWindow(const FlashcardWindow&) = delete;
  FlashcardWindow& operator=(const FlashcardWindow&) = delete;
  FlashcardWindow(FlashcardWindow&&) = delete;
  FlashcardWindow& operator=(FlashcardWindow&&) = delete;
  ~FlashcardWindow();

 private slots:
  void   submitButtonClicked();
  void continueButtonClicked();
  void      addButtonClicked();
  void   removeButtonClicked();

 private:
  explicit FlashcardWindow(bool randomize_,
			   bool fromCardbox_,
                           DataHandler dataHandler_,
		           SharedBoxes &boxes_,
		           SharedTable &table_,
	                   QWidget *parent = 0);

  explicit FlashcardWindow(unsigned int ID, QWidget *parent = 0);

  void showSuccess();
  void showFailure();
  void showResult();
  void setButtonLayout();
  void setFlashcardLayout();
  void setHadamitzkyLayout(unsigned int hadamitzkyId);
  void update();
  void hideAll();

  bool randomize{false};
  bool fromCardbox{false}; 
  unsigned int numberOfRemoves{0};
  unsigned int successes{0};
  unsigned int failures{0};
  unsigned int currentId{};
  int flashcardWindowIndex{};

  QLineEdit
    displayImi{},
    displayKun{},
    displayOn{};
  QLabel
    Sign{"漢字:"},
    Imi{"いみ:"}, Kun{"くん:"}, On{"おん:"},
    hid{}, allGraphemes{}, gids{}, radicals{},
    Success{"Success!"}, Failure{"Failure!"},
    Success2{}, Failure2{},
    Rate{}, Rate2{},
    cards{};
  QPushButton
    submitButton{"submit"},
    addButton{"add to cardbox"},
    continueButton{"continue"},
    removeButton{"remove from cardbox"};
  QFont textfont{};
  QGridLayout layout{};
};
  
};

#endif
