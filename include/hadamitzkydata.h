#ifndef GRAPHEME_H
#define GRAPHEME_H



namespace QtKanji {

class HadamitzkyData
{
 public:
  static HadamitzkyData createHadamitzkyData(unsigned int ID_)
    {return HadamitzkyData{ID_};};

  bool firstGraphemeEqualsRadical{false};
  unsigned int ID{};
  unsigned int boxId{};
  std::string radicalString{};
  std::vector<unsigned int> graphemeIndices{};

  std::vector<QString> getGraphemes()
  {
    std::vector<QString> graphemes{};
    for(unsigned short int index : graphemeIndices)
      graphemes.push_back(graphemeList[index]);

    return graphemes;
  }

 private:
  explicit HadamitzkyData(unsigned int ID_)
  {
    ID = ID_;

    if(ID_ <= 4) 
    {
      std::vector<std::string> input =
        explode("+",hadamitzkyStrings[ID_-1]);
    
      radicalString = std::move(input[0]);
      boxId = std::stoi(std::move(input[1]));

      graphemeIndices = convertStringsToIntegers( explode(",",input[2]) );

      if(std::move(input[3]) == "e") firstGraphemeEqualsRadical = true;
    }
  }
};  
  
};

#endif
