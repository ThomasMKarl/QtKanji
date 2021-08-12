#ifndef HEADERS_H
#define HEADERS_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include <QString>
#include <QApplication>
#include <QPainter>
#include <QCheckBox>
#include <QClipboard>

#include <cstdlib>
#include <ctime>

#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <memory>

#define NUMBER_OF_KANJI 540
#define NUMBER_OF_GRAPHEMES 4

namespace QtKanji {

  class MainWindow;
  class FlashcardWindow;
  class ExampleWindow;
  class Table;
  class Boxes;
  class DataHandler;
  
  using SharedCheckbox = std::shared_ptr<QCheckBox>;
  using Buttons = std::vector<QPushButton>;
  
  using SharedTable = std::shared_ptr<Table>;
  using FlashcardWindows = std::vector<FlashcardWindow*>;
  using SharedBoxes = std::shared_ptr<Boxes>;
  using SharedData = std::shared_ptr<DataHandler>;


  struct Flashcard
  {
    explicit Flashcard() = default;

    unsigned int  ID{};
    unsigned int HID{};
    std::string dataSign{};
    std::vector<std::string> dataImiVector{}, dataOnVector{}, dataKunVector{};
    
    bool operator==(const Flashcard& rhs)
    {
      if(dataSign      != rhs.dataSign)      return false;
      if(dataImiVector != rhs.dataImiVector) return false;
      if(dataOnVector  != rhs.dataOnVector)  return false;
      if(dataKunVector != rhs.dataKunVector) return false;
      return true;
    }
  };

};

#include "functions.h"

#endif
