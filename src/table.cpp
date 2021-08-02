#include "table.h"

QtKanji::Table::Table(QWidget *parent): QWidget(parent)
{  
  layout = std::make_unique<QGridLayout>();
  
  QFont textfont{};
  textfont.setPointSize(15);
  textfont.setBold(false);
  setFont(std::move(textfont));
  setWindowTitle("QtKanji Table");
  setWindowIcon(QIcon("kanji.ico"));
  
  std::string linedata{};
  std::ifstream kanjiData{"kanjidb.dat"};
  if(!kanjiData) return;
  while(!kanjiData.eof())
  {
    kanjiData.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(kanjiData, linedata, '\n');
    tableData.push_back(linedata);

    for(unsigned short int i = 0; i < 6; ++i)
      kanjiData.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  
  unsigned int row, column;
  for(unsigned int index = 0; index <= 143; ++index)
  {
    row = index/25;
    column = index%25;

    kanjiButtons.push_back(std::make_unique<QPushButton>(QString::fromStdString(tableData[index])));
    kanjiButtons[index]->setFixedSize(25,25);
    connect(kanjiButtons[index].get(),
	    &QPushButton::clicked,
	    this,
	    [=](){ kanjiClicked(tableData[index]);});
    layout->addWidget(kanjiButtons[index].get(),row,column);
  }

  setLayout(layout.get());
}

void QtKanji::Table::kanjiClicked(const std::string kanji)
{
  QClipboard *clipboard = QGuiApplication::clipboard();
  clipboard->setText(QString::fromStdString(kanji));
}
