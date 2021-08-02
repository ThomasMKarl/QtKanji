#include "mainwindow.h"

QtKanji::MainWindow::MainWindow(QWidget *parent): QWidget(parent)
{
  layout = std::make_unique<QGridLayout>();
  dataHandler = std::make_shared<DataHandler>();

  QFont textfont{};
  textfont.setPointSize(15);
  textfont.setBold(false);
  setFont(std::move(textfont));
  setWindowTitle("QtKanji Mainwindow");
  setWindowIcon(QIcon("kanji.ico"));
  

  signButton = std::make_unique<QPushButton>("train kanji");
  signButton->setFixedSize(150,30);
  connect(signButton.get(),
	  &QPushButton::clicked,
	  this,
	  &MainWindow::signButtonClicked);
  layout->addWidget(signButton.get(),4,0);

  cardboxButton = std::make_unique<QPushButton>("cardbox");
  cardboxButton->setFixedSize(150,30);
  connect(cardboxButton.get(),
	  &QPushButton::clicked,
	  this,
	  &MainWindow::cardboxButtonClicked);
  layout->addWidget(cardboxButton.get(),5,0);
	
  exampleButton = std::make_unique<QPushButton>("train words");
  exampleButton->setFixedSize(150,30);
  connect(exampleButton.get(),
          &QPushButton::clicked,
	  this,
	  &MainWindow::exampleButtonClicked);
  layout->addWidget(exampleButton.get(),4,1);
	
  engjapButton = std::make_unique<QPushButton>("ふりがな - 漢字");
  engjapButton->setFixedSize(150,30);
  connect(engjapButton.get(),
          &QPushButton::clicked,
	  this,
	  &MainWindow::engjapButtonClicked);
  layout->addWidget(engjapButton.get(),1,0);
  engjapButton->setCheckable(true);
  engjapButton->setChecked(true);
  
  japengButton = std::make_unique<QPushButton>("漢字 - ふりがな");
  japengButton->setFixedSize(150,30);
  connect(japengButton.get(),
          &QPushButton::clicked,
	  this,
	  &MainWindow::japengButtonClicked);
  layout->addWidget(japengButton.get(),1,1);
  japengButton->setCheckable(true);

  printButton = std::make_unique<QPushButton>("print examples");
  printButton->setFixedSize(170,30);
  connect(printButton.get(),
          &QPushButton::clicked,
	  this,
	  &MainWindow::printButtonClicked);
  layout->addWidget(printButton.get(),3,3);


  std::unique_ptr<Boxes> boxes_ = std::make_unique<Boxes>();

  boxes_->imiBox = std::make_unique<QCheckBox>("Hide Imi", this);
  boxes_->imiBox->setChecked(boxes_->hideImi);
  connect(boxes_->imiBox.get(),
	  &QCheckBox::clicked,
	  this,
	  &MainWindow::boxChecked);

  boxes_->kunBox = std::make_unique<QCheckBox>("Hide Kun", this);
  boxes_->kunBox->setChecked(boxes_->hideKun);
  connect(boxes_->kunBox.get(),
          &QCheckBox::clicked,
	  this,
	  &MainWindow::boxChecked);

  boxes_->onBox  = std::make_unique<QCheckBox>("Hide On" , this);
  boxes_->onBox->setChecked(boxes_->hideOn);
  connect(boxes_->onBox.get(),
          &QCheckBox::clicked,
	  this,
	  &MainWindow::boxChecked);

  boxes = std::move(boxes_);
  layout->addWidget(boxes->imiBox.get(),2,2);
  layout->addWidget(boxes->kunBox.get(),3,2);
  layout->addWidget(boxes->onBox.get(), 4,2);

  randomizeBox = std::make_unique<QCheckBox>("randomize", this);
  randomizeBox->setChecked(randomize);
  connect(randomizeBox.get(),
          &QCheckBox::clicked,
	  this,
	  &MainWindow::randomizeChecked);
  layout->addWidget(randomizeBox.get(),2,3);
		
  displayLowerLimit = std::make_unique<QLineEdit>(this);
  displayLowerLimit->setFixedSize(150,30);
  lowerLimit = std::make_unique<QLabel>("from:", this);
  displayLowerLimit->setText("317");
  layout->addWidget(lowerLimit.get(),2,0);
  layout->addWidget(displayLowerLimit.get(),2,1);
     
  displayUpperLimit = std::make_unique<QLineEdit>(this);
  displayUpperLimit->setFixedSize(150,30);
  upperLimit = std::make_unique<QLabel>("to:", this);
  displayUpperLimit->setText("460");
  layout->addWidget(upperLimit.get(),3,0);
  layout->addWidget(displayUpperLimit.get(),3,1);

  dataFail = std::make_unique<QLabel>("file error!", this);
  dataFail->hide();
  layout->addWidget(dataFail.get(),5,1);
  
  cardboxFail = std::make_unique<QLabel>("no cards in box.", this);
  cardboxFail->hide();
  layout->addWidget(cardboxFail.get(),5,1);
  
  
  setLayout(std::move(layout).get());
}

void QtKanji::MainWindow::signButtonClicked()
{
  bool fromCardbox{false};
  startFlashcardWindow(fromCardbox);
}

void QtKanji::MainWindow::cardboxButtonClicked()
{
  bool fromCardbox{true};
  startFlashcardWindow(fromCardbox);
}

void QtKanji::MainWindow::exampleButtonClicked()
{
  startExampleWindow();
}

void QtKanji::MainWindow::printButtonClicked()
{
  printExamples();
}

void QtKanji::MainWindow::boxChecked()
{
  if(boxes->kunBox->isChecked() &&  boxes->onBox->isChecked())
    boxes->imiBox->setChecked(false);

  if(boxes->imiBox->isChecked())
    boxes->hideImi = true;
  else
    boxes->hideImi = false;

  if(boxes->imiBox->isChecked() &&  boxes->onBox->isChecked())
    boxes->kunBox->setChecked(false);
    
  if(boxes->kunBox->isChecked())
    boxes->hideKun = true;
  else
    boxes->hideKun = false;

  if(boxes->imiBox->isChecked() && boxes->kunBox->isChecked())
    boxes->onBox->setChecked(false);
    
  if( boxes->onBox->isChecked())
    boxes->hideOn  = true;
  else
    boxes->hideOn  = false;
}

void QtKanji::MainWindow::randomizeChecked()
{
  randomize = !randomize;
}

void QtKanji::MainWindow::engjapButtonClicked()
{
  if(engjapButton->isChecked())
  {
    dataHandler->fromEngToJap = true;
    japengButton->setChecked(false);
  }
  else
  {
    japengButton->setChecked(true);
    japengButtonClicked();
  }
}

void QtKanji::MainWindow::japengButtonClicked()
{
  if(japengButton->isChecked())
  {
    dataHandler->fromEngToJap = false;
    engjapButton->setChecked(false);
  }
  else
  {
    engjapButton->setChecked(true);
    engjapButtonClicked();
  }
}

void QtKanji::MainWindow::startFlashcardWindow(bool fromCardbox)
{   
  if(!dataHandler->getLimits(*this)) return;
  if(!dataHandler->computeContainerData(*this, fromCardbox)) return;
  dataHandler->indexContainer.clear();
  
  unsigned int successes{0}, failures{0};
  int removeFlag{0};
  Flashcard *flashcard = Flashcard::createFlashcardWindow(*dataHandler.get(),
			                                  successes, failures,
			                                  fromCardbox, removeFlag,
			                                  *boxes.get());

  Table *table = Table::createTable();

  table->show();
  flashcard->show();
}

void QtKanji::MainWindow::startExampleWindow()
{
  if(!dataHandler->getLimits(*this)) return;
  if(!dataHandler->computeExampleData(*this)) return;

  unsigned int successes{0}, failures{0};
  Example *example = Example::createCardboxWindow(*dataHandler.get(), failures, successes);

  example->show();
}

void QtKanji::MainWindow::printExamples()
{
  if(!dataHandler->getLimits(*this)) return;
  if(!dataHandler->computeExampleData(*this)) return;

  std::ofstream kanjiFile{"examples_kanji.html"};
  std::ofstream furiganaFile{"examples_furigana.html"};
  if(!kanjiFile || !furiganaFile)
  {
    dataFail->show();
    return;
  }

  const std::string header{"<!DOCTYPE html>\n<html>\n<head>\n<title>Kanji Examples</title>\n<meta charset='UTF-8'>\n</head>\n\n<body>\n<h1>Kanji Examples</h1>\n"};
  const std::string footer{"\n</body>\n</html>"};
  
  kanjiFile << header;
  furiganaFile << header;
  
  size_t counter{0};
  for(const std::string &kanji    : dataHandler->dataKanji)
  {
    if(counter%10 == 0) kanjiFile << "<p>\n" << counter/10+1 << ".  ";
    kanjiFile << kanji << "  +  ";
    ++counter;
  }

  counter = 0;
  for(const std::string &furigana : dataHandler->dataFurigana)
  {
    if(counter%10 == 0) furiganaFile << "<p>\n" << counter/10+1 << ".  ";
    furiganaFile << furigana << "  +  ";
    ++counter;
  }

  kanjiFile << footer;
  furiganaFile << footer;
}
