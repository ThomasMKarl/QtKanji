#include "mainwindow.h"
#include <iostream>

QtKanji::MainWindow::MainWindow(QWidget *parent): QWidget(parent)
{
  move(0,0);
  
  //hadamitzkyWindow.show();
  table = std::make_shared<Table>();
  table->show();
  dataHandler = std::make_shared<DataHandler>();
  //hadamitzkyWindow.radicalKanjiMap =
  //std::vector<QtKanji::Uints> x = computeRadicalKanjiMap(dataHandler);
  //std::cout << x[0][0] << std::endl;
  //std::cout << "blub" << std::endl;
  QFont textfont{};
  textfont.setPointSize(15);
  textfont.setBold(false);
  setFont(std::move(textfont));
  setWindowTitle("QtKanji Mainwindow");
  setWindowIcon(QIcon("kanji.ico"));

  addButtonsToLayout();
  addBoxesToLayout();
  addDisplaysToLayout();

  dataFail.hide();
  layout.addWidget(&dataFail,5,1);
  
  cardboxFail.hide();
  layout.addWidget(&cardboxFail,5,1);

  setLayout(&layout);
}

void QtKanji::MainWindow::addButtonsToLayout()
{
  signButton.setFixedSize(150,30);
  connect(&signButton,
	  &QPushButton::clicked,
	  this,
	  &MainWindow::signButtonClicked);
  layout.addWidget(&signButton,4,0);

  cardboxButton.setFixedSize(150,30);
  connect(&cardboxButton,
	  &QPushButton::clicked,
	  this,
	  &MainWindow::cardboxButtonClicked);
  layout.addWidget(&cardboxButton,5,0);
	
  exampleButton.setFixedSize(150,30);
  connect(&exampleButton,
          &QPushButton::clicked,
	  this,
	  &MainWindow::exampleButtonClicked);
  layout.addWidget(&exampleButton,4,1);

  engjapButton.setFixedSize(150,30);
  connect(&engjapButton,
          &QPushButton::clicked,
	  this,
	  &MainWindow::engjapButtonClicked);
  layout.addWidget(&engjapButton,1,0);
  engjapButton.setCheckable(true);
  engjapButton.setChecked(true);
  
  japengButton.setFixedSize(150,30);
  connect(&japengButton,
          &QPushButton::clicked,
	  this,
	  &MainWindow::japengButtonClicked);
  layout.addWidget(&japengButton,1,1);
  japengButton.setCheckable(true);

  printButton.setFixedSize(190,30);
  connect(&printButton,
          &QPushButton::clicked,
	  this,
	  &MainWindow::printButtonClicked);
  layout.addWidget(&printButton,3,3);

  searchButton.setFixedSize(150,30);
  connect(&searchButton,
          &QPushButton::clicked,
	  this,
	  &MainWindow::searchButtonClicked);
  layout.addWidget(&searchButton,5,3);
}

void QtKanji::MainWindow::addBoxesToLayout()
{
  std::shared_ptr<Boxes> boxes_ = std::make_shared<Boxes>();

  boxes_->imiBox = std::make_unique<QCheckBox>("Hide Imi", this);
  boxes_->imiBox->setChecked(boxes_->hideImi);
  connect(boxes_->imiBox.get(),
	  &QCheckBox::clicked,
	  this,
	  &MainWindow::boxChecked);

  boxes_->kunBox = std::make_unique<QCheckBox>("Hide Kun");
  boxes_->kunBox->setChecked(boxes_->hideKun);
  connect(boxes_->kunBox.get(),
          &QCheckBox::clicked,
	  this,
	  &MainWindow::boxChecked);

  boxes_->onBox  = std::make_unique<QCheckBox>("Hide On");
  boxes_->onBox->setChecked(boxes_->hideOn);
  connect(boxes_->onBox.get(),
          &QCheckBox::clicked,
	  this,
	  &MainWindow::boxChecked);

  boxes = std::move(boxes_);
  layout.addWidget(boxes->imiBox.get(), 2,2);
  layout.addWidget(boxes->kunBox.get(), 3,2);
  layout.addWidget(boxes-> onBox.get(), 4,2);

  randomizeBox.setChecked(randomize);
  connect(&randomizeBox,
          &QCheckBox::clicked,
	  this,
	  &MainWindow::randomizeChecked);
  layout.addWidget(&randomizeBox,2,3);
}

void QtKanji::MainWindow::addDisplaysToLayout()
{
  displayLowerLimit.setFixedSize(150,30);
  displayLowerLimit.setText(QString::number(dataHandler->lowerLimit));
  layout.addWidget(&lowerLimit,2,0);
  layout.addWidget(&displayLowerLimit,2,1);
     
  displayUpperLimit.setFixedSize(150,30);
  displayUpperLimit.setText(QString::number(dataHandler->upperLimit));
  layout.addWidget(&upperLimit,3,0);
  layout.addWidget(&displayUpperLimit,3,1);

  search.setFixedSize(50,50);
  QFont textfont{};
  textfont.setPointSize(30);
  search.setFont(std::move(textfont));
  layout.addWidget(&search,4,3);
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

void QtKanji::MainWindow::searchButtonClicked()
{
  searchKanji();
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
  if(engjapButton.isChecked())
  {
    dataHandler->fromEngToJap = true;
    japengButton.setChecked(false);
  }
  else
  {
    japengButton.setChecked(true);
    japengButtonClicked();
  }
}

void QtKanji::MainWindow::japengButtonClicked()
{
  if(japengButton.isChecked())
  {
    dataHandler->fromEngToJap = false;
    engjapButton.setChecked(false);
  }
  else
  {
    engjapButton.setChecked(true);
    engjapButtonClicked();
  }
}

void QtKanji::MainWindow::startFlashcardWindow(bool fromCardbox)
{
  if(!dataHandler->getLimits(*this)) return;
  if(!dataHandler->computeContainerData(*this) && fromCardbox) return;
  cardboxFail.hide();
  dataHandler->indexContainer.clear();
  
  unsigned int successes{0}, failures{0};
  int removeFlag{0};
  FlashcardWindow *flashcard =
    FlashcardWindow::createFlashcardWindow(dataHandler,
				           boxes,
				           table,
				           fromCardbox, removeFlag,
				           successes, failures);
  flashcard->show();
}

void QtKanji::MainWindow::startFlashcardWindow(unsigned int ID)
{
  dataHandler->indexContainer.clear();
    
  unsigned int successes{0}, failures{0};
  int removeFlag{0};
  FlashcardWindow *flashcard =
    FlashcardWindow::createFlashcardWindow(dataHandler,
				           table,
				           ID);
  flashcard->show();
}

void QtKanji::MainWindow::startExampleWindow()
{
  if(!dataHandler->getLimits(*this)) return;
  if(!dataHandler->computeExampleData(*this)) return;

  unsigned int successes{0}, failures{0};
  ExampleWindow *example = ExampleWindow::createExampleWindow(dataHandler, failures, successes);

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
    dataFail.show();
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

void QtKanji::MainWindow::searchKanji()
{
  cardboxFail.hide();
  
  QString kanji = search.text();
  const auto index = std::find(kanjiList.begin(), kanjiList.end(), kanji);
  if(index == kanjiList.end())
  {
    dataFail.show();
    return;
  }
  dataFail.hide();
  
  unsigned int Id = std::distance(kanjiList.begin(),index)+1;

  startFlashcardWindow(Id);
}
