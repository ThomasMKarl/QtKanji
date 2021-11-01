#include "examplewindow.h"

QtKanji::ExampleWindow::ExampleWindow(DataHandler &dataHandler_, QWidget *parent)
    : dataHandler{dataHandler_}, QWidget(parent)
{
  textfont.setPointSize(20);
  textfont.setBold(false);
  setFont(textfont);

  setButtonLayout();

  setExampleWindowLayout();

  update();
}

void QtKanji::ExampleWindow::setExampleWindowLayout()
{
  const auto &EX = dataHandler.examples;

  unsigned int Id = failures + successes + 1;

  layout.addWidget(&Furigana, 1, 0);
  layout.addWidget(&Kanji, 2, 0);

  if (dataHandler.fromEngToJap)
  {
    layout.addWidget(&displayFurigana, 1, 1);

    Kanji2.setText(QString::fromStdString(EX.dataKanji[Id - 1]));
    layout.addWidget(&Kanji2, 2, 1);
  }
  else
  {
    Furigana2.setText(QString::fromStdString(EX.dataFurigana[Id - 1]));
    layout.addWidget(&Furigana2, 1, 1);

    layout.addWidget(&displayKanji, 2, 1);
  }

  English2.setText(QString::fromStdString(EX.dataEnglish[Id - 1]));
  layout.addWidget(&English, 3, 0);
  layout.addWidget(&English2, 3, 1);

  layout.addWidget(&Success, 4, 0);
  layout.addWidget(&Failure, 4, 0);
  Success.setStyleSheet("color: green; font-size: 30px");
  Failure.setStyleSheet("color: red;   font-size: 30px");

  setLayout(&layout);
}

void QtKanji::ExampleWindow::setButtonLayout()
{
  connect(&submitButton, &QPushButton::clicked, this, &ExampleWindow::submitButtonClicked);
  layout.addWidget(&submitButton, 1, 2);

  connect(&continueButton, &QPushButton::clicked, this, &ExampleWindow::continueButtonClicked);
  layout.addWidget(&continueButton, 1, 2);

  setLayout(&layout);
}

void QtKanji::ExampleWindow::update()
{
  const auto &EX = dataHandler.examples;

  unsigned int Id = failures + successes + 1;

  unsigned int numberOfExamples = dataHandler.examples.dataFurigana.size();

  if (Id == numberOfExamples + 1)
  {
    showResult();
    return;
  }

  continueButton.hide();
  submitButton.show();

  if (dataHandler.fromEngToJap)
  {
    Kanji2.setText(QString::fromStdString(EX.dataKanji[Id - 1]));
    displayFurigana.setText("");
    displayFurigana.setStyleSheet("color: black; font-size: 30px");
  }
  else
  {
    Furigana2.setText(QString::fromStdString(EX.dataFurigana[Id - 1]));
    displayKanji.setText("");
    displayKanji.setStyleSheet("color: black; font-size: 30px");
  }
  English2.setText(QString::fromStdString(EX.dataEnglish[Id - 1]));

  Success.hide();
  Failure.hide();

  adjustSize();

  setWindowTitle("QtKanji example #" + QString::number(Id) + " of " + QString::number(numberOfExamples));
  setWindowIcon(QIcon("kanji.ico"));
}

void QtKanji::ExampleWindow::submitButtonClicked()
{
  const auto &EX = dataHandler.examples;

  unsigned int Id = failures + successes + 1;

  std::string dataFurigana{}, dataKanji{};
  if (dataHandler.fromEngToJap)
  {
    dataFurigana = displayFurigana.text().toStdString();

    if (EX.furiganaMatches(Id, dataFurigana))
      showSuccess();
    else
      showFailure();
  }
  else
  {
    dataKanji = displayKanji.text().toStdString();

    if (EX.kanjiMatches(Id, dataFurigana))
      showSuccess();
    else
      showFailure();
  }
}

void QtKanji::ExampleWindow::continueButtonClicked()
{
  update();
}

void QtKanji::ExampleWindow::showSuccess()
{
  const auto &EX = dataHandler.examples;

  unsigned int Id = failures + successes + 1;

  Success.show();
  Failure.hide();
  continueButton.show();

  if (dataHandler.fromEngToJap)
  {
    displayFurigana.setText(QString::fromStdString(EX.dataFurigana[Id - 1]));

    displayFurigana.setStyleSheet("color: green; font-size: 30px");
  }
  else
  {
    displayKanji.setText(QString::fromStdString(EX.dataKanji[Id - 1]));

    displayKanji.setStyleSheet("color: green; font-size: 30px");
  }

  ++successes;

  submitButton.hide();
}

void QtKanji::ExampleWindow::showFailure()
{
  const auto &EX = dataHandler.examples;

  unsigned int Id = failures + successes + 1;

  Success.hide();
  Failure.show();
  continueButton.show();

  if (dataHandler.fromEngToJap)
  {
    displayFurigana.setText(QString::fromStdString(EX.dataFurigana[Id - 1]));

    displayFurigana.setStyleSheet("color: red; font-size: 30px");
  }
  else
  {
    displayKanji.setText(QString::fromStdString(EX.dataKanji[Id - 1]));

    displayKanji.setStyleSheet("color: red; font-size: 30px");
  }

  ++failures;

  submitButton.hide();
}

void QtKanji::ExampleWindow::showResult()
{
  hideAll();

  double ratio = 100.0 * successes / (failures + successes);

  Success.setText("Successes:");
  Success.show();
  Success2.setText(QString::number(successes));
  Failure.setText("Failures:");
  Failure.show();
  Failure2.setText(QString::number(failures));
  Rate.setText("Success Rate:");
  Rate2.setText(QString::number(ratio) + "%");

  cards.setText("Number of cards in box: " + QString::number(dataHandler.indexInCardbox.size()));

  if (dataHandler.indexInCardbox.empty())
    cards.setStyleSheet("color: green; font-size: 30px");
  else
    cards.setStyleSheet("color: red;   font-size: 30px");

  layout.addWidget(&Success, 0, 0);
  layout.addWidget(&Failure, 1, 0);
  layout.addWidget(&Success2, 0, 1);
  layout.addWidget(&Failure2, 1, 1);
  layout.addWidget(&Rate, 2, 0);
  layout.addWidget(&Rate2, 2, 1);
  layout.addWidget(&cards, 3, 0);

  Success2.setStyleSheet("color: green;  font-size: 30px");
  Failure2.setStyleSheet("color: red;    font-size: 30px");
  if (ratio >= 85)
    Rate2.setStyleSheet("color: green;  font-size: 30px");
  if (ratio < 85 && ratio >= 50)
    Rate2.setStyleSheet("color: yellow; font-size: 30px");
  if (ratio < 50 && ratio > 15)
    Rate2.setStyleSheet("color: orange; font-size: 30px");
  if (ratio <= 15)
    Rate2.setStyleSheet("color: red;    font-size: 30px");

  setLayout(&layout);

  adjustSize();

  setWindowTitle("QtKanji Result");
  setWindowIcon(QIcon("kanji.ico"));
}

void QtKanji::ExampleWindow::hideAll()
{
  displayFurigana.hide();
  displayKanji.hide();
  Furigana.hide();
  Furigana2.hide();
  English.hide();
  English2.hide();
  Kanji.hide();
  Kanji2.hide();
  Success.hide();
  Failure.hide();
  cards.hide();
  submitButton.hide();
  continueButton.hide();
}