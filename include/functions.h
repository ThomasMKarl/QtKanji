#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>
#include <memory>



namespace QtKanji {
  
class DataHandler;
using SharedData = std::shared_ptr<DataHandler>;
using Strings = std::vector<std::string>;
using Uints = std::vector<unsigned int>;
  
Strings explode(const std::string &delimiter, const std::string &string);
void explode(const std::string &delimiter, const std::string &string, Strings &result);
void shuffle(Strings &first, Strings &second, Strings &third);
Uints convertStringsToIntegers(const Strings &strings);
Uints computeContainingKanjiIndices(SharedData &dataHandler, unsigned int graphemeIndex);
std::vector<Uints> computeRadicalKanjiMap(SharedData &dataHandler);

template<typename T>
bool contains(const std::vector<T> &vector, const T &value)
{
  if (vector.empty())
     return false;
  if (find(vector.begin(), vector.end(), value) != vector.end())
     return true;
  else return false;
}

template<typename T>
bool oneInVector(const std::vector<T> &values,
		 const std::vector<T> &vector)
{
  if(values.empty() || vector.empty()) return false;
  for(size_t i = 0; i < values.size(); ++i)
    if(contains(vector, values[i])) return true;
  return false;
}

template<typename T>
bool allInVector(const std::vector<T> &values,
		 const std::vector<T> &vector)
{
  if(values.empty() || vector.empty()) return false;
  for(size_t i = 0; i < values.size(); ++i)
    if(!contains(vector, values[i])) return false;
  return true;
}

};

#endif

