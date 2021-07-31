#include "examplewindow.h"

QtKanji::Example::Example(DataHandler &dataHandler_,
		          unsigned int successes_,
		          unsigned int failures_,
		          QWidget *parent) : QWidget(parent)
{
  successes = successes_;
  failures = failures_;
  dataHandler = std::make_shared<DataHandler>(dataHandler_);
  layout = std::make_unique<QGridLayout>();
 
  QFont textfont{};
  textfont.setPointSize(20);
  textfont.setBold(false);
  setFont(std::move(textfont));

  setWindowTitle("QtKanji example #"
		 + QString::number(dataHandler->indexContainer.size()+1)
		 + " of "
		 + QString::number(dataHandler->dataFurigana.size()));
  setWindowIcon(QIcon("kanji.ico"));


  submitButton = std::make_unique<QPushButton>("submit");
  connect(submitButton.get(),
	  &QPushButton::clicked,
	  this,
	  &Example::submitButtonClicked);
  layout->addWidget(submitButton.get(),1,2);

////////////////////////////////////////////////////////////////////////////////////////////////

  randId = rand()%dataHandler->dataFurigana.size();
  if(dataHandler->indexContainer.size() == dataHandler->dataFurigana.size())
  {
    showResult(*this);
    return;
  }

  while(contains(dataHandler->indexContainer, randId)
	&& dataHandler->indexContainer.size()
	!= dataHandler->dataFurigana.size())
    randId = rand()%dataHandler->dataFurigana.size();

  dataHandler->indexContainer.push_back(randId);
  
  if(dataHandler->fromEngToJap)
  {
    displayFurigana = std::make_unique<QLineEdit>(this);
    Furigana = std::make_unique<QLabel>("Furigana:", this);

    layout->addWidget(displayFurigana.get(),1,1);

    Kanji  = std::make_unique<QLabel>
      (QString::fromStdString("Kanji: "), this);
    Kanji2 = std::make_unique<QLabel>
      (QString::fromStdString(dataHandler->dataKanji[randId]), this);
    layout->addWidget(Kanji2.get(),2,1);
  }
  else
  {
    Furigana  = std::make_unique<QLabel>
      (QString::fromStdString("Furigana: "), this);
    Furigana2 = std::make_unique<QLabel>
      (QString::fromStdString(dataHandler->dataFurigana[randId]), this);
    layout->addWidget(Furigana2.get(),1,1);

    displayKanji = std::make_unique<QLineEdit>(this);
    Kanji = std::make_unique<QLabel>("Kanji:", this);

    layout->addWidget(displayKanji.get(),2,1);
  }

  English  = std::make_unique<QLabel>
    (QString::fromStdString("Translation: "), this);
  English2 = std::make_unique<QLabel>
    (QString::fromStdString(dataHandler->dataEnglish[randId]), this);

  Success = std::make_unique<QLabel>("Success!", this);
  Failure = std::make_unique<QLabel>("Failure!", this);
  layout->addWidget(Success.get(),4,0);
  layout->addWidget(Failure.get(),4,0);
  Success->hide();
  Success->setStyleSheet("color: green");
  Failure->hide();
  Failure->setStyleSheet("color: red");

  continueButton = std::make_unique<QPushButton>("continue");
  connect(continueButton.get(),
	  &QPushButton::clicked,
	  this,
	  &Example::continueButtonClicked);
  layout->addWidget(continueButton.get(),1,2);
  continueButton->hide();

  layout->addWidget(Furigana.get(),1,0);
  layout->addWidget(Kanji.get(),2,0);
  layout->addWidget(English.get(),3,0);
  layout->addWidget(English2.get(),3,1);

  setLayout(layout.get());
}

void QtKanji::Example::submitButtonClicked()
{
  if(dataHandler->fromEngToJap)
  {
    dataHandler->dataFurigana[randId] =
      displayFurigana->text().toStdString();

    if(dataHandler->dataFurigana[randId]
       == dataHandler->truedataFurigana[randId]) showSuccess();
    else showFailure();
  }
  else
  {
    dataHandler->dataKanji[randId] =
      displayKanji->text().toStdString();

    if(dataHandler->dataKanji[randId]
       == dataHandler->truedataKanji[randId]) showSuccess();
    else showFailure();
  }
}

void QtKanji::Example::continueButtonClicked()
{
    Example *ex = new Example(*dataHandler.get(), failures, successes);
    this->close();
    ex->show();
}

void QtKanji::Example::showSuccess()
{
  Success       ->show();
  Failure       ->hide();
  continueButton->show();
  if(dataHandler->fromEngToJap)
  {
    displayFurigana->
      setText(QString::fromStdString(dataHandler->truedataFurigana[randId]));

    displayFurigana->setStyleSheet("color: green");
  }
  else
  {
    displayKanji->
      setText(QString::fromStdString(dataHandler->truedataKanji[randId]));

    displayKanji->setStyleSheet("color: green");
  }
 
  ++successes;

  submitButton->hide();
}

void QtKanji::Example::showFailure()
{
  Success       ->hide();
  Failure       ->show();
  continueButton->show();

  if(dataHandler->fromEngToJap)
  {
    displayFurigana->
      setText(QString::fromStdString(dataHandler->truedataFurigana[randId]));
 
    displayFurigana->setStyleSheet("color: red");
  }
  else
  {
    displayKanji->
      setText(QString::fromStdString(dataHandler->truedataKanji[randId]));

    displayKanji->setStyleSheet("color: red");
  }
 
  ++failures;

  submitButton->hide();
}
