#ifndef HADAMITZKYWINDOW_H
#define HADAMITZKYWINDOW_H

#include "helper.h"
#include "hadamitzkydata.h"



namespace QtKanji {

class HadamitzkyWindow : public QWidget
{
  Q_OBJECT
  
 public:
  static HadamitzkyWindow* createHadamitzkyWindow(QWidget *parent = 0)
    {return new HadamitzkyWindow{parent};}
  explicit HadamitzkyWindow(QWidget *parent = 0);

  QtKanji::Error printSigns(unsigned int lowerLimit, unsigned int upperLimit) const;
  
  Buttons graphemeButtons{NUMBER_OF_GRAPHEMES};
  QLineEdit* search{};
  QLabel possibleKanji{"possible kanji:"};
  Buttons kanjiButtons{NUMBER_OF_HADAMITZKY_KANJI};
  QGridLayout layout{};

  std::vector<Uints> radicalKanjiMap{};
  std::vector<std::map<unsigned int, unsigned int>> radicalStrokeNumberMaps{};

  HadamitzkyWindow(const HadamitzkyWindow&) = delete;
  HadamitzkyWindow& operator=(const HadamitzkyWindow&) = delete;
  HadamitzkyWindow(HadamitzkyWindow&&) = delete;
  HadamitzkyWindow& operator=(HadamitzkyWindow&&) = delete;
  ~HadamitzkyWindow() = default;
  
 private slots:
  void kanjiClicked(const unsigned int kanji);
  void graphemeButtonClicked(unsigned int graphemeIndex);
  
 private:
  unsigned short int clicks{0};
  Uints selectedRadicals{3};
};
 
};

#endif
