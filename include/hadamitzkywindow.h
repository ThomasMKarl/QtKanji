#ifndef HADAMITZKYWINDOW_H
#define HADAMITZKYWINDOW_H

#include "headers.h"
#include "hadamitzkydata.h"



namespace QtKanji {

class HadamitzkyWindow : public QWidget
{
  Q_OBJECT
  
 public:
  static HadamitzkyWindow* createHadamitzkyWindow(QWidget *parent = 0)
    {return new HadamitzkyWindow{parent};}
  explicit HadamitzkyWindow(QWidget *parent = 0);
  
  Buttons graphemeButtons{NUMBER_OF_GRAPHEMES};
  QGridLayout layout{};

  std::vector<Uints> radicalKanjiMap{};
  
 private slots:
  void graphemeButtonClicked(unsigned int graphemeIndex);
  
 private:
  unsigned short int clicks{0};
  Uints selectedRadicals{3};
};
 
};

#endif
