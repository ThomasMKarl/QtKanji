#include "datahandler.h"
#include "mainwindow.h"

bool QtKanji::DataHandler::getLimits(MainWindow &mainwindow)
{
    lowerLimit = mainwindow.displayLowerLimit.text().toInt(); //characters become zero
    upperLimit = mainwindow.displayUpperLimit.text().toInt();
    
    if(upperLimit > NUMBER_OF_KANJI)
    {
      upperLimit = NUMBER_OF_KANJI;
      mainwindow.displayUpperLimit.setText(QString::number(NUMBER_OF_KANJI));
    }
    
    if(upperLimit < lowerLimit)
    {
      mainwindow.displayLowerLimit.setText("insert integers.");
      mainwindow.displayUpperLimit.setText("");
      return false;
    }
    
    return true;
}

bool QtKanji::DataHandler::computeExampleData(MainWindow &mainwindow)
{
  dataFurigana.clear();
  dataKanji.clear();
  dataEnglish.clear();

  std::ifstream exampleData{pathToExampleData};
  if(!exampleData)
  {
    mainwindow.dataFail.show();
    return false;
  }
  mainwindow.dataFail.hide();
    
  std::string linedata{};
  while(!exampleData.eof())
  {
    std::getline(exampleData, linedata, '\n');
    if((uint)std::stoi(linedata) >= mainwindow.dataHandler->lowerLimit &&
       (uint)std::stoi(linedata) <= mainwindow.dataHandler->upperLimit)
    {
      std::getline(exampleData, linedata, '\n');
      explode(", ", linedata, dataFurigana);

      std::getline(exampleData, linedata, '\n');
      explode(", ", linedata, dataKanji);

      std::getline(exampleData, linedata, '\n');
      explode(", ", linedata, dataEnglish);
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
  if(dataKanji.empty() || dataFurigana.empty()) return false;
  if(dataKanji.size()  != dataFurigana.size() ) return false;
    
  if(mainwindow.examplesAreToRandomize())
    shuffle(dataFurigana, dataKanji, dataEnglish);
  
  truedataFurigana = dataFurigana;
  truedataKanji = dataKanji;
  
  return true;
}

bool QtKanji::DataHandler::computeContainerData(MainWindow &mainwindow)
{
  indexInCardbox.clear();
  
  std::ifstream containerData{pathToContainerData};
  if(!containerData)
  {
    mainwindow.dataFail.show();
    return false;
  }
  mainwindow.dataFail.hide();

  std::string linedata{};
  std::getline(containerData, linedata);
  if(linedata.empty())
  {
    mainwindow.cardboxFail.show();
    mainwindow.dataFail.hide();
    return false;
  }
  mainwindow.cardboxFail.hide();

  Strings help = explode(":", std::move(linedata));
  indexInCardbox =
    convertStringsToIntegers(std::move(help));

  return true;
}

void QtKanji::DataHandler::computeKanjiData(unsigned int ID)
{
  auto &FC = this->flashcard;
    
  std::string linedata{};
  std::ifstream kanjiData{pathToKanjiData};
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

      return;
    }
  }
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
