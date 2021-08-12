#ifndef GRAPHEME_H
#define GRAPHEME_H

#include "functions.h"



namespace QtKanji {

  static const std::array<QString,80> graphemeList{
  "一","丨",
  "イ","ニ","冫","孑","阝","力","又","匕","亠","十",
  "ト","イ","勹","儿","厶","厂","⻌","冂",
  "氵","土","扌","口","女","巾","犭","弓","彳","夕",
  "彡","⺿","宀","大","小","山","寸","工","ヨ","尸",
  "木","日","月","火","ネ","王","牛","方","攵","斤",
  "心","戈",
  "石","立","目","禾","⻂","田","皿","疒",
  "糸","米","舟","虫","耳","⺮",
  "言","貝","車","⻊","酉",
  "金","住","雨","門","頁","食","馬","魚","鳥"
};
  
static const Strings hadamitzkyStrings{
  "2a0.1+1+3+e",
  "0a1.1+1+1+n",
  "0a2.1+1+4+n",
  "0a3.1+2+4,1+n"
};

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
    std::vector<std::string> input =
      explode("+",hadamitzkyStrings[ID_+1]);
    
    radicalString = std::move(input[0]);
    boxId = std::stoi(std::move(input[1]));

    graphemeIndices = convertStringsToIntegers( explode(",",input[2]) );

    if(std::move(input[3]) == "e") firstGraphemeEqualsRadical = true;

    ID = ID_;
  }
};  
  
};

#endif
