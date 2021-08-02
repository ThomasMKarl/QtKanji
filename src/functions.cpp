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

void QtKanji::explode(const std::string &delimiter,
		      const std::string &string,
                      std::vector<std::string> &result)
{
  size_t start{0};
  size_t end{0};
 
  while((start = string.find_first_not_of(delimiter, end)) != std::string::npos)
  {
    end = string.find(delimiter, start);
    result.push_back(string.substr(start, end - start));
  }
}

#include<iostream>
void QtKanji::shuffle(std::vector<std::string> &first,
	              std::vector<std::string> &second,
	              std::vector<std::string> &third)
{
  srand(time(NULL));
  size_t indexToSwap{};
  for(size_t index = first.size()-1; index > 0; --index)
  {
    indexToSwap = std::rand() % (index);
    std::swap(first[index],   first[indexToSwap]);
    std::swap(second[index], second[indexToSwap]);
    std::swap(third[index],   third[indexToSwap]);
  }
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
