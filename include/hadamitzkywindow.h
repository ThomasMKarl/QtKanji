#ifndef HADAMITZKYWINDOW_H
#define HADAMITZKYWINDOW_H

#include "datahandler.h"
#include "hadamitzkydata.h"
#include "helper.h"

namespace QtKanji
{

class HadamitzkyWindow : public QWidget
{
  Q_OBJECT

public:
  static HadamitzkyWindow *createHadamitzkyWindow(QWidget *parent = 0)
  {
    return new HadamitzkyWindow{parent};
  }
  explicit HadamitzkyWindow(QWidget *parent = 0);

  void computeRadicalKanjiMap();
  QtKanji::Error printSigns(unsigned int lowerLimit, unsigned int upperLimit) const;

  QLabel possibleKanji{"possible kanji:"};
  QLineEdit *search{};
  QPushButton *printSignButton{};
  Buttons graphemeButtons{NUMBER_OF_GRAPHEMES};
  Buttons kanjiButtons{NUMBER_OF_HADAMITZKY_KANJI};
  std::array<QLabel, 11> stroke{};
  QGridLayout layout{};

  Maps maps{};

  HadamitzkyWindow(const HadamitzkyWindow &) = delete;
  HadamitzkyWindow &operator=(const HadamitzkyWindow &) = delete;
  HadamitzkyWindow(HadamitzkyWindow &&) = delete;
  HadamitzkyWindow &operator=(HadamitzkyWindow &&) = delete;
  ~HadamitzkyWindow() = default;

private slots:
  void kanjiClicked(const unsigned int kanji);
  void graphemeButtonClicked(unsigned int graphemeIndex);

private:
  unsigned short int clicks{0};
  Uints selectedRadicals{3};
};

bool nextRow(unsigned short int index);

}; // namespace QtKanji

#endif
