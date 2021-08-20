#include "hadamitzkywindow.h"

QtKanji::HadamitzkyWindow::HadamitzkyWindow(QWidget *parent): QWidget(parent)
{
  move(0,350);
  
  QFont textfont{};
  textfont.setPointSize(15);
  textfont.setBold(false);
  setFont(std::move(textfont));
  setWindowTitle("QtKanji Hadamitzky Data");
  setWindowIcon(QIcon("kanji.ico"));

  clicks = 0;
  
  selectedRadicals.resize(3);
  selectedRadicals[0] = 0;
  selectedRadicals[1] = 0;
  selectedRadicals[2] = 0;

  for(unsigned int index = 0; index < NUMBER_OF_GRAPHEMES; ++index)
  {
    graphemeButtons[index].setText( graphemeList[index] );
    graphemeButtons[index].setFixedSize(25,25);
    connect(&graphemeButtons[index],
	    &QPushButton::clicked,
	    this,
      [=](){ graphemeButtonClicked(index+1); });
	    layout.addWidget(&graphemeButtons[index],index/9,index%9+1);
    graphemeButtons[index].setCheckable(true);
  }

  layout.addWidget(&possibleKanji,9,0);
  possibleKanji.hide();

  for(unsigned int index = 0; index < kanjiButtons.size(); ++index)
  {
    layout.addWidget(&kanjiButtons[index],index/9+9,index%9+1);
    kanjiButtons[index].hide();
  }

  setLayout(&layout);
}

void QtKanji::HadamitzkyWindow::graphemeButtonClicked(unsigned int graphemeIndex)
{
  unsigned short int maxNumberOfClicks = 3;
  auto &thisButton = graphemeButtons[graphemeIndex-1];
  
  if(thisButton.isChecked()) clicks++;
  else clicks--;
  if(clicks == maxNumberOfClicks+1)
  {
    clicks--;
    thisButton.setChecked(false);
    return;
  }

  if(thisButton.isChecked())
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

  Uints kanjiIndices{};
  if(selectedRadicals[0] != 0) kanjiIndices = radicalKanjiMap[selectedRadicals[0]-1];  
  std::vector<unsigned int> copy0{kanjiIndices.begin(),kanjiIndices.end()};
  if(selectedRadicals[1] != 0) kanjiIndices = radicalKanjiMap[selectedRadicals[1]-1];
  std::vector<unsigned int> copy1{kanjiIndices.begin(),kanjiIndices.end()};
  if(selectedRadicals[2] != 0) kanjiIndices = radicalKanjiMap[selectedRadicals[2]-1];
  std::vector<unsigned int> copy2{kanjiIndices.begin(),kanjiIndices.end()};
  std::set<unsigned int> kanjis = overlap(copy0, copy1, copy2);


  if(kanjis.empty()) possibleKanji.hide();
  else               possibleKanji.show();

  for(auto &button : kanjiButtons) button.hide();

  for(unsigned int index = 0; index < kanjis.size(); ++index)
  {
    kanjiButtons[index].setText( kanjiList[index] );
    kanjiButtons[index].setFixedSize(25,25);
    connect(&kanjiButtons[index],
	    &QPushButton::clicked,
	    this,
      [=](){ kanjiClicked(index); });
    kanjiButtons[index].show();
  }
  
  setLayout(&layout);

  adjustSize();
}

void QtKanji::HadamitzkyWindow::kanjiClicked(const unsigned int index)
{
  QClipboard *clipboard = QGuiApplication::clipboard();
  clipboard->setText( kanjiList[index] );
  
  search->setText( kanjiList[index] );
}

QtKanji::Error QtKanji::HadamitzkyWindow::printSigns(unsigned int lowerLimit, unsigned int upperLimit) const
{
  std::ofstream file{"kanji_sorted_" + std::to_string(lowerLimit) + "-" + std::to_string(upperLimit) + ".html"};
  if(!file) return Error::FILE_ERROR;

  const std::string header{"<!DOCTYPE html>\n<html>\n<head>\n<title>Sorted Kanji</title>\n<meta charset='UTF-8'>\n</head>\n\n<body>\n<h1>Sorted Kanji</h1>\n"};
  const std::string footer{"\n</body>\n</html>"};
  
  file << header;

  unsigned short int counter{0};
  for(const auto &radicalStrokeNumberMap : radicalStrokeNumberMaps)
  {
    file << "Stroke number " << std::to_string(++counter) << "<p>";
    for(const auto &[radicalStrokeNumber,index] : radicalStrokeNumberMap)
      file << kanjiList[index-1].toStdString() << " ";

    file << "<p>\n";
  }

  return QtKanji::Error::SUCCESS;
}