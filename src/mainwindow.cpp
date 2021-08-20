#include "mainwindow.h"

QtKanji::MainWindow::MainWindow(QWidget *parent): QWidget(parent)
{
  move(0,0);

  table = std::make_shared<Table>();
  table->show();
  hadamitzkyWindow.show();
  std::vector<std::map<unsigned int, unsigned int>> radicalStrokeNumberMaps{};
  hadamitzkyWindow.radicalKanjiMap = computeRadicalKanjiMap(radicalStrokeNumberMaps);
  hadamitzkyWindow.radicalStrokeNumberMaps = std::move(radicalStrokeNumberMaps);
  
  textfont.setPointSize(15);
  textfont.setBold(false);
  setFont(textfont);
  setWindowTitle("QtKanji Mainwindow");
  setWindowIcon(QIcon("kanji.ico"));

  addButtonsToLayout();
  addBoxesToLayout();
  addDisplaysToLayout();

  hideErrors();

  layout.addWidget(&dataFail,5,1);
  layout.addWidget(&cardboxFail,5,1);
  layout.addWidget(&searchFail,5,1);
  layout.addWidget(&cardboxLimitFail,5,1);

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

  printExampleButton.setFixedSize(190,30);
  connect(&printExampleButton,
    &QPushButton::clicked,
	  this,
	  &MainWindow::printExampleButtonClicked);
  layout.addWidget(&printExampleButton,3,3);

  printSignButton.setFixedSize(190,30);
  connect(&printSignButton,
    &QPushButton::clicked,
	  this,
	  &MainWindow::printSignButtonClicked);
  layout.addWidget(&printSignButton,4,3);

  searchButton.setFixedSize(150,30);
  connect(&searchButton,
    &QPushButton::clicked,
	  this,
	  &MainWindow::searchButtonClicked);
  layout.addWidget(&searchButton,6,2);
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
  displayLowerLimit.setText(QString::number(1));
  layout.addWidget(&lowerLimit,2,0);
  layout.addWidget(&displayLowerLimit,2,1);
     
  displayUpperLimit.setFixedSize(150,30);
  displayUpperLimit.setText(QString::number(NUMBER_OF_KANJI));
  layout.addWidget(&upperLimit,3,0);
  layout.addWidget(&displayUpperLimit,3,1);

  search.setFixedSize(50,50);
  QFont searchTextfont{};
  searchTextfont.setPointSize(30);
  search.setFont(std::move(searchTextfont));
  layout.addWidget(&search,5,2);

  table->search = &search;
  hadamitzkyWindow.search = &search;
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
  hideErrors();
 
  unsigned int lowerLimit = displayLowerLimit.text().toInt(); //characters become zero
  unsigned int upperLimit = displayUpperLimit.text().toInt();
  CHECK_ERROR(checkLimits(lowerLimit, upperLimit));

  DataHandler dataHandler{lowerLimit, upperLimit, fromEngToJap}; 

  CHECK_ERROR(dataHandler.computeExampleData(randomize));

  ExampleWindow *example = ExampleWindow::createExampleWindow(dataHandler);

  example->show();
}

void QtKanji::MainWindow::printExampleButtonClicked()
{
  hideErrors();

  unsigned int lowerLimit = displayLowerLimit.text().toInt(); //characters become zero
  unsigned int upperLimit = displayUpperLimit.text().toInt();
  CHECK_ERROR(checkLimits(lowerLimit, upperLimit));

  DataHandler dataHandler{lowerLimit, upperLimit, fromEngToJap}; 

  CHECK_ERROR(dataHandler.computeExampleData(randomize));

  CHECK_ERROR(dataHandler.printExamples());
}

void QtKanji::MainWindow::printSignButtonClicked()
{
  hideErrors();

  unsigned int lowerLimit = displayLowerLimit.text().toInt(); //characters become zero
  unsigned int upperLimit = displayUpperLimit.text().toInt();
  CHECK_ERROR(checkLimits(lowerLimit, upperLimit));

  CHECK_ERROR(hadamitzkyWindow.printSigns(lowerLimit, upperLimit));
}

void QtKanji::MainWindow::searchButtonClicked()
{
  hideErrors();
  
  DataHandler dataHandler{};
  QString kanji = search.text();
  unsigned int Id = dataHandler.searchKanjiId(kanji);

  if(Id == 0)
  {
    displayErrorMessage(QtKanji::Error::FILE_ERROR);
    return;
  }

  startFlashcardWindow(Id);
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
    fromEngToJap = true;
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
    fromEngToJap = false;
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
  hideErrors();
  
  unsigned int lowerLimit = displayLowerLimit.text().toInt(); //characters become zero
  unsigned int upperLimit = displayUpperLimit.text().toInt();
  CHECK_ERROR(checkLimits(lowerLimit, upperLimit));

  DataHandler dataHandler{lowerLimit, upperLimit, fromEngToJap};

  if(fromCardbox) CHECK_ERROR(dataHandler.computeContainerData());
  else dataHandler.computeContainerData();
  
  FlashcardWindow *flashcard =
    FlashcardWindow::createFlashcardWindow(randomize,
                                           fromCardbox,
                                           dataHandler,
				                                   boxes,
				                                   table);
  flashcard->show();
}

void QtKanji::MainWindow::startFlashcardWindow(unsigned int ID)
{
  hideErrors();
  
  FlashcardWindow *flashcard =
    FlashcardWindow::createFlashcardWindow(ID);
  flashcard->show();
}

QtKanji::Error QtKanji::MainWindow::checkLimits(unsigned int lowerLimit, unsigned int upperLimit)
{
  if(upperLimit > NUMBER_OF_KANJI)
  {
    upperLimit = NUMBER_OF_KANJI;
    displayUpperLimit.setText(QString::number(NUMBER_OF_KANJI));
  }
    
  if(upperLimit < lowerLimit)
  {
    displayLowerLimit.setText("insert integers.");
    displayUpperLimit.setText("");
    return QtKanji::Error::FILE_ERROR;
  }
  
  return QtKanji::Error::SUCCESS;
}

void QtKanji::MainWindow::displayErrorMessage(QtKanji::Error err)
{
  hideErrors();

  switch(err)
  {
   case QtKanji::Error::FILE_ERROR:
    dataFail.show();
    return;
   case QtKanji::Error::EMPTY_CARDBOX:
    cardboxFail.show();
    return;
   case QtKanji::Error::KANJI_NOT_FOUND:
    searchFail.show();
    return;
   case QtKanji::Error::NO_KANJI_WITHIN_RANGE:
    cardboxLimitFail.show();
    return;
   default:
    return;
  }
}

void QtKanji::MainWindow::hideErrors()
{
  dataFail.hide();
  cardboxFail.hide();
  searchFail.hide();
  cardboxLimitFail.hide();
}

QtKanji::MainWindow::~MainWindow()
{
  table->search = nullptr;
  hadamitzkyWindow.search = nullptr;
}