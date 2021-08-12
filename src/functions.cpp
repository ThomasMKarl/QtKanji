#include "functions.h"
#include "datahandler.h"
#include "hadamitzkydata.h"


QtKanji::Strings QtKanji::explode(const std::string &delimiter,
			          const std::string &string)
{
  Strings result{};

  size_t start{0};
  size_t end{0};

  if(string == "")
  {
    result.push_back("");
    return result;
  }
 
  while((start = string.find_first_not_of(delimiter, end)) != std::string::npos)
  {
    end = string.find(delimiter, start);
    result.push_back(string.substr(start, end - start));
  }

  return result;
}

void QtKanji::explode(const std::string &delimiter,
		      const std::string &string,
                      Strings &result)
{
  size_t start{0};
  size_t end{0};

  if(string == "")
  {
    result.push_back("");
    return;
  }
 
  while((start = string.find_first_not_of(delimiter, end)) != std::string::npos)
  {
    end = string.find(delimiter, start);
    result.push_back(string.substr(start, end - start));
  }
}

void QtKanji::shuffle(Strings &first,
	              Strings &second,
	              Strings &third)
{
  srand(time(NULL));
  
  size_t indexToSwap{};
  for(size_t index = first.size()-1; index > 0; --index)
  {
    indexToSwap = std::rand()%index;
    std::swap(first[index],   first[indexToSwap]);
    std::swap(second[index], second[indexToSwap]);
    std::swap(third[index],   third[indexToSwap]);
  }
}

QtKanji::Uints QtKanji::convertStringsToIntegers(const Strings &strings)
{
  Uints integers{};
  for(const auto& string : strings)
  {
    unsigned int integer = std::stoi(string);
    integers.push_back(integer);
  }
  return integers;
}

QtKanji::Uints QtKanji::computeContainingKanjiIndices(SharedData &dataHandler, unsigned int graphemeIndex)
{
  QtKanji::Uints kanjiIndices{};
  for(unsigned int index = 0; index < NUMBER_OF_GRAPHEMES; ++index)
  {
    dataHandler->computeKanjiData(index+1);
    QtKanji::HadamitzkyData data =
      QtKanji::HadamitzkyData::createHadamitzkyData(dataHandler->flashcard.HID);
    if(contains(data.graphemeIndices, graphemeIndex))
      kanjiIndices.push_back(index+1);
  }
  
  return kanjiIndices;
}

std::vector<QtKanji::Uints> QtKanji::computeRadicalKanjiMap(SharedData &dataHandler)
{
  std::vector<Uints> map{NUMBER_OF_GRAPHEMES};
  for(unsigned int index = 0; index < NUMBER_OF_GRAPHEMES; ++index)
    map[index] = QtKanji::computeContainingKanjiIndices(dataHandler, index+1);

  return map;
}
