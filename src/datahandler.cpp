#include "datahandler.h"
#include "mainwindow.h"


QtKanji::Error QtKanji::DataHandler::computeExampleData(bool randomize)
{
  examples.clear();

  std::ifstream exampleData{pathToExampleData};
  if(!exampleData) return QtKanji::Error::FILE_ERROR;
    
  std::string linedata{};
  while(!exampleData.eof())
  {
    std::getline(exampleData, linedata, '\n');
    if((uint)std::stoi(linedata) >= lowerLimit &&
       (uint)std::stoi(linedata) <= upperLimit)
    {
      std::getline(exampleData, linedata, '\n');
      explode(", ", linedata, examples.dataFurigana);

      std::getline(exampleData, linedata, '\n');
      explode(", ", linedata, examples.dataKanji);

      std::getline(exampleData, linedata, '\n');
      explode(", ", linedata, examples.dataEnglish);
    }
    else
    {
      std::getline(exampleData, linedata, '\n');
      std::getline(exampleData, linedata, '\n');
      std::getline(exampleData, linedata, '\n');
    }
    std::getline(exampleData, linedata, '\n');
    std::getline(exampleData, linedata, '\n');
    std::getline(exampleData, linedata, '\n');
  }

  if(!examples.areValid()) return QtKanji::Error::FILE_ERROR;

  if(randomize) examples.shuffle();
  
  return QtKanji::Error::SUCCESS;
}

QtKanji::Error QtKanji::DataHandler::computeContainerData()
{
  indexInCardbox.clear();
  
  std::ifstream containerData{pathToContainerData};
  if(!containerData) return QtKanji::Error::FILE_ERROR;

  std::string linedata{};
  std::getline(containerData, linedata);
  if(linedata.empty()) return QtKanji::Error::EMPTY_CARDBOX;

  Strings help = explode(":", std::move(linedata));
  indexInCardbox =
    convertStringsToIntegers(std::move(help));
    
  std::sort(std::begin(indexInCardbox), std::end(indexInCardbox));

  getSortedSubarray(indexInCardbox, lowerLimit, upperLimit);
  if(indexInCardbox.empty()) return QtKanji::Error::NO_KANJI_WITHIN_RANGE;

  return QtKanji::Error::SUCCESS;
}

QtKanji::Error QtKanji::DataHandler::computeKanjiData(unsigned int ID)
{
  auto &FC = this->flashcard;
    
  std::string linedata{};
  std::ifstream kanjiData{pathToKanjiData};
  if(!kanjiData) return QtKanji::Error::FILE_ERROR;

  FC.dataImiVector.clear();
  FC.dataKunVector.clear();
  FC.dataOnVector.clear();
  while(!kanjiData.eof())
  {
    std::getline(kanjiData, linedata);
    if(linedata == std::to_string(ID))
    {
      FC.ID = ID;
      
      std::getline(kanjiData, linedata, '\n');
      if(linedata == "") FC.HID = 0;
      else               FC.HID = std::stoi(std::move(linedata));
      
      std::getline(kanjiData, linedata, '\n');
      FC.dataSign = std::move(linedata);

      std::getline(kanjiData, linedata, '\n');
      FC.dataImiVector = explode(", ", std::move(linedata));
 
      std::getline(kanjiData, linedata, '\n');
      if(linedata.empty()) FC.dataKunVector.push_back("-none-");
      else                 FC.dataKunVector = explode(", ", std::move(linedata));
 
      std::getline(kanjiData, linedata, '\n');
      if(linedata.empty()) FC.dataOnVector.push_back("-none-");
      else                 FC.dataOnVector = explode(", ", std::move(linedata));

      return QtKanji::Error::SUCCESS;
    }
    else
    {
      std::getline(kanjiData, linedata, '\n');
      std::getline(kanjiData, linedata, '\n');
      std::getline(kanjiData, linedata, '\n');
      std::getline(kanjiData, linedata, '\n');
      std::getline(kanjiData, linedata, '\n');
    }
    std::getline(kanjiData, linedata, '\n');
    std::getline(kanjiData, linedata, '\n');
    std::getline(kanjiData, linedata, '\n');
  }

  return QtKanji::Error::KANJI_NOT_FOUND;
}

unsigned int QtKanji::DataHandler::computeRandomId(bool fromCardbox, unsigned int removeFlag)
{   
  srand(time(NULL));

  unsigned int range = upperLimit - lowerLimit + 1;
  unsigned int cardboxSize = indexInCardbox.size();
  unsigned int randomIndex, randId;
  
  bool notFinished = indexContainer.size() != range + removeFlag;
  bool gotAlreadyIndex{};
  if(fromCardbox)
  {
    gotAlreadyIndex = true;
    while(gotAlreadyIndex && notFinished)
    {
      randomIndex = rand()%(cardboxSize);
      randId = indexInCardbox[randomIndex];
      gotAlreadyIndex = contains(indexContainer, randId);
    }	
  }
  else
  {
    gotAlreadyIndex = true;
    while(gotAlreadyIndex && notFinished)
    {
      randId = lowerLimit + rand()%range;
      gotAlreadyIndex = contains(indexContainer, randId);
    }
  }

  indexContainer.push_back(randId);

  return randId;
}

unsigned int QtKanji::DataHandler::searchKanjiId(QString kanji)
{
  const auto index = std::find(kanjiList.begin(), kanjiList.end(), kanji);
  if(index == kanjiList.end()) return 0;
  
  return std::distance(kanjiList.begin(),index)+1;
}

QtKanji::Error QtKanji::DataHandler::printExamples()
{
  return examples.print();
}
