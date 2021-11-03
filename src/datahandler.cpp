#include "datahandler.h"
#include "mainwindow.h"
#include "parsejson.h"

QtKanji::Error QtKanji::DataHandler::computeExampleData(bool randomize)
{
  examples.clear();

  unsigned int tryId{0};
  unsigned int counter{0};

  while (true)
  {
    tryId = qtkanji_JSON.getID_examples(counter++);
    if (tryId >= lowerLimit && tryId <= upperLimit)
    {
      explode(", ", qtkanji_JSON.getFurigana_examples(tryId - 1), examples.dataFurigana);
      explode(", ", qtkanji_JSON.getKanji_examples(tryId - 1), examples.dataKanji);
      explode(", ", qtkanji_JSON.getImi_examples(tryId - 1), examples.dataEnglish);
    }
    if (counter == upperLimit - lowerLimit + 1)
      break;
  }

  if (!examples.areValid())
    return QtKanji::Error::FILE_ERROR;

  if (randomize)
    examples.shuffle();

  return QtKanji::Error::SUCCESS;
}

QtKanji::Error QtKanji::DataHandler::computeWordData(bool randomize)
{
/*  words.clear();

  unsigned int tryLection{0};
  unsigned int counter{0};

  while (true)
  {
    tryLection = qtkanji_JSON.getLection_words(counter++);
    if (tryLection >= lowerLection && tryLection <= upperLection)
    {
      explode(", ", qtkanji_JSON.getFurigana_examples(tryLection), words.dataFurigana);
      explode(", ", qtkanji_JSON.getKanji_examples(tryLection), words.dataKanji);
      explode(", ", qtkanji_JSON.getImi_examples(tryLection), words.dataEnglish);
    }
    if (counter == upperLection - lowerLection + 1)
      break;
  }

  if (randomize)
    words.shuffle();
*/
  return QtKanji::Error::SUCCESS;
}

QtKanji::Error QtKanji::DataHandler::computeKanjiCardboxData()
{
  indexInKanjiCardbox.clear();

  std::ifstream containerData{pathToKanjiCardboxData};
  if (!containerData)
    return QtKanji::Error::FILE_ERROR;

  std::string linedata{};
  std::getline(containerData, linedata);
  if (linedata.empty())
    return QtKanji::Error::EMPTY_CARDBOX;

  Strings help = explode(":", std::move(linedata));
  indexInKanjiCardbox = convertStringsToIntegers(std::move(help));

  std::sort(std::begin(indexInKanjiCardbox), std::end(indexInKanjiCardbox));

  getSortedSubarray(indexInKanjiCardbox, lowerLimit, upperLimit);
  if (indexInKanjiCardbox.empty())
    return QtKanji::Error::NO_KANJI_WITHIN_RANGE;

  return QtKanji::Error::SUCCESS;
}

QtKanji::Error QtKanji::DataHandler::computeKanjiData(unsigned int ID)
{
  auto &FC = this->flashcard;
  FC.dataImiVector.clear();
  FC.dataKunVector.clear();
  FC.dataOnVector.clear();

  unsigned int counter{0};

  FC.ID = qtkanji_JSON.getID_kanji(ID - 1);
  if (FC.ID == ID)
    setKanjiData(ID);
  else
  {
    while (true)
    {
      FC.ID = qtkanji_JSON.getID_kanji(counter++);
      if (FC.ID == ID)
        setKanjiData(counter);
      else
        return QtKanji::Error::KANJI_NOT_FOUND;
    }
  }

  return QtKanji::Error::SUCCESS;
}

void QtKanji::DataHandler::setKanjiData(unsigned int ID)
{
  auto &FC = this->flashcard;
  FC.HID = qtkanji_JSON.getHID_kanji(ID - 1);
  FC.dataSign = qtkanji_JSON.getSign_kanji(ID - 1);
  FC.dataImiVector = qtkanji_JSON.getImiVector_kanji(ID - 1);
  FC.dataKunVector = qtkanji_JSON.getKunVector_kanji(ID - 1);
  FC.dataOnVector = qtkanji_JSON.getOnVector_kanji(ID - 1);
}

unsigned int QtKanji::DataHandler::computeRandomId(bool fromCardbox, unsigned int removeFlag)
{
  srand(time(NULL));

  unsigned int range = upperLimit - lowerLimit + 1;
  unsigned int cardboxSize = indexInKanjiCardbox.size();
  unsigned int randomIndex, randId;

  bool notFinished = indexKanjiContainer.size() != range + removeFlag;
  bool gotAlreadyIndex{};
  if (fromCardbox)
  {
    gotAlreadyIndex = true;
    while (gotAlreadyIndex && notFinished)
    {
      randomIndex = rand() % (cardboxSize);
      randId = indexInKanjiCardbox[randomIndex];
      gotAlreadyIndex = contains(indexKanjiContainer, randId);
    }
  }
  else
  {
    gotAlreadyIndex = true;
    while (gotAlreadyIndex && notFinished)
    {
      randId = lowerLimit + rand() % range;
      gotAlreadyIndex = contains(indexKanjiContainer, randId);
    }
  }

  indexKanjiContainer.push_back(randId);

  return randId;
}

unsigned int QtKanji::DataHandler::searchKanjiId(QString kanji)
{
  const auto index = std::find(kanjiList.begin(), kanjiList.end(), kanji);
  if (index == kanjiList.end())
    return 0;

  return std::distance(kanjiList.begin(), index) + 1;
}

QtKanji::Error QtKanji::DataHandler::printExamples()
{
  return examples.print();
}
