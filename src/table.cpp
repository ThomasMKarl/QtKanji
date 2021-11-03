#include "table.h"

QtKanji::Table::Table(QWidget *parent) : QWidget(parent)
{
  move(800, 0);

  QFont textfont{};
  textfont.setPointSize(15);
  textfont.setBold(false);
  setFont(std::move(textfont));
  QPalette pal = palette();
  pal.setColor(QPalette::Window, Qt::gray);
  setAutoFillBackground(true);
  setPalette(pal);

  setWindowTitle("QtKanji Table");
  setWindowIcon(QIcon("kanji.ico"));

  Coord coord{};
  for (unsigned int index = 0; index < NUMBER_OF_KANJI; ++index)
  {
    coord = getCoordinate(index);

    kanjiButtons[index].setText(kanjiList[index]);
    kanjiButtons[index].setFixedSize(25, 25);
    connect(&kanjiButtons[index], &QPushButton::clicked, this, [=]() { kanjiClicked(index); });
    layout.addWidget(&kanjiButtons[index], coord.row, coord.column);
  }

  setLayout(&layout);
}

void QtKanji::Table::kanjiClicked(const unsigned int index)
{
  QClipboard *clipboard = QGuiApplication::clipboard();
  clipboard->setText(kanjiList[index]);
  for (const auto &flashcard : flashcards)
    flashcard->displaySign.setText(kanjiList[index]);

  search->setText(kanjiList[index]);
}

QtKanji::Coord QtKanji::Table::getCoordinate(unsigned int index)
{
  Coord coord{};

  if (index < 28)
  {
    coord.row = 0;
    coord.column = index;
  }
  else if (index < 100)
  {
    index -= 28;
    coord.row = index / 12 + 1;
    coord.column = index % 12;
  }
  else if (index < 172)
  {
    index -= 100;
    coord.row = index / 12 + 1;
    coord.column = index % 12 + 13;
  }
  else if (index < 244)
  {
    index -= 172;
    coord.row = index / 12 + 7;
    coord.column = index % 12;
  }
  else if (index < 316)
  {
    index -= 244;
    coord.row = index / 12 + 7;
    coord.column = index % 12 + 13;
  }
  else if (index < 460)
  {
    index -= 316;
    coord.row = index / 24 + 26;
    coord.column = index % 24;
  }
  else if (index < 604)
  {
    index -= 460;
    coord.row = index / 24 + 34;
    coord.column = index % 24;
  }

  return coord;
}
