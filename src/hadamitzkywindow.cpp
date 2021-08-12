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
	    layout.addWidget(&graphemeButtons[index],0,index);
    graphemeButtons[index].setCheckable(true);
  }

  setLayout(&layout);
}

#include <iostream>
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

  /*Uints kanjiIncices{};
  
  if(selectedRadicals[0] != 0) kanjiIndices = radicalKanjiMap[selectedRadicals[0]-1];  
  std::set<Uints> kanji{kanjiIndices[0].begin(),kanjiIndices[0].end()};
  
  if(selectedRadicals[1] != 0) kanjiString = radicalKanjiMap[selectedRadicals[1]-1];
  std::set<Uints> copy1{kanjiIncices[1].begin(),kanjiIncices[1].end()};
  kanji.merge(std::move(copy1));
  
  if(selectedRadicals[2] != 0) kanjiString = radicalKanjiMap[selectedRadicals[2]-1];
  std::set<Uints> copy2{kanjiIncices[2].begin(),kanjiIncices[2].end()};
  kanji.merge(std::move(copy2));*/
}
