#ifndef SECONDW_H
#define SECONDW_H

#include <QDialog>
#include "thirdw.h"

namespace Ui {
class SecondW;
}

class SecondW : public QDialog
{
    Q_OBJECT

public:
    explicit SecondW(QWidget *parent = nullptr);
    ~SecondW();

signals:
    void firstWindow();  // Сигнал для первого окна на открытие

    void sendData(int points);

private slots:

    void on_pushButton_2_clicked();

    void on_passButton_clicked();

    void on_answerButton_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();


private:
    Ui::SecondW *ui;
    thirdw *win3; 

};

#endif // SECONDW_H
