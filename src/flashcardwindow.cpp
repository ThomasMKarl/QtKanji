#include "flashcardwindow.h"
#include "table.h"
#include<iostream>
QtKanji::FlashcardWindow::FlashcardWindow(SharedData &dataHandler_,
			                  SharedBoxes &boxes_,
			                  SharedTable &table_,
			                  bool fromCardbox_,
                                          int removeFlag_,
                                          unsigned int successes_,
                                          unsigned int failures_,
                                          QWidget *parent) :
  dataHandler(dataHandler_),
  boxes(boxes_),
  table(table_),
  fromCardbox(fromCardbox_),
  removeFlag(removeFlag_),
  successes(successes_),
  failures(failures_),
  QWidget(parent)
{
  table->flashcards.push_back( this );

  auto &FC = dataHandler->flashcard;
 
  QFont textfont{};
  textfont.setPointSize(20);
  textfont.setBold(false);
  setFont(std::move(textfont));
  setWindowIcon(QIcon("kanji.ico"));
  
  unsigned int range{};
  if(fromCardbox) range = dataHandler->indexInCardbox.size();
  else            range = dataHandler->upperLimit - dataHandler->lowerLimit + 1;

  if(dataHandler->indexContainer.size() == range + removeFlag)
  {
    showResult(*this);
    return;
  }
    
  randId = dataHandler->computeRandomId(fromCardbox, removeFlag);

  dataHandler->computeKanjiData(randId);

  setButtonLayout();

  setFlashcardLayout();

  if(dataHandler->fromEngToJap)
  {
    if(!boxes->hideKun) displayKun.clear();
    if(!boxes->hideOn )  displayOn.clear();
    if(!boxes->hideImi) displayImi.clear();
  }
  else displaySign.clear();

  if(boxes->hideImi)
  {
    displayImi.hide();
    Imi.hide();
  }
  if(boxes->hideKun)
  {
    displayKun.hide();
    Kun.hide();
  }
  if(boxes->hideOn)
  {
    displayOn.hide();
    On.hide();
  }

  setWindowTitle("QtKanji #" + QString::fromStdString(std::to_string(randId)));
}

QtKanji::FlashcardWindow::FlashcardWindow(SharedData &dataHandler_,
			                  SharedTable &table_,
                                          unsigned int ID,
                                          QWidget *parent) :
  dataHandler(dataHandler_),
  table(table_),
  randId(ID),
  QWidget(parent)
{ 
  table->flashcards.push_back( this );

  QFont textfont{};
  textfont.setPointSize(20);
  textfont.setBold(false);
  setFont(std::move(textfont));
  setWindowIcon(QIcon("kanji.ico"));

  dataHandler->computeKanjiData(randId);

  setFlashcardLayout();

  setWindowTitle("QtKanji #" + QString::fromStdString(std::to_string(randId)));
}

#include<iostream>
void QtKanji::FlashcardWindow::submitButtonClicked()
{
  Flashcard myFlashcard{};
  
  const auto &FC = dataHandler->flashcard;
 
  std::string input{};
  
  myFlashcard.dataSign = displaySign.text().toStdString();
      
  input = displayOn.text().toStdString();
  if(input == "-none-") input.clear();
  myFlashcard.dataOnVector  = explode(", ", std::move(input));

  input  = displayKun.text().toStdString();
  if(input == "-none-") input.clear();
  myFlashcard.dataKunVector = explode(", ", std::move(input));

  input  = displayImi.text().toStdString();
  myFlashcard.dataImiVector = explode(", ", std::move(input));

  if(myFlashcard == FC) showSuccess();
  else                  showFailure();
}

void QtKanji::FlashcardWindow::continueButtonClicked()
{
  FlashcardWindow *flashcard = createFlashcardWindow(dataHandler,
					             boxes,
					             table,
					             fromCardbox, removeFlag,
                                                     successes, failures);
  this->close();
  flashcard->show();
}

void QtKanji::FlashcardWindow::addButtonClicked()
{
  removeButton.show();
  addButton.hide();
    
  const auto index = std::find(std::begin(dataHandler->indexInCardbox),
			       std::end(dataHandler->indexInCardbox),
			       randId);
  if(index == std::end(dataHandler->indexInCardbox))
  {
    dataHandler->indexInCardbox.push_back(randId);
      
    if(fromCardbox) --removeFlag;

    std::ofstream containerData{dataHandler->pathToContainerData, std::ios::app};
    containerData << std::to_string(randId)+":";
  }
}

void QtKanji::FlashcardWindow::removeButtonClicked()
{
  removeButton.hide();
  addButton.show();
    
  std::ofstream containerData{dataHandler->pathToContainerData, std::ios::trunc};
    
  const auto index = std::find(std::begin(dataHandler->indexInCardbox),
			       std::end(dataHandler->indexInCardbox),
			       randId);
  dataHandler->indexInCardbox.erase(index);
	
  if(fromCardbox) ++removeFlag;

  if(dataHandler->indexInCardbox.empty()) containerData << "";
  else containerData << std::to_string(dataHandler->indexInCardbox[randId])+":";
}

void QtKanji::FlashcardWindow::showSuccess()
{
  const auto &FC = dataHandler->flashcard;
  
  Success       .show();
  Failure       .hide();
  continueButton.show();
  submitButton  .hide();

  ++successes;

  if(dataHandler->fromEngToJap)
  {
    QString output{""};
    for(const auto& kun : FC.dataKunVector) output += QString::fromStdString(kun) + ", ";
    output.remove(output.size()-2, 2);
    displayKun.setText(std::move(output));
    displayKun.setStyleSheet("color: green");
    
    output = "";
    for(const auto& on  : FC.dataOnVector)  output += QString::fromStdString(on) + ", ";
    output.remove(output.size()-2, 2);
    displayOn.setText(std::move(output));
    displayOn.setStyleSheet("color: green");
    
    output = "";
    for(const auto& imi : FC.dataImiVector) output += QString::fromStdString(imi) + ", ";
    output.remove(output.size()-2, 2);
    displayImi.setText(std::move(output));
    displayImi.setStyleSheet("color: green");
  }
  else
  {
    displaySign.setText(QString::fromStdString(FC.dataSign));
    displaySign.setStyleSheet("color: green");
  }
}

void QtKanji::FlashcardWindow::showFailure()
{
  const auto &FC = dataHandler->flashcard;
  
  Success       .hide();
  Failure       .show();
  continueButton.show();
  submitButton  .hide();

  ++failures;

  if(dataHandler->fromEngToJap)
  {
    QString output{""};
    for(const auto& kun : FC.dataKunVector) output += QString::fromStdString(kun) + ", ";
    output.remove(output.size()-2, 2);
    displayKun.setText(std::move(output));
    displayKun.setStyleSheet("color: red");
    
    output = "";
    for(const auto& on  : FC.dataOnVector)  output += QString::fromStdString(on)  + ", ";
    output.remove(output.size()-2, 2);
    displayOn.setText(std::move(output));
    displayOn.setStyleSheet("color: red");
    
    output = "";
    for(const auto& imi : FC.dataImiVector) output += QString::fromStdString(imi) + ", ";
    output.remove(output.size()-2, 2);
    displayImi.setText(std::move(output));
    displayImi.setStyleSheet("color: red");
  }
  else
  {
    displaySign.setText(QString::fromStdString(FC.dataSign));
    displaySign.setStyleSheet("color: red");
  }
}

void QtKanji::FlashcardWindow::setButtonLayout()
{
  submitButton.setFixedSize(120,50);
  connect(&submitButton,
          &QPushButton::clicked,
          this,
          &FlashcardWindow::submitButtonClicked);
  layout.addWidget(&submitButton,3,0);

  addButton.setFixedSize(300,50);
  connect(&addButton,
          &QPushButton::clicked,
	  this,
	  &FlashcardWindow::addButtonClicked);
  layout.addWidget(&addButton,4,0);

  removeButton.setFixedSize(300,50);
  connect(&removeButton,
          &QPushButton::clicked,
	  this,
	  &FlashcardWindow::removeButtonClicked);
  layout.addWidget(&removeButton,4,0);

  continueButton.setFixedSize(120,50);
  connect(&continueButton,
          &QPushButton::clicked,
	  this,
	  &FlashcardWindow::continueButtonClicked);
  layout.addWidget(&continueButton,3,0);
  continueButton.hide();

  if(contains(dataHandler->indexInCardbox, randId))    addButton.hide();
  else                                              removeButton.hide();
  
  setLayout(&layout);
}

void QtKanji::FlashcardWindow::setFlashcardLayout()
{
  const auto &FC = dataHandler->flashcard;
  
  displaySign.setText(QString::fromStdString(FC.dataSign));
  QFont textfont{};
  textfont.setPointSize(50);
  textfont.setBold(false);
  displaySign.setFont(std::move(textfont));
  displaySign.setFixedSize(100,100);

  QString output{""};
  for(const auto& kun : FC.dataKunVector) output += QString::fromStdString(kun) + ", ";
  output.remove(output.size()-2, 2);
  displayKun.setText(std::move(output));
  displayKun.setFixedSize(250,50);
  
  output = "";
  for(const auto& imi : FC.dataImiVector) output += QString::fromStdString(imi) + ", ";
  output.remove(output.size()-2, 2);
  displayImi.setText(std::move(output));
  displayImi.setFixedSize(500,50);
  
  output = "";
  for(const auto& on : FC.dataOnVector)   output += QString::fromStdString(on) + ", ";
  output.remove(output.size()-2, 2);
  displayOn.setText(std::move(output));
  displayOn.setFixedSize(250,50);

  layout.addWidget(&Success,4,1);
  layout.addWidget(&Failure,4,1);
  Success.hide();
  Success.setStyleSheet("color: green");
  Failure.hide();
  Failure.setStyleSheet("color: red");

  layout.addWidget(&displaySign,2,1);
  layout.addWidget(&Sign,1,1);
  layout.addWidget(&displayImi,2,2);
  layout.addWidget(&Imi,1,2);
  layout.addWidget(&displayKun,2,3);
  layout.addWidget(&Kun,1,3);
  layout.addWidget(&displayOn,2,4);
  layout.addWidget(&On,1,4);
 
  setLayout(&layout);
}
