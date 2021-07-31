#include "flashcard.h"

QtKanji::Flashcard::Flashcard(DataHandler &dataHandler_,
		              unsigned int successes_,
		              unsigned int failures_,
		              bool fromCardbox_,
		              int removeFlag_,
		              Boxes &boxes_,
		              QWidget *parent) : QWidget(parent)
{
  dataHandler = std::make_shared<DataHandler>(dataHandler_);
  fromCardbox = fromCardbox_;
  removeFlag = removeFlag_;
  successes = successes_;
  failures = failures_;
  boxes = std::make_shared<Boxes>(boxes_);
  layout = std::make_unique<QGridLayout>();
 
  QFont textfont{};
  textfont.setPointSize(20);
  textfont.setBold(false);
  setFont(std::move(textfont));
  setWindowIcon(QIcon("kanji.ico"));
  
  unsigned int cardboxSize = dataHandler->indexInCardbox.size();
  unsigned int randomIndex;
  srand(time(NULL));
  if(fromCardbox_ && !dataHandler->indexInCardbox.empty())
  {
    if(cardboxSize == 1) randomIndex = 0;
    else randomIndex = rand()%(cardboxSize-1);

    randId = dataHandler->indexInCardbox[randomIndex];

    dataHandler->lowerLimit = 0;
    dataHandler->upperLimit = cardboxSize-1;
  }
  else
  {
    randId = dataHandler->lowerLimit
      + rand()%(dataHandler->upperLimit - dataHandler->lowerLimit+1);
  }
  std::string randomIdString = std::to_string(randId);

  bool done{false};
  if(fromCardbox && dataHandler->indexInCardbox.empty()) done = true;  
  if(dataHandler->indexContainer.size() ==
     dataHandler->upperLimit - dataHandler->lowerLimit
     +1+removeFlag || done)
  {
    QtKanji::showResult(*this);
    return;
  }
  
  std::vector<unsigned int> list{};
  if(fromCardbox)
    list = dataHandler->indexInCardbox;
  else
    for(unsigned int i = dataHandler->lowerLimit; i <= dataHandler->upperLimit; i++) list.push_back(i);
    
  drawFlashcard(list);

  dataHandler->computeKanjiData(*this);

  setButtonLayout();

  if(dataHandler->fromEngToJap)
  {
    if(!boxes->hideKun) dataHandler->dataKun.clear();
    if(!boxes->hideOn )  dataHandler->dataOn.clear();
    if(!boxes->hideImi) dataHandler->dataImi.clear();
  }
  else dataHandler->dataSign.clear();

  setFlashcardLayout();

  if(boxes->hideImi)
  {
    displayImi->hide();
    Imi->hide();
  }
  if(boxes->hideKun)
  {
    displayKun->hide();
    Kun->hide();
  }
  if(boxes->hideOn)
  {
    displayOn->hide();
    On->hide();
  }
}

void QtKanji::Flashcard::submitButtonClicked()
{
    dataHandler->dataSign = displaySign->text().toStdString();

    dataHandler->dataOn  = displayOn  ->text().toStdString();
    if(dataHandler->dataOn  == "-none-") dataHandler->dataOn.clear();
    dataHandler->dataOnVector = explode(", ", dataHandler->dataOn);

    dataHandler->dataKun  = displayKun ->text().toStdString();
    if(dataHandler->dataKun == "-none-") dataHandler->dataKun.clear();
    dataHandler->dataKunVector = explode(", ", dataHandler->dataKun);

    dataHandler->dataImi  = displayImi ->text().toStdString();
    dataHandler->dataImiVector = explode(", ", dataHandler->dataImi);

    if((dataHandler->truedataSign == dataHandler->dataSign) &&
       allInVector(dataHandler->truedataOnVector, dataHandler->dataOnVector) &&
       oneInVector(dataHandler->dataImiVector, dataHandler->truedataImiVector) &&
       allInVector(dataHandler->truedataKunVector, dataHandler->dataKunVector)) showSuccess();
    else showFailure();
}

void QtKanji::Flashcard::continueButtonClicked()
{
  Flashcard *flashcard = new Flashcard(*dataHandler.get(),
				       successes, failures,
				       fromCardbox, removeFlag,
				       *boxes.get());
  this->close();
  flashcard->show();
}

void QtKanji::Flashcard::addButtonClicked()
{
  removeButton->show();
  addButton->hide();
    
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

void QtKanji::Flashcard::removeButtonClicked()
{
  removeButton->hide();
  addButton->show();
    
  std::ofstream containerData{dataHandler->pathToContainerData, std::ios::trunc};
    
  const auto index = std::find(std::begin(dataHandler->indexInCardbox),
			       std::end(dataHandler->indexInCardbox),
			       randId);
  dataHandler->indexInCardbox.erase(index);
	
  if(fromCardbox) ++removeFlag;

  if(dataHandler->indexInCardbox.empty()) containerData << "";
  else containerData << std::to_string(dataHandler->indexInCardbox[randId])+":";
}

void QtKanji::Flashcard::showSuccess()
{
  Success       ->show();
  Failure       ->hide();
  continueButton->show();
  submitButton  ->hide();

  ++successes;

  if(dataHandler->fromEngToJap)
  {
    displayKun->setText(QString::fromStdString(dataHandler->truedataKun));
    displayKun->setStyleSheet("color: green");

    displayOn->setText(QString::fromStdString(dataHandler->truedataOn));
    displayOn->setStyleSheet("color: green");

    displayImi->setText(QString::fromStdString(dataHandler->truedataImi));
    displayImi->setStyleSheet("color: green");
  }
  else
  {
    displaySign->setText(QString::fromStdString(dataHandler->truedataSign));
    displaySign->setStyleSheet("color: green");
  }
}

void QtKanji::Flashcard::showFailure()
{
  Success       ->hide();
  Failure       ->show();
  continueButton->show();
  submitButton  ->hide();

  ++failures;

  if(dataHandler->fromEngToJap)
  {
    displayKun->setText(QString::fromStdString(dataHandler->truedataKun));
    displayKun->setStyleSheet("color: red");

    displayOn->setText(QString::fromStdString(dataHandler->truedataOn));
    displayOn->setStyleSheet("color: red");

    displayImi->setText(QString::fromStdString(dataHandler->truedataImi));
    displayImi->setStyleSheet("color: red");
  }
  else
  {
    displaySign->setText(QString::fromStdString(dataHandler->truedataSign));
    displaySign->setStyleSheet("color: red");
  }
}

void QtKanji::Flashcard::setButtonLayout()
{  
  submitButton = std::make_unique<QPushButton>("submit");
  submitButton->setFixedSize(120,50);
  connect(submitButton.get(),
          &QPushButton::clicked,
          this,
          &Flashcard::submitButtonClicked);
  layout->addWidget(submitButton.get(),3,0);

  addButton = std::make_unique<QPushButton>("add to cardbox");
  addButton->setFixedSize(300,50);
  connect(addButton.get(),
          &QPushButton::clicked,
	  this,
	  &Flashcard::addButtonClicked);
  layout->addWidget(addButton.get(),4,0);

  removeButton = std::make_unique<QPushButton>("remove from cardbox");
  removeButton->setFixedSize(300,50);
  connect(removeButton.get(),
          &QPushButton::clicked,
	  this,
	  &Flashcard::removeButtonClicked);
  layout->addWidget(removeButton.get(),4,0);

  if(contains(dataHandler->indexInCardbox, randId)) addButton->hide();
  else removeButton->hide();

  continueButton = std::make_unique<QPushButton>("continue");
  continueButton->setFixedSize(120,50);
  connect(continueButton.get(),
          &QPushButton::clicked,
	  this,
	  &Flashcard::continueButtonClicked);
  layout->addWidget(continueButton.get(),3,0);
  continueButton->hide();

  setLayout(layout.get());
}

void QtKanji::Flashcard::setFlashcardLayout()
{
  QFont textfont{};
  textfont.setPointSize(50);
  textfont.setBold(false);

  displaySign = std::make_unique<QLineEdit>(this);
  Sign = std::make_unique<QLabel>("漢字:", this);
  displaySign->setText(QString::fromStdString(dataHandler->dataSign));
  displaySign->setFont(std::move(textfont));
  displaySign->setFixedSize(100,100);

  displayImi = std::make_unique<QLineEdit>(this);
  Imi = std::make_unique<QLabel>("いみ:", this);
  displayImi->setText(QString::fromStdString(dataHandler->dataImi));
  displayImi->setFixedSize(500,50);

  displayKun = std::make_unique<QLineEdit>(this);
  Kun = std::make_unique<QLabel>("くん:", this);
  displayKun->setText(QString::fromStdString(dataHandler->dataKun));
  displayKun->setFixedSize(250,50);

  displayOn = std::make_unique<QLineEdit>(this);
  On = std::make_unique<QLabel>("おん:", this);
  displayOn->setText(QString::fromStdString(dataHandler->dataOn));
  displayOn->setFixedSize(250,50);

  Success = std::make_unique<QLabel>("Success!", this);
  Failure = std::make_unique<QLabel>("Failure!", this);
  layout->addWidget(Success.get(),4,1);
  layout->addWidget(Failure.get(),4,1);
  Success->hide();
  Success->setStyleSheet("color: green");
  Failure->hide();
  Failure->setStyleSheet("color: red");

  layout->addWidget(displaySign.get(),2,1);
  layout->addWidget(Sign.get(),1,1);

  layout->addWidget(displayImi.get(),2,2);
  layout->addWidget(Imi.get(),1,2);

  layout->addWidget(displayKun.get(),2,3);
  layout->addWidget(Kun.get(),1,3);

  layout->addWidget(displayOn.get(),2,4);
  layout->addWidget(On.get(),1,4);

  setLayout(layout.get());
}

void QtKanji::Flashcard::drawFlashcard(std::vector<unsigned int> &list)
{
  while((contains(dataHandler->indexContainer, randId)
	|| !contains(list, randId))
	&& dataHandler->indexContainer.size()
	!= dataHandler->upperLimit - dataHandler->lowerLimit + 1 + removeFlag)
  {
    unsigned int randomIndex = dataHandler->lowerLimit + rand()%(dataHandler->upperLimit - dataHandler->lowerLimit + 1);
    if(fromCardbox) randId = dataHandler->indexInCardbox[randomIndex];
    else randId = randomIndex;
  }
  dataHandler->indexContainer.push_back(randId);

  setWindowTitle("QtKanji #" + QString::fromStdString(std::to_string(randId)));
}
