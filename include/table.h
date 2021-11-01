#ifndef TABLE_H
#define TABLE_H

#include "headers.h"

#include "flashcardwindow.h"

namespace QtKanji
{

struct Coord
{
  explicit Coord() = default;
  explicit Coord(unsigned int row_, unsigned int column_) : row(row_), column(column_){}
  unsigned int row{};
  unsigned int column{};
};

class Table : public QWidget
{
  Q_OBJECT

public:
  static Table *createTable(QWidget *parent = 0)
  {
    return new Table{parent};
  }
  explicit Table(QWidget *parent = 0);

  QLineEdit *search{};

  FlashcardWindows flashcards{};

  Table(const Table &) = delete;
  Table &operator=(const Table &) = delete;
  Table(Table &&) = delete;
  Table &operator=(Table &&) = delete;
  ~Table() = default;

private slots:
  void kanjiClicked(const unsigned int kanji);

private:
  Coord getCoordinate(unsigned int index);

  Buttons kanjiButtons{NUMBER_OF_KANJI};
  QGridLayout layout{};
};

}; // namespace QtKanji

#endif
