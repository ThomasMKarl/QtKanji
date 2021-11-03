#ifndef PARSEJSON_H
#define PARSEJSON_H

namespace QtKanji
{
class JsonParser
{
public:
  JsonParser()
  {
    std::ifstream kanjiDB{"/mnt/RAID/saves/work_offload/Git/QtKanji/bin/kanjidb.json"};
    std::string kanjiDB_string = std::string{std::istreambuf_iterator<char>(kanjiDB), std::istreambuf_iterator<char>()};
    kanji_JSON.Parse(kanjiDB_string.c_str());
    assert(kanji_JSON.IsObject());
    assert(kanji_JSON.HasMember("flashcard"));

    std::ifstream examplesDB{"/mnt/RAID/saves/work_offload/Git/QtKanji/bin/examples.json"};
    std::string examplesDB_string =
        std::string{std::istreambuf_iterator<char>(examplesDB), std::istreambuf_iterator<char>()};
    examples_JSON.Parse(examplesDB_string.c_str());
    assert(examples_JSON.IsObject());
    assert(examples_JSON.HasMember("examples"));

    /*std::ifstream wordsDB{"bin/words.json"};
    std::string wordsDB_string = std::string{std::istreambuf_iterator<char>(wordsDB), std::istreambuf_iterator<char>()};
    kanji_JSON.Parse(wordsDB_string.c_str());
    assert(words_JSON.IsObject());
    assert(words_JSON.HasMember("vocabulary"));*/
  }

  unsigned int getID_kanji(unsigned int element) const
  {
    auto &JSON = kanji_JSON["flashcard"][element];
    return JSON["id"].GetUint();
  }

  unsigned int getHID_kanji(unsigned int element) const
  {
    auto &JSON = kanji_JSON["flashcard"][element];
    return JSON["hid"].GetUint();
  }

  std::string getSign_kanji(unsigned int element) const
  {
    auto &JSON = kanji_JSON["flashcard"][element];
    return JSON["kanji"].GetString();
  }

  Strings getKunVector_kanji(unsigned int element) const
  {
    Strings dataKunVector{};
    auto &JSON = kanji_JSON["flashcard"][element];
    for (const auto &input : JSON["kun"].GetArray())
      dataKunVector.push_back(input.GetString());
    if (dataKunVector[0].empty())
      dataKunVector[0] = "-none-";
    return dataKunVector;
  }

  Strings getImiVector_kanji(unsigned int element) const
  {
    Strings dataImiVector{};
    auto &JSON = kanji_JSON["flashcard"][element];
    for (const auto &input : JSON["imi"].GetArray())
      dataImiVector.push_back(input.GetString());
    return dataImiVector;
  }

  Strings getOnVector_kanji(unsigned int element) const
  {
    Strings dataOnVector{};
    auto &JSON = kanji_JSON["flashcard"][element];
    for (const auto &input : JSON["on"].GetArray())
      dataOnVector.push_back(input.GetString());
    if (dataOnVector[0].empty())
      dataOnVector[0] = "-none-";
    return dataOnVector;
  }

  unsigned int getID_examples(unsigned int element) const
  {
    auto &JSON = examples_JSON["examples"][element];
    return JSON["id"].GetUint();
  }

  std::string getFurigana_examples(unsigned int element) const
  {
    auto &JSON = examples_JSON["examples"][element];
    return JSON["furigana"].GetString();
  }

  std::string getKanji_examples(unsigned int element) const
  {
    auto &JSON = examples_JSON["examples"][element];
    return JSON["kanji"].GetString();
  }

  std::string getImi_examples(unsigned int element) const
  {
    auto &JSON = examples_JSON["examples"][element];
    return JSON["imi"].GetString();
  }
/*
  unsigned int getLection_words(unsigned int element) const
  {
    auto &JSON = examples_JSON["vocabulary"][element];
    return JSON["lection"].GetUint();
  }

  std::string getFurigana_words(unsigned int lection) const
  {
    std::string output{};
    auto &JSON = examples_JSON["vocabulary"][lection - 1];
    auto &WORD = JSON["word"]["furigana"];
    for (const auto &word : WORD)
      output.append(word.GetString());
    return output;
  }

  std::string getKanji_words(unsigned int lection) const
  {
    std::string output{};
    auto &JSON = examples_JSON["vocabulary"][lection - 1];
    auto &WORD = JSON["word"]["kanji"];
    for (const auto &word : WORD)
      output.append(word.GetString());
    return output;
  }

  std::string getImi_words(unsigned int lection) const
  {
    std::string output{};
    auto &JSON = examples_JSON["vocabulary"][lection - 1];
    auto &WORD = JSON["word"]["imi"];
    for (const auto &word : WORD)
      output.append(word.GetString());
    return output;
  }
*/
private:
  rapidjson::Document kanji_JSON{};
  rapidjson::Document examples_JSON{};
  rapidjson::Document words_JSON{};
};

static const JsonParser qtkanji_JSON{};
} // namespace QtKanji

#endif
