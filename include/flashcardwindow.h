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
  static FlashcardWindow* createFlashcardWindow(bool randomize_,
					                                      bool fromCardbox_,
                                                SharedData &dataHandler_,
					                                      SharedBoxes &boxes_,
					                                      SharedTable &table_,                                             
	                                              QWidget *parent = 0)
    {return new FlashcardWindow{randomize_,
			                          fromCardbox_,
                                dataHandler_,
			                          boxes_,
			                          table_,
			                          parent};}

  static FlashcardWindow* createFlashcardWindow(SharedData &dataHandler_,
					                                      SharedTable &table_,
	                                              unsigned int ID,
	                                              QWidget *parent = 0)
    {return new FlashcardWindow{dataHandler_,
			                          table_,
			                          ID,
			                          parent};}
  
  bool randomize{false};
  bool fromCardbox{false}; 
  unsigned int numberOfRemoves{0};
  unsigned int successes{0};
  unsigned int failures{0};
  unsigned int currentId{};
  unsigned int flashcardWindowIndex{};

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
  QFont textfont{};
  QGridLayout layout{};

  SharedData dataHandler{};
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
                           SharedData &dataHandler_,
		                       SharedBoxes &boxes_,
		                       SharedTable &table_,
	                         QWidget *parent = 0);

  explicit FlashcardWindow(SharedData &dataHandler_,
		                       SharedTable &table_,
	                         unsigned int ID,
	                         QWidget *parent = 0);
  void showSuccess();
  void showFailure();
  void setButtonLayout();
  void setFlashcardLayout();
  void update();
  void hideAll();
};
  
};

#endif
