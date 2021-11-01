#include "hadamitzkywindow.h"

std::future<void> computeRadicalKanjiMap_asyncHandle{};
QtKanji::HadamitzkyWindow::HadamitzkyWindow(QWidget *parent) : QWidget(parent)
{
  move(0, 350);

  QFont textfont{};
  textfont.setPointSize(15);
  textfont.setBold(false);
  setFont(std::move(textfont));
  setWindowTitle("QtKanji Table of Graphemes");
  setWindowIcon(QIcon("kanji.ico"));

  clicks = 0;

  selectedRadicals.resize(3);
  selectedRadicals[0] = 0;
  selectedRadicals[1] = 0;
  selectedRadicals[2] = 0;

  unsigned short int row{0};
  unsigned short int column{1};
  layout.addWidget(&stroke[row], row, 0);
  stroke[row].setText("(strokes) " + QString::number(row + 1));
  stroke[row].setAlignment(Qt::AlignRight);
  for (unsigned int short index = 0; index < NUMBER_OF_GRAPHEMES; ++index)
  {
    if (nextRow(index))
    {
      row++;
      column = 1;
      layout.addWidget(&stroke[row], row, 0);
      stroke[row].setText(QString::number(row + 1));
      stroke[row].setAlignment(Qt::AlignRight);
    }
    graphemeButtons[index].setText(graphemeList[index]);
    graphemeButtons[index].setFixedSize(25, 25);
    connect(&graphemeButtons[index], &QPushButton::clicked, this, [=]() { graphemeButtonClicked(index + 1); });
    layout.addWidget(&graphemeButtons[index], row, column);
    graphemeButtons[index].setCheckable(true);
    graphemeButtons[index].setEnabled(false);
    column++;
  }

  layout.addWidget(&possibleKanji, row + 1, 0);

  for (unsigned int index = 0; index < kanjiButtons.size(); ++index)
  {
    layout.addWidget(&kanjiButtons[index], index / 9 + row + 1, index % 9 + 1);
    kanjiButtons[index].setFixedSize(25, 25);
    kanjiButtons[index].hide();
  }

  setLayout(&layout);

  computeRadicalKanjiMap_asyncHandle =
      std::async(std::launch::async, &QtKanji::HadamitzkyWindow::computeRadicalKanjiMap, this);
}

void QtKanji::HadamitzkyWindow::graphemeButtonClicked(unsigned int graphemeIndex)
{
  unsigned short int maxNumberOfClicks = 3;
  auto &thisButton = graphemeButtons[graphemeIndex - 1];

  if (thisButton.isChecked())
    clicks++;
  else
    clicks--;
  if (clicks == maxNumberOfClicks + 1)
  {
    clicks--;
    thisButton.setChecked(false);
    return;
  }

  if (thisButton.isChecked())
  {
    const auto indexIterator = std::find(selectedRadicals.begin(), selectedRadicals.end(), 0);
    unsigned int index = std::distance(selectedRadicals.begin(), indexIterator);
    selectedRadicals[index] = graphemeIndex;
  }
  else
  {
    const auto indexIterator = std::find(selectedRadicals.begin(), selectedRadicals.end(), graphemeIndex);
    unsigned int index = std::distance(selectedRadicals.begin(), indexIterator);
    selectedRadicals[index] = 0;
  }

  Uints copy0{}, copy1{}, copy2{};
  if (selectedRadicals[0] != 0)
    copy0 = maps.radicalKanjiMap[selectedRadicals[0] - 1];
  if (selectedRadicals[1] != 0)
    copy1 = maps.radicalKanjiMap[selectedRadicals[1] - 1];
  if (selectedRadicals[2] != 0)
    copy2 = maps.radicalKanjiMap[selectedRadicals[2] - 1];
  std::set<unsigned int> kanjis = overlap(copy0, copy1, copy2);

  for (auto &button : kanjiButtons)
    button.hide();

  unsigned short int buttonNumber{0};
  for (const auto index : kanjis)
  {
    kanjiButtons[buttonNumber].setText(kanjiList[index - 1]);
    connect(&kanjiButtons[buttonNumber], &QPushButton::clicked, this, [=]() { kanjiClicked(index); });
    kanjiButtons[buttonNumber].show();
    buttonNumber++;
  }

  setLayout(&layout);

  adjustSize();
}

void QtKanji::HadamitzkyWindow::kanjiClicked(const unsigned int index)
{
  QClipboard *clipboard = QGuiApplication::clipboard();
  clipboard->setText(kanjiList[index - 1]);

  search->setText(kanjiList[index - 1]);
}

void QtKanji::HadamitzkyWindow::computeRadicalKanjiMap()
{
  maps.radicalKanjiMap.resize(NUMBER_OF_GRAPHEMES);
  for (unsigned int index = 1; index <= NUMBER_OF_GRAPHEMES; ++index)
    maps.radicalKanjiMap[index - 1] = maps.computeContainingKanjiIndices(index);

  for (auto &graphemeButton : graphemeButtons)
    graphemeButton.setEnabled(true);

  printSignButton->setEnabled(true);
}

QtKanji::Error QtKanji::HadamitzkyWindow::printSigns(unsigned int lowerLimit, unsigned int upperLimit) const
{
  std::ofstream htmlFile{"kanji_sorted_" + std::to_string(lowerLimit) + "-" + std::to_string(upperLimit) + ".html"};
  std::ofstream texFile{"kanji_sorted_" + std::to_string(lowerLimit) + "-" + std::to_string(upperLimit) + ".tex"};
  if (!htmlFile || !texFile)
    return Error::FILE_ERROR;

  const std::string header{
      "<!DOCTYPE html>\n<html>\n<head>\n<title>Sorted Kanji</title>\n<meta charset='UTF-8'>\n</head>\n<style>table, "
      "th {border:1px solid black;}</style>\n\n<body>\n<h1>Sorted Kanji</h1>\n"};
  const std::string footer{"\n</body>\n</html>"};

  htmlFile << header;

  unsigned short int strokeNumber{1};
  for (const auto &radicalStrokeNumberMap : maps.radicalStrokeNumberMaps)
  {
    if (!radicalStrokeNumberMap.empty())
    {
      htmlFile << "<h3>Stroke number " << std::to_string(strokeNumber) << "</h3>";
      htmlFile << "<table style=''>\n<tr><th align='left'>Kanji</th><th align='left'>global index</th><th "
                  "align='left'>Had. index</th><th align='left'>radical string</th></tr>\n";
      texFile << "\\section{stroke number " << std::to_string(strokeNumber) << "}\n";
      texFile << "  \\begin{longtable}[c]{llll}\n    \\bfseries Kanji & \\bfseries global index & \\bfseries "
                 "Had. index & \\bfseries radical string\\\\\\hline\\endhead\n";
    }

    strokeNumber++;
    for (const auto &[radicalStrokeNumber, indices] : radicalStrokeNumberMap)
    {
      for (const auto index : indices)
      {
        DataHandler data{};
        if (data.computeKanjiData(index) != QtKanji::Error::SUCCESS)
          continue;
        HadamitzkyData hdata = HadamitzkyData::createHadamitzkyData(data.flashcard.HID);

        htmlFile << "  <tr><td>" << kanjiList[index - 1].toStdString() << "</td><td>" << index << "</td><td>"
                 << data.flashcard.HID << "</td><td>" << hdata.radicalString << "</td></tr>\n";
        texFile << "    \\bfseries " << kanjiList[index - 1].toStdString() << " & " << index << " & "
                << data.flashcard.HID << " & " << hdata.radicalString << "\\\\\n";
      }
    }

    if (!radicalStrokeNumberMap.empty())
    {
      htmlFile << "<p></table>\n";
      texFile << "  \\end{longtable}\n";
    }
  }

  return QtKanji::Error::SUCCESS;
}

bool QtKanji::nextRow(unsigned short int index)
{
  if (index == 2 || index == 20 || index == 40 || index == 52 || index == 60 || index == 66 || index == 71 ||
      index == 75 || index == 77 || index == 78)
    return true;
  return false;
}
