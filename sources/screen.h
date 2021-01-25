#ifndef SCREEN_H
#define SCREEN_H

#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QApplication>
#include <cstdio>
#include <vector>

class Screen : public QWidget {
public:
  Screen(QWidget *parent = NULL) : QWidget(parent) {
    this->resize(320, 240);
    this->setWindowTitle("HelloGUI with Qt");
    label = new QLabel("Position 0, 0", this);
    label->setGeometry(120, 20, 150, 30);
  }
protected:
  void mouseMoveEvent(QMouseEvent *m)
  {
      //matrix[m->x()][m->y()] = 1;
      //label->setText(QString("Position %1, %2").arg(x).arg(y));
  }
  void paintEvent(QPaintEvent *) //override paintEvent function
  {
      /*QPainter qp(this);
      unsigned PenSize = 10;
      //unsigned offset = PenSize ? PenSize : 1;

      QPen WhitePen(Qt::white, PenSize, Qt::SolidLine),
           BlackPen(Qt::black, PenSize, Qt::SolidLine);

      for(unsigned int i = 0; i < 1000; i++)
      {
          for(unsigned int j = 0; j < 1000; j++)
          {
              //if(matrix[i[j]] == 1) qp.setPen(BlackPen);
              //else qp.setPen(WhitePen);

             // qp.drawPoint(matrix[i[j]]);
          }
      }*/
  }
private:
  QLabel* label;
  int mX, mY;
  unsigned int matrix[1000][1000];
};

#endif // SCREEN_H

