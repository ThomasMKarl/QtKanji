#include "examplewindow.h"

QtKanji::ExampleWindow::ExampleWindow(SharedData &dataHandler_, QWidget *parent) :
  dataHandler{dataHandler_},
  QWidget(parent)
{  
  setButtonLayout();

  setExampleWindowLayout();

  update();
}

void QtKanji::ExampleWindow::setExampleWindowLayout()
{
  const auto &EX = dataHandler->examples;

  unsigned int Id = failures + successes + 1;

  layout.addWidget(&Furigana, 1,0);
  layout.addWidget(&Kanji, 2,0);
  
  if(dataHandler->fromEngToJap)
  {
    layout.addWidget(&displayFurigana,1,1);

    Kanji2.setText(QString::fromStdString(EX.dataKanji[Id-1]));
    layout.addWidget(&Kanji2,2,1);
  }
  else
  {
    Furigana2.setText(QString::fromStdString(EX.dataFurigana[Id-1]));
    layout.addWidget(&Furigana2,1,1);

    layout.addWidget(&displayKanji,2,1);
  }

  English2.setText(QString::fromStdString(EX.dataEnglish[Id-1]));
  layout.addWidget(&English, 3,0);
  layout.addWidget(&English2,3,1);
 
  layout.addWidget(&Success,4,0);
  layout.addWidget(&Failure,4,0); 
  Success.setStyleSheet("color: green");
  Failure.setStyleSheet("color: red");
  
  setLayout(&layout);
}

void QtKanji::ExampleWindow::setButtonLayout()
{
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

  setLayout(&layout);
}

void QtKanji::ExampleWindow::update()
{
  textfont.setPointSize(20);
  textfont.setBold(false);
  setFont(textfont);

  const auto &EX = dataHandler->examples;

  unsigned int Id = failures + successes + 1;

  unsigned int numberOfExamples = dataHandler->examples.dataFurigana.size();
  
  if(Id == numberOfExamples+1)
  {
    showResult(*this);
    return;
  }

  continueButton.hide();
  submitButton.show();

  if(dataHandler->fromEngToJap) 
    Kanji2.setText(QString::fromStdString(EX.dataKanji[Id-1]));
  else 
    Furigana2.setText(QString::fromStdString(EX.dataFurigana[Id-1]));

  English2.setText(QString::fromStdString(EX.dataEnglish[Id-1]));

  Success.hide();
  Failure.hide();

  adjustSize();

  setWindowTitle("QtKanji example #"
		 + QString::number(Id)
		 + " of "
		 + QString::number(numberOfExamples));
  setWindowIcon(QIcon("kanji.ico"));
}

void QtKanji::ExampleWindow::submitButtonClicked()
{
  const auto &EX = dataHandler->examples;

  unsigned int Id = failures + successes + 1;

  std::string dataFurigana{}, dataKanji{};
  if(dataHandler->fromEngToJap)
  {
    dataFurigana = displayFurigana.text().toStdString();

    if(EX.furiganaMatches(Id, dataFurigana)) showSuccess();
    else showFailure();
  }
  else
  {
    dataKanji = displayKanji.text().toStdString();

    if(EX.kanjiMatches(Id, dataFurigana)) showSuccess();
    else showFailure();
  }
}

void QtKanji::ExampleWindow::continueButtonClicked()
{
  update();
}

void QtKanji::ExampleWindow::showSuccess()
{
  const auto &EX = dataHandler->examples;
  
  unsigned int Id = failures + successes + 1;

  Success       .show();
  Failure       .hide();
  continueButton.show();
  
  if(dataHandler->fromEngToJap)
  {
    displayFurigana.setText(
      QString::fromStdString(EX.dataFurigana[Id-1]));

    displayFurigana.setStyleSheet("color: green");
  }
  else
  {
    displayKanji.setText(
      QString::fromStdString(EX.dataKanji[Id-1]));

    displayKanji.setStyleSheet("color: green");
  }
 
  ++successes;

  submitButton.hide();
}

void QtKanji::ExampleWindow::showFailure()
{
  const auto &EX = dataHandler->examples;

  unsigned int Id = failures + successes + 1;

  Success       .hide();
  Failure       .show();
  continueButton.show();

  if(dataHandler->fromEngToJap)
  {
    displayFurigana.setText(
      QString::fromStdString(EX.dataFurigana[Id-1]));
 
    displayFurigana.setStyleSheet("color: red");
  }
  else
  {
    displayKanji.setText(
      QString::fromStdString(EX.dataKanji[Id-1]));

    displayKanji.setStyleSheet("color: red");
  }
 
  ++failures;

  submitButton.hide();
}
