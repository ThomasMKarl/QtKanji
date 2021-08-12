#include "examplewindow.h"

QtKanji::ExampleWindow::ExampleWindow(SharedData &dataHandler_,
		                      unsigned int successes_,
		                      unsigned int failures_,
		                      QWidget *parent) :
  dataHandler{dataHandler_},
  successes{successes_},
  failures{failures_},
  QWidget(parent)
{
  randId = failures + successes;
  unsigned int numberOfExamples = dataHandler->dataFurigana.size();
  
  if(randId == numberOfExamples)
  {
    showResult(*this);
    return;
  }
 
  QFont textfont{};
  textfont.setPointSize(20);
  textfont.setBold(false);
  setFont(std::move(textfont));

  setWindowTitle("QtKanji example #"
		 + QString::number(randId+1)
		 + " of "
		 + QString::number(numberOfExamples));
  setWindowIcon(QIcon("kanji.ico"));
  
////////////////////////////////////////////////////////////////////////////////////////////////

  connect(&submitButton,
	  &QPushButton::clicked,
	  this,
	  &ExampleWindow::submitButtonClicked);
  layout.addWidget(&submitButton,1,2);

  connect(&continueButton,
	  &QPushButton::clicked,
	  this,
	  &ExampleWindow::continueButtonClicked);
  layout.addWidget(&continueButton,1,2);
  continueButton.hide();

////////////////////////////////////////////////////////////////////////////////////////////////
  
  layout.addWidget(&Furigana, 1,0);
  layout.addWidget(&Kanji, 2,0);
  
  if(dataHandler->fromEngToJap)
  {
    layout.addWidget(&displayFurigana,1,1);

    Kanji2.setText(QString::fromStdString(dataHandler->dataKanji[randId]));
    layout.addWidget(&Kanji2,2,1);
  }
  else
  {
    Furigana2.setText(QString::fromStdString(dataHandler->dataFurigana[randId]));
    layout.addWidget(&Furigana2,1,1);

    layout.addWidget(&displayKanji,2,1);
  }

  English2.setText(QString::fromStdString(dataHandler->dataEnglish[randId]));
  layout.addWidget(&English, 3,0);
  layout.addWidget(&English2,3,1);
  
////////////////////////////////////////////////////////////////////////////////////////////////
  
  layout.addWidget(&Success,4,0);
  layout.addWidget(&Failure,4,0);
  Success.hide();
  Success.setStyleSheet("color: green");
  Failure.hide();
  Failure.setStyleSheet("color: red");
  
////////////////////////////////////////////////////////////////////////////////////////////////

  setLayout(&layout);
}

void QtKanji::ExampleWindow::submitButtonClicked()
{
  if(dataHandler->fromEngToJap)
  {
    dataHandler->dataFurigana[randId] =
      displayFurigana.text().toStdString();

    if(dataHandler->dataFurigana[randId]
       == dataHandler->truedataFurigana[randId]) showSuccess();
    else showFailure();
  }
  else
  {
    dataHandler->dataKanji[randId] =
      displayKanji.text().toStdString();

    if(dataHandler->dataKanji[randId]
       == dataHandler->truedataKanji[randId]) showSuccess();
    else showFailure();
  }
}

void QtKanji::ExampleWindow::continueButtonClicked()
{
  ExampleWindow *exampleWindow =
    createExampleWindow(dataHandler, successes, failures);
  this->close();
  exampleWindow->show();
}

void QtKanji::ExampleWindow::showSuccess()
{
  Success       .show();
  Failure       .hide();
  continueButton.show();
  
  if(dataHandler->fromEngToJap)
  {
    displayFurigana.setText(
      QString::fromStdString(dataHandler->truedataFurigana[randId]));

    displayFurigana.setStyleSheet("color: green");
  }
  else
  {
    displayKanji.setText(
      QString::fromStdString(dataHandler->truedataKanji[randId]));

    displayKanji.setStyleSheet("color: green");
  }
 
  ++successes;

  submitButton.hide();
}

void QtKanji::ExampleWindow::showFailure()
{
  Success       .hide();
  Failure       .show();
  continueButton.show();

  if(dataHandler->fromEngToJap)
  {
    displayFurigana.setText(
      QString::fromStdString(dataHandler->truedataFurigana[randId]));
 
    displayFurigana.setStyleSheet("color: red");
  }
  else
  {
    displayKanji.setText(
      QString::fromStdString(dataHandler->truedataKanji[randId]));

    displayKanji.setStyleSheet("color: red");
  }
 
  ++failures;

  submitButton.hide();
}
