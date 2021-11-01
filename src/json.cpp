#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace json
{

using Strings = std::vector<std::string>;

int writeFlashcards(const std::string &path);
int writeWords(const std::string &path);
int writeExamples(const std::string &path);
int convertAsciiToJson(const std::string &path);

Strings explode(const std::string &delimiter, const std::string &string);

} // namespace json

int main(int argc, char **argv)
{
  std::string linedata{};
  std::string path{};
  if (argc == 2)
  {
    path = argv[1];
    path += "/";
  }
  if (argc > 2)
  {
    std::cerr << "Usage: " << argv[0] << " <folder>"
              << "\n";
    return EXIT_FAILURE;
  }

  return json::convertAsciiToJson(path);
}

namespace json
{

int convertAsciiToJson(const std::string &path)
{
  int err = writeFlashcards(path);
  if (err != EXIT_SUCCESS)
    return err;

  err = writeWords(path);
  if (err != EXIT_SUCCESS)
    return err;

  return writeExamples(path);
}

int writeFlashcards(const std::string &path)
{
  std::ifstream kanjiData{path + "kanjidb.dat"};
  if (!kanjiData)
  {
    std::cerr << "Error: " << path + "kanjidb.dat not found!\n";
    return EXIT_FAILURE;
  }

  std::ofstream out{path + "examples.json"};
  if (!out)
  {
    std::cerr << "Error: could not write " << path + "examples.json!\n";
    return EXIT_FAILURE;
  }

  Strings dataImiVector{}, dataKunVector{}, dataOnVector{};
  std::string dataSign{}, linedata{};
  unsigned int ID{}, HID{};

  out << "{\n  \"$schema\": \"http://json-schema.org/draft-04/schema#\",\n  \"title\": \"QtKanji Database\",\n  "
         "\"description\": \"contains flashcards\",\n  \"type\": \"object\",\n";

  out << "  \"flashcard\": [\n";

  while (!kanjiData.eof())
  {
    out << "    {\n";

    dataImiVector.clear();
    dataKunVector.clear();
    dataOnVector.clear();

    std::getline(kanjiData, linedata);
    ID = std::stoi(linedata);

    std::getline(kanjiData, linedata, '\n');
    if (linedata.empty())
      HID = 0;
    else
      HID = std::stoi(linedata);

    std::getline(kanjiData, linedata, '\n');
    dataSign = std::move(linedata);

    std::getline(kanjiData, linedata, '\n');
    dataImiVector = explode(", ", linedata);

    std::getline(kanjiData, linedata, '\n');
    if (linedata.empty())
      dataKunVector.push_back("");
    else
      dataKunVector = explode(", ", linedata);

    std::getline(kanjiData, linedata, '\n');
    if (linedata.empty())
      dataOnVector.push_back("");
    else
      dataOnVector = explode(", ", linedata);

    std::getline(kanjiData, linedata, '\n');
    std::getline(kanjiData, linedata, '\n');
    std::getline(kanjiData, linedata, '\n');

    out << "      \"id\": " << ID << ",\n";
    out << "      \"hid\": " << HID << ",\n";
    out << "      \"kanji\": \"" << dataSign << "\",\n";

    std::string help{};
    out << "      \"imi\": [";
    for (const auto &o : dataImiVector)
      help += "\"" + o + "\", ";
    help.erase(help.end() - 2, help.end());
    out << help;
    out << "],\n";

    help.clear();
    out << "      \"kun\": [";
    for (const auto &o : dataKunVector)
      help += "\"" + o + "\", ";
    help.erase(help.end() - 2, help.end());
    out << help;
    out << "],\n";

    help.clear();
    out << "      \"on\": [";
    for (const auto &o : dataOnVector)
      help += "\"" + o + "\", ";
    help.erase(help.end() - 2, help.end());
    out << help;
    out << "]\n";

    if (ID == 540)
      out << "    }\n  ]";
    else
      out << "    },\n";
  }

  out << "\n}";

  return EXIT_SUCCESS;
}

int writeWords(const std::string &path)
{
  std::ifstream wordsData{path + "words.dat"};
  if (!wordsData)
  {
    std::cerr << "Error: " << path + "words.dat not found!\n";
    return EXIT_FAILURE;
  }
  std::ofstream out{path + "words.json"};
  if (!out)
  {
    std::cerr << "Error: could not write " << path + "words.json!\n";
    return EXIT_FAILURE;
  }

  out << "{\n  \"$schema\": \"http://json-schema.org/draft-04/schema#\",\n  \"title\": \"QtKanji Vocabulary\",\n  "
         "\"description\": \"contains vocabulary from Minna no Nihingo lections 1-50, 51 may be extended\",\n  "
         "\"type\": \"object\",";
  out << "  \"vocabulary\": [\n";

  Strings dataFuriganaVector{}, dataKanjiVector{}, dataImiVector{};
  std::string linedata{};
  unsigned int tryLection{0}, lection{0}, count{0};

  while (!wordsData.eof())
  {
    count = 0;
    dataFuriganaVector.clear();
    dataKanjiVector.clear();
    dataImiVector.clear();

    std::getline(wordsData, linedata);
    tryLection = std::stoi(linedata);

    if (tryLection != ++lection)
    {
      std::cerr << "Error at lection " << lection << "\n";
      return EXIT_FAILURE;
    }

    out << "  {\n    \"lection\": " << lection << ",\n    \"words\": [\n";

    std::getline(wordsData, linedata, '\n');
    while (true)
    {
      out << "    {\n";

      ++count;

      dataFuriganaVector = explode(", ", linedata);
      if (dataFuriganaVector.empty())
      {
        std::cerr << "Error at lection " << lection << ", word " << count << " - empty furigana\n";
        return EXIT_FAILURE;
      }

      std::getline(wordsData, linedata, '\n');
      dataKanjiVector = explode(", ", linedata);

      std::getline(wordsData, linedata, '\n');
      dataImiVector = explode(", ", linedata);
      if (dataImiVector.empty())
      {
        std::cerr << "Error at lection " << lection << ", word " << count << " - empty translation\n";
        return EXIT_FAILURE;
      }

      std::getline(wordsData, linedata, '\n');
      std::getline(wordsData, linedata, '\n');

      std::string help{};
      out << "      \"furigana\": [";
      for (const auto &o : dataFuriganaVector)
        help += "\"" + o + "\", ";
      help.erase(help.end() - 2, help.end());
      out << help;
      out << "],\n";

      help.clear();
      out << "      \"kanji\": [";
      for (const auto &o : dataKanjiVector)
        help += "\"" + o + "\", ";
      help.erase(help.end() - 2, help.end());
      out << help;
      out << "],\n";

      help.clear();
      out << "      \"imi\": [";
      for (const auto &o : dataImiVector)
        help += "\"" + o + "\", ";
      help.erase(help.end() - 2, help.end());
      out << help;
      out << "]\n";

      if (linedata.empty())
      {
        out << "    }    \n]  \n},\n";
        break;
      }
      if (wordsData.eof())
      {
        out << "    }    \n]  \n}\n";
        break;
      }

      out << "    },\n";
    }
  }

  out << "  ]\n}";

  return EXIT_SUCCESS;
}

int writeExamples(const std::string &path)
{
  std::ifstream examplesData{path + "examples.dat"};
  if (!examplesData)
  {
    std::cerr << "Error: " << path + "examples.dat not found!\n";
    return EXIT_FAILURE;
  }
  std::ofstream out{path + "examples.json"};
  if (!out)
  {
    std::cerr << "Error: could not write " << path + "examples.json!\n";
    return EXIT_FAILURE;
  }

  Strings dataFuriganaVector{}, dataKanjiVector{}, dataImiVector{};
  std::string linedata{};
  unsigned int ID{};

  out << "{\n  \"$schema\": \"http://json-schema.org/draft-04/schema#\",\n  \"title\": \"QtKanji Database\",\n  "
         "\"description\": \"contains examples\",\n  \"type\": \"object\",";
  out << "  \"examples\": [\n";

  while (!examplesData.eof())
  {
    dataFuriganaVector.clear();
    dataKanjiVector.clear();
    dataImiVector.clear();

    out << "    {\n";

    std::getline(examplesData, linedata);
    ID = std::stoi(linedata);

    std::getline(examplesData, linedata, '\n');
    dataFuriganaVector = explode(", ", linedata);

    std::getline(examplesData, linedata, '\n');
    dataKanjiVector = explode(", ", linedata);

    std::getline(examplesData, linedata, '\n');
    dataImiVector = explode(", ", linedata);

    std::getline(examplesData, linedata, '\n');
    std::getline(examplesData, linedata, '\n');
    std::getline(examplesData, linedata, '\n');

    if (dataFuriganaVector.size() != dataKanjiVector.size() || dataKanjiVector.size() != dataImiVector.size())
    {
      std::cerr << "Error at example " << ID << "\n";
      return EXIT_FAILURE;
    }

    out << "      \"id\": " << ID << ",\n";

    std::string help{};
    out << "      \"furigana\": [";
    for (const auto &o : dataFuriganaVector)
      help += "\"" + o + "\", ";
    help.erase(help.end() - 2, help.end());
    out << help;
    out << "],\n";

    help.clear();
    out << "      \"kanji\": [";
    for (const auto &o : dataKanjiVector)
      help += "\"" + o + "\", ";
    help.erase(help.end() - 2, help.end());
    out << help;
    out << "],\n";

    help.clear();
    out << "      \"imi\": [";
    for (const auto &o : dataImiVector)
      help += "\"" + o + "\", ";
    help.erase(help.end() - 2, help.end());
    out << help;
    out << "]\n";

    if (ID == 540)
      out << "    }\n  ]";
    else
      out << "    },\n";
  }

  out << "\n}";

  return EXIT_SUCCESS;
}

Strings explode(const std::string &delimiter, const std::string &string)
{
  Strings result{};

  size_t start{0};
  size_t end{0};

  if (string.empty())
  {
    result.push_back("");
    return result;
  }

  while ((start = string.find_first_not_of(delimiter, end)) != std::string::npos)
  {
    end = string.find(delimiter, start);
    result.push_back(string.substr(start, end - start));
  }

  return result;
}

} // namespace json