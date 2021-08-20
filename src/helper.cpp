#include "headers.h"
#include "datahandler.h"
#include "hadamitzkydata.h"

bool QtKanji::Examples::furiganaMatches(const unsigned int Id, const std::string furigana) const
{
  if(furigana != dataFurigana[Id-1]) return false;
  return true;
}

bool QtKanji::Examples::kanjiMatches(const unsigned int Id, const std::string kanji) const
{
  if(kanji    != dataKanji[Id-1])    return false;
  return true;
}

bool QtKanji::Examples::areValid() const
{
  if(dataKanji.empty() || dataFurigana.empty()) return false;
  if(dataKanji.size()  != dataFurigana.size() ) return false;
  if(dataKanji.size()  !=  dataEnglish.size() ) return false;

  return true;
}

void QtKanji::Examples::clear()
{
  dataFurigana.clear();
  dataKanji.clear();
  dataEnglish.clear();
}

QtKanji::Error QtKanji::Examples::print() const
{
  std::ofstream kanjiFile{"examples_kanji.html"};
  std::ofstream furiganaFile{"examples_furigana.html"};
  if(!kanjiFile || !furiganaFile) return Error::FILE_ERROR;

  const std::string header{"<!DOCTYPE html>\n<html>\n<head>\n<title>Kanji Examples</title>\n<meta charset='UTF-8'>\n</head>\n\n<body>\n<h1>Kanji Examples</h1>\n"};
  const std::string footer{"\n</body>\n</html>"};
  
  kanjiFile << header;
  furiganaFile << header;
  
  size_t counter{0};
  for(const std::string &kanji    : dataKanji)
  {
    if(counter%10 == 0) kanjiFile << "<p>\n" << counter/10+1 << ".  ";
    kanjiFile << kanji << "  +  ";
    ++counter;
  }

  counter = 0;
  for(const std::string &furigana : dataFurigana)
  {
    if(counter%10 == 0) furiganaFile << "<p>\n" << counter/10+1 << ".  ";
    furiganaFile << furigana << "  +  ";
    ++counter;
  }

  kanjiFile << footer;
  furiganaFile << footer;

  return QtKanji::Error::SUCCESS;
}

void QtKanji::Examples::shuffle()
{
  srand(time(NULL));
  
  size_t indexToSwap{};
  for(size_t index = dataFurigana.size()-1; index > 0; --index)
  {
    indexToSwap = std::rand()%index;
    std::swap(dataFurigana[index], dataFurigana[indexToSwap]);
    std::swap(dataKanji[index],    dataKanji[indexToSwap]);
    std::swap(dataEnglish[index],  dataEnglish[indexToSwap]);
  }
}


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

QtKanji::Uints QtKanji::computeContainingKanjiIndices(unsigned int graphemeIndex, 
                                                      std::vector<std::map<unsigned int, unsigned int>> &radicalStrokeNumberMaps)
{
  DataHandler dataHandler{};

  std::map<unsigned int, unsigned int> strokeMap{};

  QtKanji::Uints kanjiIndices{};
  QtKanji::Uints radicalStrokeNumbers{};
  for(unsigned int index = 1; index <= 4; ++index)
  {
    dataHandler.computeKanjiData(index);
    QtKanji::HadamitzkyData data =
      QtKanji::HadamitzkyData::createHadamitzkyData(dataHandler.flashcard.HID);
      
    if(contains(data.graphemeIndices, graphemeIndex))
      kanjiIndices.push_back(index);

    strokeMap.insert({data.strokeNumber,index});
    radicalStrokeNumbers.push_back(data.radicalStrokeNumber);
  }
  
  radicalStrokeNumberMaps.resize(29);
  //for(const auto & [strokeNumber,index] : strokeMap)
    //radicalStrokeNumberMaps[strokeNumber-1].insert({1,1});

  return kanjiIndices;
}

std::vector<QtKanji::Uints> QtKanji::computeRadicalKanjiMap(std::vector<std::map<unsigned int, unsigned int>> &radicalStrokeNumberMaps)
{
  std::vector<Uints> map{NUMBER_OF_GRAPHEMES};
  for(unsigned int index = 1; index <= NUMBER_OF_GRAPHEMES; ++index)
    map[index-1] = QtKanji::computeContainingKanjiIndices(index, radicalStrokeNumberMaps);

  return map;
}
