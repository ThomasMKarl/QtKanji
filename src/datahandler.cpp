#include "datahandler.h"
#include "mainwindow.h"

bool QtKanji::DataHandler::getLimits(MainWindow &mainwindow)
{
    lowerLimit = mainwindow.displayLowerLimit->text().toInt(); //characters become zero
    upperLimit = mainwindow.displayUpperLimit->text().toInt();
    
    if(upperLimit > 460) upperLimit = 460;
    
    if(upperLimit < lowerLimit)
    {
        mainwindow.displayLowerLimit->setText("insert integers.");
        mainwindow.displayUpperLimit->setText("");
        return false;
    }
    
    return true;
}

bool QtKanji::DataHandler::computeExampleData(MainWindow &mainwindow)
{
  std::string lowerLimitString =
    std::to_string(mainwindow.dataHandler->lowerLimit);
  std::string upperLimitString =
    std::to_string(mainwindow.dataHandler->upperLimit);

  std::ifstream exampleData{pathToExampleData};
  if(!exampleData)
  {
    mainwindow.dataFail->show();
    return false;
  }
  mainwindow.dataFail->hide();
    
  std::string linedata{};
  while(!exampleData.eof())
  {
    std::getline(exampleData, linedata, '\n');
    if(linedata >= lowerLimitString &&
       linedata <= upperLimitString)
    {
      std::getline(exampleData, linedata, '\n');
      dataFurigana = explode(", ", linedata);
      truedataFurigana = dataFurigana;

      std::getline(exampleData, linedata, '\n');
      dataKanji = explode(", ", linedata);
      truedataKanji = dataKanji;

      std::getline(exampleData, linedata, '\n');
      dataEnglish = explode(", ", linedata);
    }
  }

  return true;
}

bool QtKanji::DataHandler::computeContainerData(MainWindow &mainwindow, bool fromCardbox)
{
  indexInCardbox.clear();
  
  std::ifstream containerData{pathToContainerData};
  if(!containerData)
  {
    mainwindow.dataFail->show();
    return false;
  }
  mainwindow.dataFail->hide();

  std::string linedata{};
  std::getline(containerData, linedata);
  if(linedata.empty() && fromCardbox)
  {
    mainwindow.cardboxFail->show();
    mainwindow.dataFail->hide();
    return false;
  }
  mainwindow.cardboxFail->hide();

  std::vector<std::string> help = explode(":", std::move(linedata));
  indexInCardbox =
    convertStringsToIntegers(std::move(help));

  return true;
}

void QtKanji::DataHandler::computeKanjiData(Flashcard &flashcard)
{
  std::string linedata{};
  unsigned int linecount{0};
  std::ifstream kanjiData{pathToKanjiData};
  while(!kanjiData.eof())
  {
    std::getline(kanjiData, linedata);
    ++linecount;
    if(linedata == std::to_string(flashcard.randId))
    {
      std::getline(kanjiData, linedata, '\n');
      dataSign = linedata;
      truedataSign = dataSign;

      std::getline(kanjiData, linedata, '\n');
      dataImi = linedata;
      if(flashcard.boxes->hideImi) dataImi.clear();
      dataImiVector = explode(", ", dataImi);
      truedataImi = dataImi;
      truedataImiVector = dataImiVector;

      std::getline(kanjiData, linedata, '\n');
      if(linedata.empty()) dataKun = "-none-";
      else                 dataKun = linedata;

      if(flashcard.boxes->hideKun) dataKun.clear();
      dataKunVector = explode(", ", dataKun);
      truedataKun = dataKun;
      truedataKunVector = dataKunVector;

      std::getline(kanjiData, linedata, '\n');
      if(linedata.empty()) dataOn = "-none-";
      else dataOn = linedata;

      if(flashcard.boxes->hideOn) dataOn.clear();
      dataOnVector = explode(", ", dataOn);
      truedataOn = dataOn;
      truedataOnVector = dataOnVector;

      linecount += 4;
    }
  }
}
