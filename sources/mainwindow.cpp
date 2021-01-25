#include "flashcard.h"
#include "examplewindow.h"
#include "mainwindow.h"
#include "screen.h"

std::vector<std::string> explode3(const std::string &delimiter, const std::string &str)
{
    std::vector<std::string> arr;

    int strleng = str.length();
    int delleng = delimiter.length();
    if (delleng==0) return arr;//no change

    int i=0;
    int k=0;
    while(i<strleng)
    {
        int j=0;
        while (i+j<strleng && j<delleng && str[i+j]==delimiter[j])
            j++;
        if (j==delleng)//found delimiter
        {
            arr.push_back(str.substr(k, i-k));
            i+=delleng;
            k=i;
        }
        else
        {
            i++;
        }
    }

    return arr;
}

std::vector<unsigned int> convert_string(std::vector<std::string> &arr)
{
    std::vector<unsigned int> conv_arr;
    for(unsigned int i = 0; i < arr.size(); i++)
    {
        int foo = std::stoi(arr[i]);
        conv_arr.push_back(foo);
    }
    return conv_arr;
}

MainWindow::MainWindow(QWidget *parent): QWidget(parent)
{
        srand(time(NULL));

        QGridLayout *layout=new QGridLayout;

        signButton = new QPushButton("train kanji");
        signButton->setFixedSize(150,30);
        connect(signButton, &QPushButton::clicked, this, &MainWindow::signButtonClicked);
        layout->addWidget(signButton,4,0);

        containerButton = new QPushButton("cardbox");
        containerButton->setFixedSize(150,30);
        connect(containerButton, &QPushButton::clicked, this, &MainWindow::containerButtonClicked);
        layout->addWidget(containerButton,5,0);

        exButton = new QPushButton("train words");
        exButton->setFixedSize(150,30);
        connect(exButton, &QPushButton::clicked, this, &MainWindow::exButtonClicked);
        layout->addWidget(exButton,4,1);

        engjapButton = new QPushButton("ふりがな - 漢字");
        engjapButton->setFixedSize(150,30);
        connect(engjapButton, &QPushButton::clicked, this, &MainWindow::engjapButtonClicked);
        layout->addWidget(engjapButton,1,0);
        engjapButton->setCheckable(true);
        engjapButton->setChecked(true);

        japengButton = new QPushButton("漢字 - ふりがな");
        japengButton->setFixedSize(150,30);
        connect(japengButton, &QPushButton::clicked, this, &MainWindow::japengButtonClicked);
        layout->addWidget(japengButton,1,1);
        japengButton->setCheckable(true);

        imibox = new QCheckBox("Hide Imi", this);
        imibox->setChecked(false);
        connect(imibox, &QCheckBox::clicked, this, &MainWindow::boxChecked);

        kunbox = new QCheckBox("Hide Kun", this);
        kunbox->setChecked(false);
        connect(kunbox, &QCheckBox::clicked, this, &MainWindow::boxChecked);

        onbox  = new QCheckBox("Hide On" , this);
        onbox->setChecked(false);
        connect(onbox,  &QCheckBox::clicked, this, &MainWindow::boxChecked);

        //set default values in displays
        displayLL = new QLineEdit(this);
        displayLL->setFixedSize(150,30);
        LL = new QLabel("from:", this);
        displayLL->setText("317");
     
        displayUL = new QLineEdit(this);
        displayUL->setFixedSize(150,30);
        UL = new QLabel("to:", this);
        displayUL->setText("460");

        dataFail = new QLabel("file error!", this);
        dataFail->hide();
        cbFail = new QLabel("no cards in box.", this);
        cbFail->hide();

        layout->addWidget(dataFail,5,1);
        layout->addWidget(cbFail,5,1);
        layout->addWidget(displayLL,2,1);
        layout->addWidget(LL,2,0);
        layout->addWidget(displayUL,3,1);
        layout->addWidget(UL,3,0);
        layout->addWidget(imibox,2,2);
        layout->addWidget(kunbox,3,2);
        layout->addWidget(onbox, 4,2);

        textfont.setPointSize(15);
        textfont.setBold(false);

        this->setFont(textfont);      
        this->setLayout(layout);
        this->setFixedSize(450,200);

        this->setWindowTitle("QtKanji mainwindow");
}

void MainWindow::signButtonClicked()
{
    container_flag = 0;
    start_flashcard_window();
}

void MainWindow::containerButtonClicked()
{
    container_flag = 1;
    start_flashcard_window();
}

void MainWindow::exButtonClicked()
{
    start_example_window();
}

void MainWindow::boxChecked()
{
    if(kunbox->isChecked() &&  onbox->isChecked()) imibox->setChecked(false);
    if(imibox->isChecked()) hideimi = true;
    else hideimi = false;

    if(imibox->isChecked() &&  onbox->isChecked()) kunbox->setChecked(false);
    if(kunbox->isChecked()) hidekun = true;
    else hidekun = false;

    if(imibox->isChecked() && kunbox->isChecked())  onbox->setChecked(false); 
    if( onbox->isChecked())  hideon  = true;
    else hideon  = false;
}

void MainWindow::engjapButtonClicked()
{
    if(engjapButton->isChecked())
    {
        direction = "engjap";
        japengButton->setChecked(false);
    }
    else
    {
        japengButton->setChecked(true);
        japengButtonClicked();
    }
}

void MainWindow::japengButtonClicked()
{
    if(japengButton->isChecked())
    {
        direction = "japeng";
        engjapButton->setChecked(false);
    }
    else
    {
        engjapButton->setChecked(true);
        engjapButtonClicked();
    }
}

void MainWindow::start_flashcard_window()
{  
    //get values from input but catch invalid data
    mLL = displayLL->text().toInt(); //characters become zero
    mUL = displayUL->text().toInt();
    if(mUL > 460) mUL = 460;
    if(mUL < mLL)
    {
        displayLL->setText("insert integers.");
        displayUL->setText("");
        return;
    }

    //////////////////////////////////
    if(!cardbox.empty()) cardbox.clear();
    std::ifstream read_cont("../container.dat");

    if(!read_cont)
    {
        dataFail->show();
        return;
    }
    dataFail->hide();

    std::string linedata = "";
    std::getline(read_cont, linedata);
    if(linedata == "" && container_flag == 1)
    {
        cbFail->show();
        dataFail->hide();
        return;
    }
    else
    {
        cbFail->hide();
    }

    std::vector<std::string> help = explode3(":", linedata);
    cardbox = convert_string(help);

    read_cont.close();

    //////////////////////////////////
    numbers.clear();
    Flashcard *flash = new Flashcard(mLL, mUL, direction, numbers, 0, 0, cardbox, container_flag, 0, hideimi, hidekun, hideon);
    flash->show();

    /*Screen *s = new Screen();  // create a widget
    s->setStyleSheet("background-color:white;");
    s->show(); //show the widget and its children
    s->update();*/
}

void MainWindow::start_example_window(/*std::string direction*/)
{
    //get values from input but catch invalid data
    mLL = displayLL->text().toInt(); //characters become zero
    mUL = displayUL->text().toInt();
    if(mUL < mLL)
    {
        displayLL->setText("insert integers.");
        displayUL->setText("");
        return;
    }

    //////////////////////////////////////////////////////////////////////////////

    std::string LLstring = std::to_string(mLL);
    std::string ULstring = std::to_string(mUL);

    std::vector<std::string> dataFuri, truedataFuri, dataKan, truedataKan, dataEng;

    std::ifstream examples("../examples.dat");
    if(!examples)
    {
        dataFail->show();
        return;
    }
    dataFail->hide();
    std::string linedata;
    std::vector<std::string> help;
    while (!examples.eof())
    {
        std::getline(examples, linedata, '\n');
        if(linedata >= LLstring && linedata <= ULstring)
        {
            std::getline(examples, linedata, '\n');
            help = explode2(", ", linedata);
            for(unsigned int i = 0; i < help.size(); i++)
            {
                dataFuri.push_back(help[i]);
                truedataFuri.push_back(help[i]);
            }

            std::getline(examples, linedata, '\n');
            help = explode2(", ", linedata);
            for(unsigned int i = 0; i < help.size(); i++)
            {
                dataKan.push_back(help[i]);
                truedataKan.push_back(help[i]);
            }

            std::getline(examples, linedata, '\n');
            help = explode2(", ", linedata);
            for(unsigned int i = 0; i < help.size(); i++)
            {
                dataEng.push_back(help[i]);
            }
        }
    }
    examples.close();

    ////////////////////////////////////////////////////////////////////////////
    numbers.clear();
    Example *ex = new Example(dataFuri, dataKan, dataEng, numbers, direction, 0, 0);
    ex->show();

    //Screen *s = new Screen();  // create a widget
    //s->setStyleSheet("background-color:white;");
    //s->show(); //show the widget and its children
    //s->update();

    //QTimer *timer = new QTimer(this); //start timer
    //connect(timer, &QTimer::timeout, s, static_cast <void (Screen::*)()>(&Screen::update)); //specify correct connect function (three possibilities)
    //timer->start(1); //update ising window every 0.5 seconds
}

