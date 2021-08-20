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
  explicit DataHandler(unsigned int lowerLimit_, 
                       unsigned int upperLimit_, 
                       bool fromEngToJap_) :
    lowerLimit(lowerLimit_),
    upperLimit(upperLimit_),
    fromEngToJap(fromEngToJap_){}

  unsigned int lowerLimit{1};
  unsigned int upperLimit{NUMBER_OF_KANJI};
  bool fromEngToJap{true};

  std::string pathToContainerData{"container.dat"};
  std::string pathToExampleData{"examples.dat"};
  std::string pathToKanjiData{"kanjidb.dat"};

  Uints indexInCardbox{};

  Examples examples{};
  Flashcard flashcard{};

  bool getFromEngToJap() const {return fromEngToJap;}
  
  Error computeExampleData(bool randomize);
  Error computeContainerData();
  
  void computeKanjiData(unsigned int Id);

  unsigned int computeRandomId(bool fromCardbox, unsigned int removeFlag);
  unsigned int searchKanjiId(QString kanji);
  Error printExamples();

 private:     
  Uints indexContainer{};
};

};

#endif
