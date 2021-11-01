#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include "helper.h"

namespace QtKanji
{

struct Boxes
{
  Boxes() = default;

  bool hideImi{false};
  bool hideKun{false};
  bool hideOn{false};
  SharedCheckbox imiBox{}, kunBox{}, onBox{};
};

class DataHandler
{
public:
  DataHandler() = default;
  explicit DataHandler(unsigned int lowerLimit_, unsigned int upperLimit_, bool fromEngToJap_)
      : lowerLimit(lowerLimit_), upperLimit(upperLimit_), fromEngToJap(fromEngToJap_)
  {
  }

  unsigned int lowerLimit{1};
  unsigned int upperLimit{NUMBER_OF_KANJI};
  unsigned int lowerLection{1};
  unsigned int upperLection{25};
  bool fromEngToJap{true};

  std::string pathToContainerData{"container.dat"};

  Uints indexInCardbox{};

  Examples examples{};
  Examples words{};
  Flashcard flashcard{};

  bool getFromEngToJap() const
  {
    return fromEngToJap;
  }

  Error computeExampleData(bool randomize);
  Error computeWordData(bool randomize);
  Error computeContainerData();
  Error computeKanjiData(unsigned int Id);
  void setKanjiData(unsigned int ID);

  unsigned int computeRandomId(bool fromCardbox, unsigned int removeFlag);
  unsigned int searchKanjiId(QString kanji);
  Error printExamples();

private:
  Uints indexContainer{};
};

}; // namespace QtKanji

#endif
