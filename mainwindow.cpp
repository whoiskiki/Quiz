#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QPixmap>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(1000,800);
    this->setFixedSize(1000,800);
    this->setWindowTitle("TIME FOR QUIZ!");
    window = new SecondW(this);

    connect(window, &SecondW::firstWindow, this, &MainWindow::show);

    QPixmap back("://plup.jpeg");
    back = back.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, back);
    this->setPalette(palette);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_rulesButton_clicked()
{

    QMessageBox msgBox;
    msgBox.information(this, "ПРАВИЛА", "1)Ответ необходимо записывать в том падеже, в каком вопрос того требует. \n"
                                  "2)За каждый правильный ответ начисляется 100 очков, за каждый неправильный они же отнимаются. \n"
                                  "3)Числовые ответы так же необходимо записывать словами.\n"
                                  "4)Если ответ не будет введен, но кнопка 'Далее' нажата, вы получите предупреждающее сообщение.\n"
                                  "5)Для прохождения во второй раунд необходимо набрать хотя бы 800 очков(допускается 1 ошибка в 1 раунде). \n"
                                  "6)Во втором раунде допускать ошибку нельзя - очки автоматически аннулируются и вы проигрываете.\n"
                                  "7)Вам будет дано лишь 15 секунд на ввод ответа во втором раунде. \n");

}

void MainWindow::on_startButton_clicked()
{
    hide();
    window->show();   
}

void MainWindow::on_exitButton_clicked()
{
    QApplication::quit();
}
