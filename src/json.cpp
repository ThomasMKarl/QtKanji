#include<string>
#include<vector>
#include<fstream>


using Strings = std::vector<std::string>;

Strings explode(const std::string &delimiter, const std::string &string)
{
  Strings result{};

  size_t start{0};
  size_t end{0};

  if(string.empty())
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

int main()
{
  std::string linedata{};
  std::ifstream kanjiData{"bin/kanjidb.dat"};
  std::ofstream out{"bin/kanjidb.json"};

  Strings dataImiVector{}, dataKunVector{}, dataOnVector{};
  std::string dataSign{};
  unsigned int ID{}, HID{};

  out << "{\n  \"flashcard\": [\n";
  
  while(!kanjiData.eof())
  {
    out << "    {\n";

      dataImiVector.clear();
      dataKunVector.clear();
      dataOnVector.clear();
    
      std::getline(kanjiData, linedata);
      ID = std::stoi(linedata);

      std::getline(kanjiData, linedata, '\n');
      if(linedata == "") HID = 0;
      else               HID = std::stoi(std::move(linedata));
      
      std::getline(kanjiData, linedata, '\n');
      dataSign = std::move(linedata);

      std::getline(kanjiData, linedata, '\n');
      dataImiVector = explode(", ", std::move(linedata));
 
      std::getline(kanjiData, linedata, '\n');
      if(linedata.empty()) dataKunVector.push_back("");
      else                 dataKunVector = explode(", ", std::move(linedata));
 
      std::getline(kanjiData, linedata, '\n');
      if(linedata.empty()) dataOnVector.push_back("");
      else                 dataOnVector = explode(", ", std::move(linedata));
      
    std::getline(kanjiData, linedata, '\n');
    std::getline(kanjiData, linedata, '\n');
    std::getline(kanjiData, linedata, '\n');

    out << "      \"id\": "  << ID  << ",\n";
    out << "      \"hid\": " << HID << ",\n";
    out << "      \"kanji\": \"" << dataSign << "\",\n";

    std::string help{};
    out << "      \"imi\": [";
    for(const auto &o : dataImiVector)
      help += "\"" + o + "\", ";
    help.erase(help.end()-2,help.end());
    out << std::move(help);
    out << "],\n";
    
    help.clear();
    out << "      \"kun\": [";
    for(const auto &o : dataKunVector)
      help += "\"" + o + "\", ";
    help.erase(help.end()-2,help.end());
    out << std::move(help);
    out << "],\n";

    help.clear();
    out << "      \"on\": [";
    for(const auto &o : dataOnVector )
      help += "\"" + o + "\", ";
    help.erase(help.end()-2,help.end());
    out << std::move(help);
    out << "]\n";

    out << "    },\n";
  }
  
  out << "  ]\n}";
}
