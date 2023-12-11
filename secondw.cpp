#include "secondw.h"
#include "ui_secondw.h"

#include <QStackedWidget>
#include <QFile>
#include <QDebug>
#include <QProcess>
#include <QMessageBox>

int allQuestions;
int score;

SecondW::SecondW(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondW)
{
    ui->setupUi(this);
    this->resize(1000,800);
    this->setFixedSize(1000,800);
    this->setWindowTitle("It`s a game time!");

    QPixmap back("://fonchik.jpeg");
    back = back.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, back);
    this->setPalette(palette);

    win3 = new thirdw(this);

    ui->stackedWidget->setCurrentIndex(0);

    connect(this, SIGNAL(sendData(int)), win3, SLOT(recieveData(int)));

    ui->pushButton_3->setVisible(false);

    allQuestions = 1;
    score = 0;
}

SecondW::~SecondW()
{
    delete ui;
}


void SecondW::on_pushButton_2_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Ты уверен, что хочешь покинуть игру?", "Может быть, именно сегодня ты станешь миллионером...",  QMessageBox::No | QMessageBox::Yes);
    if (reply == QMessageBox::Yes) {
        this->hide();
        ui->stackedWidget->setCurrentIndex(0);
        ui->score->setText("0");
        emit firstWindow();
    }
}

void SecondW::on_passButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(allQuestions++);
    ui->lineEdit->clear();
    ui->pushButton->setEnabled(false);
}


void SecondW::on_answerButton_clicked()
{
    QString answer = ui->lineEdit->text();
    int page = ui->stackedWidget->currentIndex();
    QFile file(":/answers.txt");
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Ошибка при открытии файла";
    }
    if (answer.isEmpty()) {
        QMessageBox::StandardButton err = QMessageBox::warning(this, "Вот глупый человек!", "Ответ сам себя не напишет:/", QMessageBox::Ok);

    }
    QTextStream in(&file);
    QString program = "grep";
    QStringList arguments;
    arguments << "-w" << "-i" << answer << "/home/fraukrizz/myPro/answers.txt";

    QProcess *myPro = new QProcess(this);
    myPro->start(program, arguments);
    myPro->waitForFinished();
    QString lin;
    lin.append(myPro->readAllStandardOutput());
    if (!answer.isEmpty()) {
        if (!lin.isEmpty()) {
            QString pro1 = "grep";
            QStringList argument;
            argument << "-n" << "-i" << "^" + answer + "$" << "/home/fraukrizz/myPro/answers.txt";

            QProcess *myPro1 = new QProcess(this);
            QProcess *myPro2 = new QProcess(this);
            myPro1->setStandardOutputProcess(myPro2);
            myPro1->start(pro1, argument);
            myPro1->waitForFinished();
            myPro2->start("bash", QStringList() << "-c" << " head -n 1 | cut -d: -f1");
            myPro2->waitForFinished();
            QString lin2;
            lin2.append(myPro2->readAllStandardOutput());
            if (page == (lin2.toInt() - 1 )) {
                ui->result->setStyleSheet("color: black; background: white; border-width: 3px; border-style: solid; border-color: green green green green");
                ui->result->setText("Отлично! Ответ верный, мой юный падаван:)");
                ui->result->setVisible(true);
                ui->lineEdit->setEnabled(false);
                score += 100;
                ui->points->setText(QString::number(score));
            } else {
                ui->result->setStyleSheet("color: black; background: white; border-width: 3px; border-style: solid; border-color: red red red red");
                ui->result->setText("Упс...Это неверный ответ!");
                ui->result->setVisible(true);
                ui->lineEdit->setEnabled(false);
                if (score > 0) {
                    score -= 100;
                }
                ui->points->setText(QString::number(score));
            }
        }

        if (lin.isEmpty()) {
            ui->result->setStyleSheet("color: black; background: white; border-width: 3px; border-style: solid; border-color: red red red red");
            ui->result->setText("Упс...Это неверный ответ!");
            ui->result->setVisible(true);
            ui->lineEdit->setEnabled(false);
            if (score > 0) {
                score -= 100;
            }
            ui->points->setText(QString::number(score));
        }

        if (ui->stackedWidget->currentIndex() == 9) {
            ui->pushButton_3->setVisible(true);
        } else {
            ui->pushButton->setEnabled(true);
        }
    }
}

void SecondW::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(allQuestions++);
    ui->result->setVisible(false);
    ui->lineEdit->setEnabled(true);
    ui->lineEdit->clear();
    ui->pushButton->setEnabled(false);
    ui->score->setText(QString::number(allQuestions));
}

void SecondW::on_pushButton_3_clicked()
{
    this->hide();
    win3->show();
    emit sendData(score);
}
