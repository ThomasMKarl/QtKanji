#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>



namespace QtKanji {
  
std::vector<std::string> explode(const std::string &delimiter, const std::string &string);

std::vector<unsigned int> convertStringsToIntegers(const std::vector<std::string> &strings);

template<typename T>
bool contains(const std::vector<T> &vector, const T &value)
{
  if (vector.empty())
     return false;
  if (find(vector.begin(), vector.end(), value) != vector.end())
     return true;
  else
     return false;
}

template<typename T>
bool oneInVector(const std::vector<T> &values,
		 const std::vector<T> &vector)
{
      if(values.empty() || vector.empty()) return false;
      for(unsigned int i = 0; i < values.size(); ++i)
      {
          if(contains(vector, values[i])) return true;
      }
      return false;
}

template<typename T>
bool allInVector(const std::vector<T> &values,
		 const std::vector<T> &vector)
{
  if(values.empty() || vector.empty()) return false;
  for(unsigned int i = 0; i < values.size(); ++i)
  {
    if(!contains(vector, values[i])) return false;
  }
  return true;
}

};

#endif

