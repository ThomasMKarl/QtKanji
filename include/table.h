#ifndef TABLE_H
#define TABLE_H

#include "headers.h"

#include "datahandler.h"
#include "flashcard.h"



namespace QtKanji {

using Buttons = std::vector<std::unique_ptr<QPushButton>>;
 
class Table : public QWidget
{
  Q_OBJECT
  
 public:
  static Table* createTable(QWidget *parent = 0) {return new Table{parent};}

  Buttons kanjiButtons{};
  std::unique_ptr<QGridLayout> layout{};
  std::vector<std::string> tableData{};
  
 private slots:
  void kanjiClicked(const std::string kanji);
   
 private:
  explicit Table(QWidget *parent = 0);
};

};

#endif
