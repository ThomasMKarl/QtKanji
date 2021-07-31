#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include "headers.h"



namespace QtKanji {
struct Boxes
{
  explicit Boxes() = default;
  
  bool hideImi{false};
  bool hideKun{false};
  bool hideOn {false};
  std::shared_ptr<QCheckBox> imiBox{}, kunBox{}, onBox{};
};

class MainWindow;
class Flashcard;
class DataHandler
{
 public:
  explicit DataHandler() = default;
  
  bool fromEngToJap{true};
  
  unsigned int lowerLimit{245};
  unsigned int upperLimit{460};
  
  std::string pathToContainerData{"container.dat"};
  std::string pathToExampleData{"examples.dat"};
  std::string pathToKanjiData{"kanjidb.dat"};
  
  std::vector<unsigned int> indexContainer{};
  std::vector<unsigned int> indexInCardbox{};
  
  std::vector<std::string> dataFurigana{}, truedataFurigana{},
                           dataKanji{}, truedataKanji{},
                           dataEnglish{};
  std::string dataSign{}, truedataSign{},
              dataImi{}, truedataImi{},
              dataOn{}, truedataOn{},
              dataKun{}, truedataKun{};
  std::vector<std::string> dataImiVector{}, truedataImiVector{},
                           dataOnVector{}, truedataOnVector{},
                           dataKunVector{}, truedataKunVector{};

  bool getLimits(MainWindow &mainwindow);
  bool computeExampleData(MainWindow &mainwindow);
  bool computeContainerData(MainWindow &mainwindow, bool fromCardbox);
  
  void computeKanjiData(Flashcard &flashcard);
};

};

#endif
