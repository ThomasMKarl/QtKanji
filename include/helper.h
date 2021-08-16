#ifndef HELPER_H
#define HELPER_H

#include "headers.h"



namespace QtKanji {

  struct Flashcard
  {
    explicit Flashcard() = default;

    unsigned int  ID{};
    unsigned int HID{};
    std::string dataSign{};
    Strings dataImiVector{}, dataOnVector{}, dataKunVector{};
    
    bool operator==(const Flashcard& rhs)
    {
      if(dataSign      != rhs.dataSign)      return false;
      if(dataImiVector != rhs.dataImiVector) return false;
      if(dataOnVector  != rhs.dataOnVector)  return false;
      if(dataKunVector != rhs.dataKunVector) return false;
      return true;
    }
  };

  struct Examples
  {
    explicit Examples() = default;

    Strings dataFurigana{}, dataKanji{}, dataEnglish{};
    
    bool furiganaMatches(const unsigned int Id, const std::string furigana) const;
    bool kanjiMatches(const unsigned int Id, const std::string kanji) const;
    bool areValid() const;
    void clear();
    Error print() const;
    void shuffle();
  };

  Strings explode(const std::string &delimiter, const std::string &string);
  void explode(const std::string &delimiter, const std::string &string, Strings &result);
  void shuffle(Strings &first, Strings &second, Strings &third);
  Uints convertStringsToIntegers(const Strings &strings);
  Uints computeContainingKanjiIndices(SharedData &dataHandler, unsigned int graphemeIndex);
  std::vector<Uints> computeRadicalKanjiMap(SharedData &dataHandler);

  template<typename T>
  void getSortedSubarray(std::vector<T> &array, const size_t lowerLimit, const size_t upperLimit)
  {
    if(upperLimit < lowerLimit) return;

    size_t counter{0};
    for(size_t index = 0; index < array.size(); ++index)
    {
      if(array[index] > upperLimit) break;
      if(array[index] >= lowerLimit) 
      {
        array[counter] = std::move(array[index]);
        ++counter;
      }

      array.resize(counter);
    }
  }

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

  template<typename T>
  std::set<T> overlap(const std::vector<T> &copy0, const std::vector<T> &copy1, const std::vector<T> &copy2)
  {
    std::set<T> result{};

    bool in1{}, in2{};
    for(const auto &index : copy0)
    {
      if(!copy1.empty())
      {
        if(contains(copy1,index)) in1 = true;
        else                      in1 = false;
      }
      else in1 = true;

      if(!copy2.empty())
      {
        if(contains(copy2,index)) in2 = true;
        else                      in2 = false;
      }
      else in2 = true;

      if(in1 && in2) result.insert(index);
    }

    return result;
  }
};

#endif