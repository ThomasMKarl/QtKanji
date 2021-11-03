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
  explicit DataHandler(unsigned int lowerLimit_, unsigned int upperLimit_, unsigned int lowerLection_,
                       unsigned int upperLection_, bool fromFuriToKanji_, bool fromJapToEng_)
      : lowerLimit(lowerLimit_), upperLimit(upperLimit_), lowerLection(lowerLection_), upperLection(upperLection_),
        fromFuriToKanji(fromFuriToKanji_), fromJapToEng(fromJapToEng_)
  {
  }

  unsigned int lowerLimit{1};
  unsigned int upperLimit{NUMBER_OF_KANJI};
  unsigned int lowerLection{1};
  unsigned int upperLection{25};
  bool fromFuriToKanji{true};
  bool fromJapToEng{true};

  std::string pathToKanjiCardboxData{"kanjiCardbox.dat"};
  std::string pathToExampleCardboxData{"exampleCardbox.dat"};
  std::string pathToWordCardboxData{"wordCardbox.dat"};

  Uints indexInKanjiCardbox{};
  Uints indexInExampleCardbox{};
  Uints indexInWordCardbox{};

  Examples examples{};
  Examples words{};
  Flashcard flashcard{};

  bool getFromFuriToKanji() const
  {
    return fromFuriToKanji;
  }

  bool getFromJapToEng() const
  {
    return fromJapToEng;
  }

  Error computeExampleData(bool randomize);
  Error computeWordData(bool randomize);
  Error computeKanjiData(unsigned int Id);

  Error computeKanjiCardboxData();

  void setKanjiData(unsigned int ID);

  unsigned int computeRandomId(bool fromCardbox, unsigned int removeFlag);
  unsigned int searchKanjiId(QString kanji);
  Error printExamples();

private:
  Uints indexKanjiContainer{};
  Uints indexExampleContainer{};
  Uints indexWordContainer{};
};

}; // namespace QtKanji

#endif
