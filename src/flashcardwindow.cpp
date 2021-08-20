#include "flashcardwindow.h"
#include "table.h"

QtKanji::FlashcardWindow::FlashcardWindow(bool randomize_,
			                                    bool fromCardbox_,
                                          DataHandler dataHandler_,
			                                    SharedBoxes &boxes_,
			                                    SharedTable &table_,
                                          QWidget *parent) :
  randomize(randomize_),
  fromCardbox(fromCardbox_),
  dataHandler(std::move(dataHandler_)),
  boxes(boxes_),
  table(table_),
  QWidget(parent)
{
  if(!dataHandler.fromEngToJap)
  {
    table->flashcards.push_back( this );
    flashcardWindowIndex = table->flashcards.size()-1;
  }
  else flashcardWindowIndex = -1;
 
  textfont.setPointSize(20);
  textfont.setBold(false);
  setFont(textfont);
  setWindowIcon(QIcon("kanji.ico"));

  setButtonLayout();
  
  update();

  std::ofstream containerData{dataHandler.pathToContainerData, std::ios::app};
  if(!containerData) addButton.hide();
}

QtKanji::FlashcardWindow::FlashcardWindow(unsigned int Id, QWidget *parent) :
  currentId(Id),
  QWidget(parent)
{ 
  textfont.setPointSize(20);
  textfont.setBold(false);
  setFont(textfont);
  setWindowIcon(QIcon("kanji.ico"));

  dataHandler.computeKanjiData(currentId);

  setFlashcardLayout();
  setHadamitzkyLayout(dataHandler.flashcard.HID);

  setWindowTitle("QtKanji #" + QString::fromStdString(std::to_string(currentId)));
}

void QtKanji::FlashcardWindow::submitButtonClicked()
{
  Flashcard myFlashcard{};
  
  const auto &FC = dataHandler.flashcard;
 
  std::string input{};
  
  myFlashcard.dataSign = displaySign.text().toStdString();
      
  input = displayOn.text().toStdString();
  if(input == "-none-") input.clear();
  myFlashcard.dataOnVector  = explode(", ", std::move(input));

  input  = displayKun.text().toStdString();
  if(input == "-none-") input.clear();
  myFlashcard.dataKunVector = explode(", ", std::move(input));

  input  = displayImi.text().toStdString();
  myFlashcard.dataImiVector = explode(", ", std::move(input));

  if(myFlashcard == FC) showSuccess();
  else                  showFailure();
}

void QtKanji::FlashcardWindow::continueButtonClicked()
{
  continueButton.hide();
  submitButton.show();  

  update();
}

void QtKanji::FlashcardWindow::addButtonClicked()
{
  removeButton.show();
  addButton.hide();
    
  const auto index = std::find(std::begin(dataHandler.indexInCardbox),
			                         std::end(dataHandler.indexInCardbox),
			                         currentId);
  if(index == std::end(dataHandler.indexInCardbox))
  {
    dataHandler.indexInCardbox.push_back(currentId);
      
    if(fromCardbox) --numberOfRemoves;

    std::ofstream containerData{dataHandler.pathToContainerData, std::ios::app};
    containerData << std::to_string(currentId)+":";
  }
}

void QtKanji::FlashcardWindow::removeButtonClicked()
{
  removeButton.hide();
  addButton.show();
    
  const auto index = std::find(std::begin(dataHandler.indexInCardbox),
			                         std::end(dataHandler.indexInCardbox),
			                         currentId);

  if(index == std::end(dataHandler.indexInCardbox)) return;

  std::ofstream containerData{dataHandler.pathToContainerData, std::ios::trunc};

  //std::cout << dataHandler.indexInCardbox[0] << std::endl;
  dataHandler.indexInCardbox.erase(index);
  //std::cout << dataHandler.indexInCardbox[0] << std::endl;
	
  if(fromCardbox) ++numberOfRemoves;

  if(dataHandler.indexInCardbox.empty()) containerData << "";
  else
  {
    for(const auto Id : dataHandler.indexInCardbox) containerData << std::to_string(Id)+":";
  }
}

void QtKanji::FlashcardWindow::showSuccess()
{
  const auto &FC = dataHandler.flashcard;
  
  Success       .show();
  Failure       .hide();
  continueButton.show();
  submitButton  .hide();

  ++successes;

  if(dataHandler.fromEngToJap)
  {
    QString output{""};
    for(const auto& kun : FC.dataKunVector) output += QString::fromStdString(kun) + ", ";
    output.remove(output.size()-2, 2);
    displayKun.setText(std::move(output));
    displayKun.setStyleSheet("color: green");
    
    output = "";
    for(const auto& on  : FC.dataOnVector)  output += QString::fromStdString(on) + ", ";
    output.remove(output.size()-2, 2);
    displayOn.setText(std::move(output));
    displayOn.setStyleSheet("color: green");
    
    output = "";
    for(const auto& imi : FC.dataImiVector) output += QString::fromStdString(imi) + ", ";
    output.remove(output.size()-2, 2);
    displayImi.setText(std::move(output));
    displayImi.setStyleSheet("color: green");
  }
  else
  {
    displaySign.setText(QString::fromStdString(FC.dataSign));
    displaySign.setStyleSheet("color: green");
  }
}

void QtKanji::FlashcardWindow::showFailure()
{
  const auto &FC = dataHandler.flashcard;
  
  Success       .hide();
  Failure       .show();
  continueButton.show();
  submitButton  .hide();

  ++failures;

  if(dataHandler.fromEngToJap)
  {
    QString output{""};
    for(const auto& kun : FC.dataKunVector) output += QString::fromStdString(kun) + ", ";
    output.remove(output.size()-2, 2);
    displayKun.setText(std::move(output));
    displayKun.setStyleSheet("color: red");
    
    output = "";
    for(const auto& on  : FC.dataOnVector)  output += QString::fromStdString(on)  + ", ";
    output.remove(output.size()-2, 2);
    displayOn.setText(std::move(output));
    displayOn.setStyleSheet("color: red");
    
    output = "";
    for(const auto& imi : FC.dataImiVector) output += QString::fromStdString(imi) + ", ";
    output.remove(output.size()-2, 2);
    displayImi.setText(std::move(output));
    displayImi.setStyleSheet("color: red");
  }
  else
  {
    displaySign.setText(QString::fromStdString(FC.dataSign));
    displaySign.setStyleSheet("color: red");
  }
}

void QtKanji::FlashcardWindow::showResult()
{
  adjustSize();

  double ratio =
    100.0*successes /
    (failures + successes);
  
  Success.setText("Successes:");
  Success2.setText(QString::number(successes));
  Failure.setText("Failures:");
  Failure2.setText(QString::number(failures));
  Rate.setText("Success Rate:");
  Rate2.setText(QString::number(ratio) + "%");


  cards.setText("Number of cards in box: "
                       + QString::number(dataHandler.indexInCardbox.size()));
    
  if(dataHandler.indexInCardbox.empty())
    cards.setStyleSheet("color: green");
  else
    cards.setStyleSheet("color: red");

  layout.addWidget(&Success,  0,0);
  layout.addWidget(&Failure,  1,0);
  layout.addWidget(&Success2, 0,1);
  layout.addWidget(&Failure2, 1,1);
  layout.addWidget(&Rate,     2,0);
  layout.addWidget(&Rate2,    2,1);
  layout.addWidget(&cards,    3,0);

  Success2.setStyleSheet("color: green");
  Failure2.setStyleSheet("color: red");
  if(ratio >= 85)
    Rate2.setStyleSheet("color: green" );
  if(ratio < 85 && ratio >= 50)
    Rate2.setStyleSheet("color: yellow");
  if(ratio < 50 && ratio >  15)
    Rate2.setStyleSheet("color: orange");
  if(ratio <= 15)
    Rate2.setStyleSheet("color: red"   );

  setLayout(&layout);
  setWindowTitle("QtKanji Result");
  setWindowIcon(QIcon("kanji.ico"));
}

void QtKanji::FlashcardWindow::setButtonLayout()
{
  submitButton.setFixedSize(120,50);
  connect(&submitButton,
          &QPushButton::clicked,
          this,
          &FlashcardWindow::submitButtonClicked);
  layout.addWidget(&submitButton,3,0);

  addButton.setFixedSize(300,50);
  connect(&addButton,
          &QPushButton::clicked,
	        this,
	        &FlashcardWindow::addButtonClicked);
  layout.addWidget(&addButton,4,0);

  removeButton.setFixedSize(300,50);
  connect(&removeButton,
          &QPushButton::clicked,
	        this,
	        &FlashcardWindow::removeButtonClicked);
  layout.addWidget(&removeButton,4,0);

  continueButton.setFixedSize(120,50);
  connect(&continueButton,
          &QPushButton::clicked,
	        this,
	        &FlashcardWindow::continueButtonClicked);
  layout.addWidget(&continueButton,3,0);
  continueButton.hide();
  
  setLayout(&layout);
}

void QtKanji::FlashcardWindow::setFlashcardLayout()
{
  const auto &FC = dataHandler.flashcard;
  
  displaySign.setText(QString::fromStdString(FC.dataSign));
  QFont kanjiTextfont{};
  kanjiTextfont.setPointSize(50);
  kanjiTextfont.setBold(false);
  displaySign.setFont(std::move(kanjiTextfont));
  displaySign.setFixedSize(100,100);

  QString output{""};
  for(const auto& kun : FC.dataKunVector) output += QString::fromStdString(kun) + ", ";
  output.remove(output.size()-2, 2);
  displayKun.setText(std::move(output));
  displayKun.setFixedSize(250,50);
  
  output = "";
  for(const auto& imi : FC.dataImiVector) output += QString::fromStdString(imi) + ", ";
  output.remove(output.size()-2, 2);
  displayImi.setText(std::move(output));
  displayImi.setFixedSize(500,50);
  
  output = "";
  for(const auto& on : FC.dataOnVector  ) output += QString::fromStdString(on) + ", ";
  output.remove(output.size()-2, 2);
  displayOn.setText(std::move(output));
  displayOn.setFixedSize(250,50);

  layout.addWidget(&Success,4,1);
  layout.addWidget(&Failure,4,1);
  Success.hide();
  Success.setStyleSheet("color: green");
  Failure.hide();
  Failure.setStyleSheet("color: red");

  layout.addWidget(&displaySign,2,1);
  layout.addWidget(&Sign,1,1);
  layout.addWidget(&displayImi,2,2);
  layout.addWidget(&Imi,1,2);
  layout.addWidget(&displayKun,2,3);
  layout.addWidget(&Kun,1,3);
  layout.addWidget(&displayOn,2,4);
  layout.addWidget(&On,1,4);
 
  setLayout(&layout);
}

void QtKanji::FlashcardWindow::setHadamitzkyLayout(unsigned int hadamitzkyId)
{
  if(hadamitzkyId == 0) return;

  HadamitzkyData data = HadamitzkyData::createHadamitzkyData(hadamitzkyId);

  hid.setText("HId: " + QString::number(data.ID));

  std::vector<QString> graphemes = data.getGraphemes();
  QString help{" "};
  for(const auto& grapheme : graphemes)
    help += (grapheme + "  ");
  allGraphemes.setText(std::move(help));

  if(data.firstGraphemeEqualsRadical) help = "/";
  else help = " ";
  QString graphemeIndexString{};
  for(const auto graphemeIndex : data.graphemeIndices)
  {
    graphemeIndexString = QString::number(graphemeIndex);
    help += graphemeIndexString + " ";
    if(graphemeIndexString.size() == 1) help += " ";
  }
  gids.setText(std::move(help));
  
  radicals.setText(QString::fromStdString(data.radicalString));

  layout.addWidget(&hid,3,1);
  layout.addWidget(&radicals,3,2);
  layout.addWidget(&allGraphemes,4,1);
  layout.addWidget(&gids,5,1);
  
  setLayout(&layout);
}

void QtKanji::FlashcardWindow::update()
{
  unsigned int range{};
  if(fromCardbox) range = dataHandler.indexInCardbox.size() + numberOfRemoves;
  else            range = dataHandler.upperLimit - dataHandler.lowerLimit + 1;

  if(successes + failures == range)
  {
    hideAll();
    showResult();
    return;
  }

  if(randomize) currentId = dataHandler.computeRandomId(fromCardbox, numberOfRemoves);
  else          
  {
    currentId = dataHandler.lowerLimit + successes+failures;
    if(fromCardbox) currentId = dataHandler.indexInCardbox[currentId-1];
  }

  dataHandler.computeKanjiData(currentId);

  setFlashcardLayout();

  if(contains(dataHandler.indexInCardbox, currentId))
  {   
    addButton.hide();
    removeButton.show();
  }
  else
  {
    addButton.show();
    removeButton.hide();
  }

  if(dataHandler.getFromEngToJap())
  {
    if(!boxes->hideKun) displayKun.clear();
    if(!boxes->hideOn )  displayOn.clear();
    if(!boxes->hideImi) displayImi.clear();
  }
  else displaySign.clear();

  if(boxes->hideImi)
  {
    displayImi.hide();
    Imi.hide();
  }
  if(boxes->hideKun)
  {
    displayKun.hide();
    Kun.hide();
  }
  if(boxes->hideOn)
  {
    displayOn.hide();
    On.hide();
  }

  adjustSize();

  setWindowTitle("QtKanji #" + QString::fromStdString(std::to_string(currentId)));
}

void QtKanji::FlashcardWindow::hideAll()
{
    displaySign.hide();
    displayImi.hide();
    displayKun.hide();
    displayOn.hide();
    Sign.hide();
    Imi.hide();
    Kun.hide();
    On.hide();
    submitButton.hide();
    addButton.hide();
    continueButton.hide();
    removeButton.hide();
}

QtKanji::FlashcardWindow::~FlashcardWindow()
{
  if(flashcardWindowIndex != -1)
    table->flashcards.erase(std::begin(table->flashcards)+flashcardWindowIndex);
}