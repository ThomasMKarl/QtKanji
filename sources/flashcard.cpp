#include "flashcard.h"
//#include "functions.h"

std::vector<std::string> explode(const std::string &delimiter, const std::string &str)
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
bool contains(std::vector<T> v, T x)
{
      if (v.empty())
           return false;
      if (find(v.begin(), v.end(), x) != v.end())
           return true;
      else
           return false;
}

template<typename T>
bool one_in_vec(std::vector<T> v1, std::vector<T> v2)
{
      if(v1.empty() || v2.empty()) return false;
      for(unsigned int i = 0; i < v1.size(); i++)
      {
          if(!contains(v2, v1[i])) return false;
      }
      return true;
}

template<typename T>
bool all_in_vec(std::vector<T> v1, std::vector<T> v2)
{
    if(v1.empty() || v2.empty()) return false;
    if(v1.size() != v2.size())   return false;
    for(unsigned int i = 0; i < v1.size(); i++)
    {
        if(!contains(v2, v1[i])) return false;
    }
    return true;
}

//////////////////////////////////////////////////////
Flashcard::Flashcard(unsigned int LL, unsigned int UL, std::string direction, std::vector<unsigned int> numbers, unsigned int in_successes, unsigned int in_failures,
                                                                              std::vector<unsigned int> cardbox, int cont_flag, int rem_flag, bool hideimi, bool hidekun, bool hideon, QWidget *parent) : QWidget(parent)
{   
    mContFlag = cont_flag;

    mRemFlag = rem_flag;

    mDirection = direction;

    mNumbers = numbers;

    mCardbox = cardbox;

    mSuccesses = in_successes;
    mFailures  = in_failures;

    mHideimi = hideimi;
    mHidekun = hidekun;
    mHideon  = hideon ;

    std::string randIdstring;
    if(cont_flag && cardbox.size()!=0)
    {
        if(cardbox.size() == 1)
        {
            randId = 0;
        }
        else
        {
            randId = rand()%(cardbox.size()-1);
        }
        randId = cardbox[randId];
        randIdstring = std::to_string(randId);

        mLL = 0;
        mUL = cardbox.size()-1;
    }
    else
    {
        mLL = LL;
        mUL = UL;

        randId = LL+rand()%(UL-LL+1);
        randIdstring = std::to_string(randId);
    }

    std::vector<unsigned int> list;
    for(unsigned int i = 245; i <= 460; i++) list.push_back(i);

    textfont.setPointSize(20);
    textfont.setBold(false);

    this->setFont(textfont);

    int zero_flag = 0;
    if(cont_flag && mCardbox.size()==0) zero_flag = 1;
    if(mNumbers.size() == UL-LL+1+rem_flag || zero_flag)
    {
        double ratio = 100.0*mSuccesses/(mFailures+mSuccesses);

        QGridLayout *exlayout=new QGridLayout;
        Success  = new QLabel("Successes:", this);
        Success2 = new QLabel(QString::number(mSuccesses), this);
        Failure  = new QLabel("Failures:", this);
        Failure2 = new QLabel(QString::number(mFailures), this);
        Rate     = new QLabel("Success Rate:", this);
        Rate2    = new QLabel(QString::number(ratio) + "%", this);

        cards = new QLabel("Number of cards in box: " + QString::number(cardbox.size()), this);
        if(cardbox.size() == 0) cards->setStyleSheet("color: green");
        else cards->setStyleSheet("color: red");

        exlayout->addWidget(Success,0,0);
        exlayout->addWidget(Failure,1,0);
        exlayout->addWidget(Success2,0,1);
        exlayout->addWidget(Failure2,1,1);
        exlayout->addWidget(Rate, 2,0);
        exlayout->addWidget(Rate2,2,1);
        exlayout->addWidget(cards,3,0);

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
        if(cont_flag) list = cardbox;
        while((contains(mNumbers, randId) || !contains(list, randId)) && mNumbers.size() != UL-LL+1+rem_flag)
        {
            //if(!contains(list, randId) && !contains(mNumbers, randId)) mNumbers.push_back(randId);
            //mNumbers.push_back(randId);

            randId = LL+rand()%(UL-LL+1);
            if(cont_flag) randId = cardbox[randId];
            randIdstring = std::to_string(randId);
        }
        mNumbers.push_back(randId);

        //////////////////////////////////////////////////////////////////////////////

        std::ifstream kanji("../kanjidb.dat");
        std::string linedata = "";
        unsigned int linecount = 0;
        while (!kanji.eof())
        {
            std::getline(kanji, linedata);
            linecount += 1;
            if(linedata == randIdstring)
            {
                std::getline(kanji, linedata, '\n');
                dataSign = linedata;
                truedataSign = dataSign;

                std::getline(kanji, linedata, '\n');
                dataImi = linedata;
                if(mHideimi) dataImi = "";
                dataImivec = explode(", ", dataImi);
                truedataImi = dataImi;
                truedataImivec = dataImivec;

                std::getline(kanji, linedata, '\n');
                if(linedata=="")
                {
                    dataKun = "-none-";
                }
                else
                {
                    dataKun = linedata;
                }
                if(mHidekun) dataKun = "";
                dataKunvec = explode(", ", dataKun);
                truedataKun = dataKun;
                truedataKunvec = dataKunvec;

                std::getline(kanji, linedata, '\n');
                if(linedata=="")
                {
                    dataOn = "-none-";
                }
                else
                {
                    dataOn = linedata;
                }
                if(mHideon) dataOn = "";
                dataOnvec = explode(", ", dataOn);
                truedataOn = dataOn;
                truedataOnvec = dataOnvec;

                linecount += 4;

                kanji.close();
                break;
            }
        }
        kanji.close();

        ////////////////////////////////////////////////////////////////////////

        QGridLayout *flashlayout=new QGridLayout;

        submitButton = new QPushButton("submit");
        submitButton->setFixedSize(120,50);
        connect(submitButton, &QPushButton::clicked, this, &Flashcard::submitButtonClicked);
        flashlayout->addWidget(submitButton,3,0);

        addButton = new QPushButton("add to cardbox");
        addButton->setFixedSize(270,50);
        connect(addButton, &QPushButton::clicked, this, &Flashcard::addButtonClicked);
        flashlayout->addWidget(addButton,4,0);

        remButton = new QPushButton("remove from cardbox");
        remButton->setFixedSize(270,50);
        connect(remButton, &QPushButton::clicked, this, &Flashcard::remButtonClicked);
        flashlayout->addWidget(remButton,4,0);

        if(contains(cardbox, randId)) addButton->hide();
        else remButton->hide();

        contButton = new QPushButton("continue");
        contButton->setFixedSize(120,50);
        connect(contButton, &QPushButton::clicked, this, &Flashcard::contButtonClicked);
        flashlayout->addWidget(contButton,3,0);
        contButton->hide();

        if(direction=="japeng")
        {
            if(!mHidekun) dataKun = "";
            if(!mHideon ) dataOn  = "";
            if(!mHideimi) dataImi = "";
        }
        if(direction=="engjap")
        {
            dataSign = "";
        }

        ///////////////////////////////////////////////////////////////////////

        //set default values in displays
        Id = new QLabel(QString::fromStdString("ID: " + randIdstring), this);

        displaySign= new QLineEdit(this);
        Sign = new QLabel("漢字:", this);
        displaySign->setText(QString::fromStdString(dataSign));
        textfont.setPointSize(50);
        textfont.setBold(false);
        displaySign->setFont(textfont);
        displaySign->setFixedSize(100,100);

        displayImi= new QLineEdit(this);
        Imi = new QLabel("いみ:", this);
        displayImi->setText(QString::fromStdString(dataImi));
        displayImi->setFixedSize(500,50);

        displayKun= new QLineEdit(this);
        Kun = new QLabel("くん:", this);
        displayKun->setText(QString::fromStdString(dataKun));
        displayKun->setFixedSize(250,50);

        displayOn= new QLineEdit(this);
        On = new QLabel("おん:", this);
        displayOn->setText(QString::fromStdString(dataOn));
        displayOn->setFixedSize(250,50);

        Success = new QLabel("Success!", this);
        Failure = new QLabel("Failure!", this);
        flashlayout->addWidget(Success,4,1);
        flashlayout->addWidget(Failure,4,1);
        Success->hide();
        Success->setStyleSheet("color: green");
        Failure->hide();
        Failure->setStyleSheet("color: red");


        flashlayout->addWidget(Id,2,0);

        flashlayout->addWidget(displaySign,2,1);
        flashlayout->addWidget(Sign,1,1);

        flashlayout->addWidget(displayImi,2,2);
        flashlayout->addWidget(Imi,1,2);

        flashlayout->addWidget(displayKun,2,3);
        flashlayout->addWidget(Kun,1,3);

        flashlayout->addWidget(displayOn,2,4);
        flashlayout->addWidget(On,1,4);

        this->setLayout(flashlayout);

        int counter = 0;
        if(mHideimi)
        {
            displayImi->hide();
            Imi->hide();
            counter += 1;
        }
        if(mHidekun)
        {
            displayKun->hide();
            Kun->hide();
            counter += 1;
        }
        if(mHideon )
        {
            displayOn ->hide();
            On ->hide();
            counter += 1;
        }

        int size = 1500;
        if(mHideimi) size -= 500;
        if(mHidekun) size -= 300;
        if(mHideon ) size -= 300;

        this->setFixedSize(size,250);
        this->setWindowTitle("QtKanji #" + QString::fromStdString(randIdstring));
        this->setWindowIcon(QIcon("kanji.ico"));
    }
}

void Flashcard::submitButtonClicked()
{
    dataSign = displaySign->text().toStdString();

    dataOn  = displayOn  ->text().toStdString();
    if(dataOn  == "-none-") dataOn  = "";
    dataOnvec = explode(", ", dataOn);

    dataKun  = displayKun ->text().toStdString();
    if(dataKun == "-none-") dataKun = "";
    dataKunvec = explode(", ", dataKun);

    dataImi  = displayImi ->text().toStdString();
    dataImivec = explode(", ", dataImi);


    if((truedataSign == dataSign) && all_in_vec(truedataOnvec, dataOnvec) && one_in_vec(dataImivec, truedataImivec) && all_in_vec(truedataKunvec, dataKunvec))
    {//Success
        Success     ->show();
        Failure     ->hide();
        contButton  ->show();
        submitButton->hide();

        mSuccesses += 1;

        if(mDirection=="japeng")
        {
            displayKun->setText(QString::fromStdString(truedataKun));
            displayKun->setStyleSheet("color: green");

            displayOn->setText(QString::fromStdString(truedataOn));
            displayOn->setStyleSheet("color: green");

            displayImi->setText(QString::fromStdString(truedataImi));
            displayImi->setStyleSheet("color: green");
        }
        if(mDirection=="engjap")
        {
            displaySign->setText(QString::fromStdString(truedataSign));
            displaySign->setStyleSheet("color: green");
            displaySign->setFont(textfont);
        }
    }
    else
    {//Failure
        Success     ->hide();
        Failure     ->show();
        contButton  ->show();
        submitButton->hide();

        mFailures += 1;

        if(mDirection=="japeng")
        {
            displayKun->setText(QString::fromStdString(truedataKun));
            displayKun->setStyleSheet("color: red");

            displayOn->setText(QString::fromStdString(truedataOn));
            displayOn->setStyleSheet("color: red");

            displayImi->setText(QString::fromStdString(truedataImi));
            displayImi->setStyleSheet("color: red");
        }
        if(mDirection=="engjap")
        {
            displaySign->setText(QString::fromStdString(truedataSign));
            displaySign->setStyleSheet("color: red");
            displaySign->setFont(textfont);
        }
    }
}

void Flashcard::contButtonClicked()
{
    this->close();
    Flashcard *flash = new Flashcard(mLL, mUL, mDirection, mNumbers, mSuccesses, mFailures, mCardbox, mContFlag, mRemFlag, mHideimi, mHidekun, mHideon);
    flash->show();
}

void Flashcard::addButtonClicked()
{
    remButton->show();
    addButton->hide();

    if(!contains(mCardbox, randId))
    {
        mCardbox.push_back(randId);
        if(mContFlag) mRemFlag -= 1;

        std::ofstream f;
        f.open("../container.dat", std::ios::app);
        f << std::to_string(randId)+":";
        f.close();
    }
}

void Flashcard::remButtonClicked()
{
    remButton->hide();
    addButton->show();

    std::ofstream f;
    f.open("../container.dat", std::ios::trunc);
    for(unsigned int i = 0; i < mCardbox.size(); i++)
    {
        if(mCardbox[i] == randId)
        {
            if(i == mCardbox.size()-1)
            {
                mCardbox.pop_back();
            }
            else
            {
                mCardbox[i] = mCardbox[mCardbox.size()-1];
                mCardbox.pop_back();
            }
            if(mContFlag) mRemFlag += 1;
        }

        if(mCardbox.size() == 0) f << "";
        else f << std::to_string(mCardbox[i])+":";
    }
    f.close();
}
