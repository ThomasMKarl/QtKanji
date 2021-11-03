#include "mainwindow.h"

QtKanji::MainWindow::MainWindow(QWidget *parent) : QDialog(parent)
{
  move(0, 0);

  QPalette pal = palette();
  pal.setColor(QPalette::Window, Qt::gray);
  setAutoFillBackground(true);
  setPalette(pal);

  tabWidget = std::make_unique<QTabWidget>();
  tabWidget->setTabsClosable(true);

  table = std::make_shared<Table>();
  table->show();

  printSignButton.setEnabled(false);
  hadamitzkyWindow.printSignButton = &printSignButton;
  tabHadWidget = std::make_unique<QTabWidget>();
  tabHadWidget->addTab(&hadamitzkyWindow, tr("Graphemes"));
  tabHadWidget->setCurrentIndex(tabHadWidget->count() - 1);
  layout.addWidget(tabHadWidget.get(), 1, 7, 10, 1);

  textfont.setPointSize(15);
  textfont.setBold(false);
  setFont(textfont);
  setWindowTitle("QtKanji Mainwindow");
  setWindowIcon(QIcon("kanji.ico"));

  addButtonsToLayout();
  addBoxesToLayout();
  addDisplaysToLayout();

  hideErrors();

  layout.addWidget(&cardboxes, 5, 0);
  layout.addWidget(&dataFail, 5, 1);
  layout.addWidget(&cardboxFail, 5, 1);
  layout.addWidget(&searchFail, 5, 1);
  layout.addWidget(&cardboxLimitFail, 5, 1);

  setLayout(&layout);
}

void QtKanji::MainWindow::addButtonsToLayout()
{
  QPalette pal = signButton.palette();
  pal.setColor(QPalette::Button, QColor(Qt::darkRed));

  signButton.setFixedSize(160, 30);
  signButton.setAutoFillBackground(true);
  signButton.setPalette(pal);
  connect(&signButton, &QPushButton::clicked, this, &MainWindow::signButtonClicked);
  layout.addWidget(&signButton, 4, 0);

  exampleButton.setFixedSize(160, 30);
  exampleButton.setAutoFillBackground(true);
  exampleButton.setPalette(pal);
  connect(&exampleButton, &QPushButton::clicked, this, &MainWindow::exampleButtonClicked);
  layout.addWidget(&exampleButton, 4, 1);

  wordButton.setFixedSize(160, 30);
  wordButton.setAutoFillBackground(true);
  wordButton.setPalette(pal);
  connect(&wordButton, &QPushButton::clicked, this, &MainWindow::exampleButtonClicked);
  layout.addWidget(&wordButton, 4, 2);

  for (unsigned int button = 0; button < NUMBER_OF_CARDBOX_BUTTONS; ++button)
  {
    if (button == 0 || button == 1)
      pal.setColor(QPalette::Button, QColor(Qt::red));
    if (button == 2 || button == 3)
      pal.setColor(QPalette::Button, QColor(Qt::darkYellow));
    if (button == 4)
      pal.setColor(QPalette::Button, QColor(Qt::darkGreen));

    cardboxKanjiButtons[button].setFixedSize(30, 30);
    cardboxKanjiButtons[button].setAutoFillBackground(true);
    cardboxKanjiButtons[button].setPalette(pal);
    cardboxKanjiButtons[button].setText(QString::number(button + 1));
    connect(&cardboxKanjiButtons[button], &QPushButton::clicked, this, &MainWindow::cardboxButtonClicked);
    layout.addWidget(&cardboxKanjiButtons[button], 6 + button, 0);

    cardboxExampleButtons[button].setText(QString::number(button + 1));
    cardboxExampleButtons[button].setAutoFillBackground(true);
    cardboxExampleButtons[button].setPalette(pal);
    cardboxExampleButtons[button].setFixedSize(30, 30);
    connect(&cardboxExampleButtons[button], &QPushButton::clicked, this, &MainWindow::cardboxButtonClicked);
    layout.addWidget(&cardboxExampleButtons[button], 6 + button, 1);

    cardboxWordButtons[button].setText(QString::number(button + 1));
    cardboxWordButtons[button].setAutoFillBackground(true);
    cardboxWordButtons[button].setPalette(pal);
    cardboxWordButtons[button].setFixedSize(30, 30);
    connect(&cardboxWordButtons[button], &QPushButton::clicked, this, &MainWindow::cardboxButtonClicked);
    layout.addWidget(&cardboxWordButtons[button], 6 + button, 2);
  }

  furikanjiButton.setFixedSize(160, 30);
  connect(&furikanjiButton, &QPushButton::clicked, this, &MainWindow::furikanjiButtonClicked);
  layout.addWidget(&furikanjiButton, 1, 0);
  furikanjiButton.setCheckable(true);
  furikanjiButton.setChecked(true);

  kanjifuriButton.setFixedSize(160, 30);
  connect(&kanjifuriButton, &QPushButton::clicked, this, &MainWindow::kanjifuriButtonClicked);
  layout.addWidget(&kanjifuriButton, 1, 1);
  kanjifuriButton.setCheckable(true);

  japengButton.setFixedSize(160, 30);
  connect(&japengButton, &QPushButton::clicked, this, &MainWindow::japengButtonClicked);
  layout.addWidget(&japengButton, 1, 2);
  japengButton.setCheckable(true);
  japengButton.setChecked(true);

  engjapButton.setFixedSize(160, 30);
  connect(&engjapButton, &QPushButton::clicked, this, &MainWindow::engjapButtonClicked);
  layout.addWidget(&engjapButton, 1, 3);
  engjapButton.setCheckable(true);

  pal.setColor(QPalette::Button, QColor(Qt::blue));
  printExampleButton.setFixedSize(190, 30);
  printExampleButton.setAutoFillBackground(true);
  printExampleButton.setPalette(pal);
  connect(&printExampleButton, &QPushButton::clicked, this, &MainWindow::printExampleButtonClicked);
  layout.addWidget(&printExampleButton, 3, 5);

  printSignButton.setFixedSize(190, 30);
  printSignButton.setAutoFillBackground(true);
  printSignButton.setPalette(pal);
  connect(&printSignButton, &QPushButton::clicked, this, &MainWindow::printSignButtonClicked);
  layout.addWidget(&printSignButton, 4, 5);

  pal.setColor(QPalette::Button, QColor(Qt::red));
  searchButton.setFixedSize(160, 30);
  searchButton.setAutoFillBackground(true);
  searchButton.setPalette(pal);
  connect(&searchButton, &QPushButton::clicked, this, &MainWindow::searchButtonClicked);
  layout.addWidget(&searchButton, 2, 5);
}

void QtKanji::MainWindow::addBoxesToLayout()
{
  std::shared_ptr<Boxes> boxes_ = std::make_shared<Boxes>();

  boxes_->imiBox = std::make_unique<QCheckBox>("Hide Imi", this);
  boxes_->imiBox->setChecked(boxes_->hideImi);
  connect(boxes_->imiBox.get(), &QCheckBox::clicked, this, &MainWindow::boxChecked);

  boxes_->kunBox = std::make_unique<QCheckBox>("Hide Kun");
  boxes_->kunBox->setChecked(boxes_->hideKun);
  connect(boxes_->kunBox.get(), &QCheckBox::clicked, this, &MainWindow::boxChecked);

  boxes_->onBox = std::make_unique<QCheckBox>("Hide On");
  boxes_->onBox->setChecked(boxes_->hideOn);
  connect(boxes_->onBox.get(), &QCheckBox::clicked, this, &MainWindow::boxChecked);

  boxes = std::move(boxes_);
  layout.addWidget(boxes->imiBox.get(), 2, 4);
  layout.addWidget(boxes->kunBox.get(), 3, 4);
  layout.addWidget(boxes->onBox.get(), 4, 4);

  randomizeBox.setChecked(randomize);
  connect(&randomizeBox, &QCheckBox::clicked, this, &MainWindow::randomizeChecked);
  layout.addWidget(&randomizeBox, 5, 3);

  randomizeBox.setChecked(kanji);
  connect(&kanjiBox, &QCheckBox::clicked, this, &MainWindow::kanjiChecked);
  layout.addWidget(&kanjiBox, 4, 3);
}

void QtKanji::MainWindow::addDisplaysToLayout()
{
  displayLowerLimit.setFixedSize(150, 30);
  displayLowerLimit.setText(QString::number(1));
  layout.addWidget(&lowerLimit, 2, 0);
  layout.addWidget(&displayLowerLimit, 2, 1);

  displayUpperLimit.setFixedSize(150, 30);
  displayUpperLimit.setText(QString::number(NUMBER_OF_KANJI));
  layout.addWidget(&upperLimit, 3, 0);
  layout.addWidget(&displayUpperLimit, 3, 1);

  displayLowerLection.setFixedSize(150, 30);
  displayLowerLection.setText(QString::number(1));
  layout.addWidget(&lowerLection, 2, 2);
  layout.addWidget(&displayLowerLection, 2, 3);

  displayUpperLection.setFixedSize(150, 30);
  displayUpperLection.setText(QString::number(25));
  layout.addWidget(&upperLection, 3, 2);
  layout.addWidget(&displayUpperLection, 3, 3);

  search.setFixedSize(50, 50);
  QFont searchTextfont{};
  searchTextfont.setPointSize(30);
  search.setFont(std::move(searchTextfont));
  layout.addWidget(&search, 1, 5);

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

  unsigned int lowerLimit = displayLowerLimit.text().toInt(); // characters become zero
  unsigned int upperLimit = displayUpperLimit.text().toInt();
  CHECK_ERROR(checkLimits(lowerLimit, upperLimit, NUMBER_OF_KANJI));

  unsigned int lowerLection = displayLowerLection.text().toInt(); // characters become zero
  unsigned int upperLection = displayUpperLection.text().toInt();
  CHECK_ERROR(checkLimits(lowerLimit, upperLimit, 25));

  DataHandler dataHandler{lowerLimit, upperLimit, lowerLection, upperLection, fromFuriToKanji, fromJapToEng};

  CHECK_ERROR(dataHandler.computeExampleData(randomize));

  ExampleWindow *example = ExampleWindow::createExampleWindow(dataHandler);

  example->show();
}

void QtKanji::MainWindow::printExampleButtonClicked()
{
  hideErrors();

  unsigned int lowerLimit = displayLowerLimit.text().toInt(); // characters become zero
  unsigned int upperLimit = displayUpperLimit.text().toInt();
  CHECK_ERROR(checkLimits(lowerLimit, upperLimit, NUMBER_OF_KANJI));

  unsigned int lowerLection = displayLowerLection.text().toInt(); // characters become zero
  unsigned int upperLection = displayUpperLection.text().toInt();
  CHECK_ERROR(checkLimits(lowerLimit, upperLimit, 25));

  DataHandler dataHandler{lowerLimit, upperLimit, lowerLection, upperLection, fromFuriToKanji, fromJapToEng};

  CHECK_ERROR(dataHandler.computeExampleData(randomize));

  CHECK_ERROR(dataHandler.printExamples());
}

void QtKanji::MainWindow::printSignButtonClicked()
{
  hideErrors();

  unsigned int lowerLimit = displayLowerLimit.text().toInt(); // characters become zero
  unsigned int upperLimit = displayUpperLimit.text().toInt();
  CHECK_ERROR(checkLimits(lowerLimit, upperLimit, NUMBER_OF_KANJI));

  printSignButton.setText("print kanji sorted");
  CHECK_ERROR(hadamitzkyWindow.printSigns(lowerLimit, upperLimit));
}

void QtKanji::MainWindow::searchButtonClicked()
{
  hideErrors();

  DataHandler dataHandler{};
  QString kanji = search.text();
  unsigned int Id = dataHandler.searchKanjiId(kanji);

  if (Id == 0)
  {
    displayErrorMessage(QtKanji::Error::FILE_ERROR);
    return;
  }

  startFlashcardWindow(Id);
}

void QtKanji::MainWindow::boxChecked()
{
  if (boxes->kunBox->isChecked() && boxes->onBox->isChecked())
    boxes->imiBox->setChecked(false);

  if (boxes->imiBox->isChecked())
    boxes->hideImi = true;
  else
    boxes->hideImi = false;

  if (boxes->imiBox->isChecked() && boxes->onBox->isChecked())
    boxes->kunBox->setChecked(false);

  if (boxes->kunBox->isChecked())
    boxes->hideKun = true;
  else
    boxes->hideKun = false;

  if (boxes->imiBox->isChecked() && boxes->kunBox->isChecked())
    boxes->onBox->setChecked(false);

  if (boxes->onBox->isChecked())
    boxes->hideOn = true;
  else
    boxes->hideOn = false;
}

void QtKanji::MainWindow::randomizeChecked()
{
  randomize = !randomize;
}

void QtKanji::MainWindow::kanjiChecked()
{
  kanji = !kanji;
}

void QtKanji::MainWindow::furikanjiButtonClicked()
{
  if (furikanjiButton.isChecked())
  {
    fromFuriToKanji = true;
    kanjifuriButton.setChecked(false);
  }
  else
  {
    kanjifuriButton.setChecked(true);
    kanjifuriButtonClicked();
  }
}

void QtKanji::MainWindow::kanjifuriButtonClicked()
{
  if (kanjifuriButton.isChecked())
  {
    fromFuriToKanji = false;
    furikanjiButton.setChecked(false);
  }
  else
  {
    furikanjiButton.setChecked(true);
    furikanjiButtonClicked();
  }
}

void QtKanji::MainWindow::japengButtonClicked()
{
  if (japengButton.isChecked())
  {
    fromJapToEng = true;
    engjapButton.setChecked(false);
  }
  else
  {
    engjapButton.setChecked(true);
    engjapButtonClicked();
  }
}

void QtKanji::MainWindow::engjapButtonClicked()
{
  if (engjapButton.isChecked())
  {
    fromJapToEng = false;
    japengButton.setChecked(false);
  }
  else
  {
    japengButton.setChecked(true);
    japengButtonClicked();
  }
}

void QtKanji::MainWindow::startFlashcardWindow(bool fromCardbox)
{
  hideErrors();

  unsigned int lowerLimit = displayLowerLimit.text().toInt(); // characters become zero
  unsigned int upperLimit = displayUpperLimit.text().toInt();
  CHECK_ERROR(checkLimits(lowerLimit, upperLimit, NUMBER_OF_KANJI));

  unsigned int lowerLection = displayLowerLection.text().toInt(); // characters become zero
  unsigned int upperLection = displayUpperLection.text().toInt();
  CHECK_ERROR(checkLimits(lowerLimit, upperLimit, 25));

  DataHandler dataHandler{lowerLimit, upperLimit, lowerLection, upperLection, fromFuriToKanji, fromJapToEng};

  if (fromCardbox)
  {
    CHECK_ERROR(dataHandler.computeKanjiCardboxData());
    tabWidget->addTab(FlashcardWindow::createFlashcardWindow(randomize, fromCardbox, dataHandler, boxes, table),
                      tr("Cardbox"));
  }
  else
  {
    dataHandler.computeKanjiCardboxData();
    tabWidget->addTab(FlashcardWindow::createFlashcardWindow(randomize, fromCardbox, dataHandler, boxes, table),
                      tr("Flashcard"));
  }

  tabWidget->setCurrentIndex(tabWidget->count() - 1);
  layout.addWidget(tabWidget.get(), 11, 0, 1, 10);
}

void QtKanji::MainWindow::startFlashcardWindow(unsigned int ID)
{
  hideErrors();

  tabWidget->addTab(FlashcardWindow::createFlashcardWindow(ID), tr("Search"));
  tabWidget->setCurrentIndex(tabWidget->count() - 1);
  layout.addWidget(tabWidget.get(), 11, 0, 1, 10);
}

QtKanji::Error QtKanji::MainWindow::checkLimits(unsigned int lowerLimit, unsigned int upperLimit, unsigned int max)
{
  if (upperLimit > max)
  {
    upperLimit = max;
    displayUpperLimit.setText(QString::number(max));
  }

  if (upperLimit < lowerLimit)
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

  switch (err)
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
