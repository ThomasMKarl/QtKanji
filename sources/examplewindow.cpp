#include "examplewindow.h"
//#include "functions.h"

std::vector<std::string> explode2(const std::string &delimiter, const std::string &str)
{
    std::vector<std::string> arr;

    int strleng = str.length();
    int delleng = delimiter.length();
    if (delleng==0)
        return arr;//no change

    int i=0;
    int k=0;
    while( i<strleng )
    {
        int j=0;
        while (i+j<strleng && j<delleng && str[i+j]==delimiter[j])
            j++;
        if (j==delleng)//found delimiter
        {
            arr.push_back(  str.substr(k, i-k) );
            i+=delleng;
            k=i;
        }
        else
        {
            i++;
        }
    }
    arr.push_back(  str.substr(k, i-k) );
    return arr;
}

template<typename T>
bool contains2(std::vector<T> v, T x)
{
      if (v.empty())
           return false;
      if (find(v.begin(), v.end(), x) != v.end())
           return true;
      else
           return false;
}

template<typename T>
bool all_in_vec2(std::vector<T> v1, std::vector<T> v2)
{
      if(v1.empty() || v2.empty()) return false;
      for(unsigned int i = 0; i < v1.size(); i++)
      {
          if(!contains(v2, v1[i])) return false;
      }
      return true;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

Example::Example(std::vector<std::string> in_dataFuri,
                 std::vector<std::string> in_dataKan,
                 std::vector<std::string> in_dataEng, std::vector<unsigned int> numbers, std::string direction, unsigned int in_failures, unsigned int in_successes, QWidget *parent) : QWidget(parent)
{
    mDirection = direction;

    mNumbers = numbers;

    mSuccesses = in_successes;
    mFailures  = in_failures;

    dataFuri     = in_dataFuri;
    truedataFuri = in_dataFuri;
    dataKan      = in_dataKan;
    truedataKan  = in_dataKan;
    dataEng      = in_dataEng;

    QGridLayout *exlayout=new QGridLayout;

    submitButton = new QPushButton("submit");
    connect(submitButton, &QPushButton::clicked, this, &Example::submitButtonClicked);
    exlayout->addWidget(submitButton,1,2);

    textfont.setPointSize(20);
    textfont.setBold(false);

    this->setFont(textfont);

////////////////////////////////////////////////////////////////////////////////////////////////

    randId = rand()%dataFuri.size();
    if(mNumbers.size() == dataFuri.size())
    {
        double ratio = 100.0*mSuccesses/(mFailures+mSuccesses);

        QGridLayout *exlayout=new QGridLayout;
        Success  = new QLabel("Successes:", this);
        Success2 = new QLabel(QString::number(mSuccesses), this);
        Failure  = new QLabel("Failures:", this);
        Failure2 = new QLabel(QString::number(mFailures), this);
        Rate     = new QLabel("Success Rate:", this);
        Rate2    = new QLabel(QString::number(ratio) + "%", this);
        exlayout->addWidget(Success,0,0);
        exlayout->addWidget(Failure,1,0);
        exlayout->addWidget(Success2,0,1);
        exlayout->addWidget(Failure2,1,1);
        exlayout->addWidget(Rate, 2,0);
        exlayout->addWidget(Rate2,2,1);

        Success2->setStyleSheet("color: green");
        Failure2->setStyleSheet("color: red");
        if(ratio >= 85)               Rate2->setStyleSheet("color: green" );
        if(ratio < 85 && ratio >= 50) Rate2->setStyleSheet("color: yellow");
        if(ratio < 50 && ratio >  15) Rate2->setStyleSheet("color: orange");
        if(ratio <= 15)               Rate2->setStyleSheet("color: red"   );

        this->setLayout(exlayout);
        this->setWindowTitle("QtKanji result");
        this->setWindowIcon(QIcon("kanji.ico"));

        this->resize(60,60);
    }
    else
    {
        while(contains2(mNumbers, randId) && mNumbers.size() != dataFuri.size())
        {
            randId = rand()%dataFuri.size();
        }
        mNumbers.push_back(randId);

////////////////////////////////////////////////////////////////////////////////////////////////
        //set default values in displays

        if(mDirection == "engjap")
        {
            displayFuri= new QLineEdit(this);
            Furi = new QLabel("Furigana:", this);

            exlayout->addWidget(displayFuri,1,1);

            Kan  = new QLabel(QString::fromStdString("Kanji: "), this);
            Kan2 = new QLabel(QString::fromStdString(dataKan[randId]), this);
            exlayout->addWidget(Kan2,2,1);
        }
        else
        {
            Furi  = new QLabel(QString::fromStdString("Furigana: "), this);
            Furi2 = new QLabel(QString::fromStdString(dataFuri[randId]), this);
            exlayout->addWidget(Furi2,1,1);

            displayKan= new QLineEdit(this);
            Kan = new QLabel("Kanji:", this);

            exlayout->addWidget(displayKan,2,1);
        }

        Eng  = new QLabel(QString::fromStdString("Translation: "), this);
        Eng2 = new QLabel(QString::fromStdString(dataEng[randId]), this);

        Success = new QLabel("Success!", this);
        Failure = new QLabel("Failure!", this);
        exlayout->addWidget(Success,4,0);
        exlayout->addWidget(Failure,4,0);
        Success->hide();
        Success->setStyleSheet("color: green");
        Failure->hide();
        Failure->setStyleSheet("color: red");

        contButton = new QPushButton("continue");
        connect(contButton, &QPushButton::clicked, this, &Example::contButtonClicked);
        exlayout->addWidget(contButton,1,2);
        contButton->hide();

        exlayout->addWidget(Furi,1,0);
        exlayout->addWidget(Kan,2,0);
        exlayout->addWidget(Eng,3,0);
        exlayout->addWidget(Eng2,3,1);

        this->setLayout(exlayout);

        this->resize(600,100);
        this->setWindowTitle("QtKanji example #" + QString::number(mNumbers.size()) + " of " + QString::number(dataFuri.size()));
        this->setWindowIcon(QIcon("kanji.ico"));
    }
}

void Example::submitButtonClicked()
{
    if(mDirection == "engjap")
    {
        dataFuri[randId] = displayFuri->text().toStdString();

        if(dataFuri[randId] == truedataFuri[randId])
        {//SUCCSESS
            Success   ->show();
            Failure   ->hide();
            contButton->show();

            displayFuri->setText(QString::fromStdString(truedataFuri[randId]));

            displayFuri->setStyleSheet("color: green");
            displayFuri->setFont(textfont);

            mSuccesses += 1;

            submitButton->hide();
        }
        else
        {//FAILURE
            Success   ->hide();
            Failure   ->show();
            contButton->show();

            displayFuri->setText(QString::fromStdString(truedataFuri[randId]));

            displayFuri->setStyleSheet("color: red");
            displayFuri->setFont(textfont);

            mFailures += 1;

            submitButton->hide();
        }
    }
    else
    {
        dataKan[randId] = displayKan ->text().toStdString();

        if(dataKan[randId] == truedataKan[randId])
        {//SUCCSESS
            Success   ->show();
            Failure   ->hide();
            contButton->show();

            displayKan ->setText(QString::fromStdString(truedataKan[randId]));

            displayKan ->setStyleSheet("color: green");
            displayKan ->setFont(textfont);

            mSuccesses += 1;

            submitButton->hide();
        }
        else
        {//FAILURE
            Success   ->hide();
            Failure   ->show();
            contButton->show();

            displayKan ->setText(QString::fromStdString(truedataKan[randId]));

            displayKan ->setStyleSheet("color: red");
            displayKan ->setFont(textfont);

            mFailures += 1;

            submitButton->hide();
        }
    }
}

void Example::contButtonClicked()
{
    this->close();
    Example *ex = new Example(truedataFuri, truedataKan, dataEng, mNumbers, mDirection, mFailures, mSuccesses);
    ex->show();
}
