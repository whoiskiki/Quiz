#ifndef THIRDW_H
#define THIRDW_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class thirdw;
}

class thirdw : public QDialog
{
    Q_OBJECT

public:
    explicit thirdw(QWidget *parent = nullptr);
    ~thirdw();
public slots:
   void recieveData(int points);

private slots:

   void on_pushButton_2_clicked();

   void on_pushButton_clicked();

   void on_pushButton_3_clicked();

private:
    Ui::thirdw *ui;
    QTimer *timer;
    int min = 0;
    int sec = 15;

private slots:
    void TimerSlot();
};

#endif // THIRDW_H
