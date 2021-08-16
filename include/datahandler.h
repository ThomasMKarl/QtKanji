#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include "helper.h"



namespace QtKanji {
  
struct Boxes
{
  explicit Boxes() = default;
  
  bool hideImi{false};
  bool hideKun{false};
  bool hideOn {false};
  SharedCheckbox imiBox{}, kunBox{}, onBox{};
};

class DataHandler
{
 public:
  explicit DataHandler() = default;
  
  bool fromEngToJap{true};
  
  unsigned int lowerLimit{1};
  unsigned int upperLimit{NUMBER_OF_KANJI};
  
  std::string pathToContainerData{"container.dat"};
  std::string pathToExampleData{"examples.dat"};
  std::string pathToKanjiData{"kanjidb.dat"};
  
  Uints indexContainer{};
  Uints indexInCardbox{};

  Examples examples{};
  Flashcard flashcard{};

  Error getLimits(MainWindow &mainwindow);
  Error computeExampleData(MainWindow &mainwindow);
  Error computeContainerData(MainWindow &mainwindow);
  
  void computeKanjiData(unsigned int Id);

  unsigned int computeRandomId(bool fromCardbox, unsigned int removeFlag);
  unsigned int searchKanjiId(QString kanji);
  Error printExamples();
};

};

#endif
