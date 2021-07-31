#include "functions.h"

std::vector<std::string> QtKanji::explode(const std::string &delimiter,
					  const std::string &string)
{
  std::vector<std::string> result{};

  size_t start{0};
  size_t end{0};
 
  while((start = string.find_first_not_of(delimiter, end)) != std::string::npos)
  {
    end = string.find(delimiter, start);
    result.push_back(string.substr(start, end - start));
  }

  return result;
}

std::vector<unsigned int> QtKanji::convertStringsToIntegers(const std::vector<std::string> &strings)
{
  std::vector<unsigned int> integers{};
  for(const auto& string : strings)
  {
    unsigned int integer = std::stoi(string);
    integers.push_back(integer);
  }
  return integers;
}
