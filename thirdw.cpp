#include "thirdw.h"
#include "ui_thirdw.h"

#include <QPixmap>
#include <QMessageBox>
#include <QProcess>
#include <QFile>
#include <QDebug>

int allQ;
int point2;

thirdw::thirdw(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::thirdw)
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

    ui->pushButton->setVisible(false);
    ui->pushButton_2->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->stackedWidget->setVisible(false);
    ui->lineEdit->setVisible(false);
    ui->score->setVisible(false);
    ui->ques->setVisible(false);
    ui->answer->setVisible(false);
    ui->timer->setVisible(false);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerSlot()));

    allQ = 1;
}

thirdw::~thirdw()
{
    delete ui;
}

void thirdw::recieveData(int points)
{
    ui->res->setText("Мои искренние поздравления, ты дошел до конца викторины! Итак, твои результаты: \n"
                      "Всего ты ответил на 10 вопросов\n"
                      "И заработал " + QString::number(points) + " очков.\n"
                      );

    if (points <= 200) {
        ui->pic->setStyleSheet("border-image: url(://tix.jpeg);");
        ui->text->setText("Тебе вообще что ли неинтересно было? :(\n"
                          "Тихиро в тебе крайне разочарована...");
    }

    if (points >= 300 && points <= 500) {
        ui->pic->setStyleSheet("border-image: url(://mon.jpeg);");
        ui->text->setText("Лучше иди пересмотри 'Принцесса Мононоке', слабак,\n"
                          "вдруг поможет...");
    }

    if (points >= 600 && points <= 700) {
        ui->pic->setStyleSheet("border-image: url(://chicks.jpeg);");
        ui->text->setText("А ты неплох! Однако тебе еще учиться и учиться, чтобы достичь моего уровня.\n");
    }

    if (points >= 800) {
        ui->pic->setStyleSheet("border-image: url(://miyazaki.jpeg);");
        ui->text->setText("Повелевай, мой господин, ты достоин зваться сильнейшим!\n");
         QMessageBox::StandardButton inf= QMessageBox::information(this, "Предложение, от которого невозможно отказаться!", "Ты набрал достаточное количество очков для прохождения во второй раунд\n Играем?" , QMessageBox::No|QMessageBox::Yes);
         if (inf == QMessageBox::No) {
             QApplication::quit();
         } else {
             point2 = points;
             ui->pic->setVisible(false);
             ui->text->setVisible(false);
             ui->res->setVisible(false);

             ui->stackedWidget->setCurrentIndex(0);
             ui->stackedWidget->setVisible(true);
             ui->pushButton->setVisible(true);
             ui->lineEdit->setVisible(true);
             ui->score->setVisible(true);
             ui->ques->setVisible(true);
             ui->score->setText(QString::number(point2));
             ui->timer->setVisible(true);
             timer->start(1000);
         }
    }
}

void thirdw::on_pushButton_2_clicked()
{
    ui->timer->setText("00:15");
    sec = 15;
    timer->start(1000);
    ui->stackedWidget->setCurrentIndex(allQ);
    ui->lineEdit->clear();
    ui->lineEdit->setEnabled(true);
    ui->pushButton_2->setVisible(false);
    ui->ques->setText(QString::number(allQ++));
    ui->answer->setVisible(false);
}

void thirdw::on_pushButton_clicked()
{
    ui->lineEdit->setEnabled(false);
    QString answer = ui->lineEdit->text();
    int page = ui->stackedWidget->currentIndex();
    QFile file(":/answers.txt");
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Ошибка при открытии файла";
    }
    if (answer.isEmpty()) {
        QMessageBox::StandardButton war = QMessageBox::warning(this, "Вот глупый человек!", "Ответ сам себя не напишет:/", QMessageBox::Ok);

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
    qDebug() << "lin: " << lin;
    qDebug() << "page: " << page;
    //qDebug() << then;
    if (!answer.isEmpty()) {
        //qDebug() << "answer не пустой";
        if (!lin.isEmpty()) {
            //qDebug() << "lin не пустая";
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
            if (page == lin2.toInt() - 11) {
                ui->answer->setStyleSheet("color: black; background: white; border-width: 3px; border-style: solid; border-color: green green green green");
                ui->answer->setText("Отлично! Ответ верный, поэтому ты получаешь 500 очков.");
                ui->answer->setVisible(true);
                timer->stop();
                point2 += 500;
                ui->score->setText(QString::number(point2));
                ui->pushButton_2->setVisible(true);
            } else {
                ui->answer->setStyleSheet("color: black; background: white; border-width: 3px; border-style: solid; border-color: red red red red");
                ui->answer->setText("К сожалению, этот ответ неверный...Может быть в следующий раз ты будешь соображать лучше:) Пока!");
                ui->answer->setVisible(true);
                ui->score->setText("0");
                timer->stop();
                ui->pushButton_3->setVisible(true);
                ui->lineEdit->setEnabled(false);
            }
        }
        if (lin.isEmpty()) {
            //qDebug()<< "lol";
            ui->answer->setStyleSheet("color: black; background: white; border-width: 3px; border-style: solid; border-color: red red red red");
            ui->answer->setText("К сожалению, этот ответ неверный...Может быть в следующий раз ты будешь соображать лучше:) Пока!");
            ui->answer->setVisible(true);
            ui->score->setText("0");
            timer->stop();
            ui->pushButton_3->setVisible(true);
            ui->lineEdit->setEnabled(false);
        }
    }
}

void thirdw::on_pushButton_3_clicked()
{
    QApplication::quit();
}

void thirdw::TimerSlot()
{
    sec--;
    if (sec <= 0) {
        ui->answer->setStyleSheet("color: black; background: white; border-width: 3px; border-style: solid; border-color: red red red red");
        ui->answer->setText("К сожалению, этот ответ неверный...Может быть в следующий раз ты будешь соображать лучше:) Пока!");
        ui->answer->setVisible(true);
        ui->lineEdit->setEnabled(false);
        ui->score->setText("0");
        timer->stop();
        ui->pushButton_3->setVisible(true);
    }

    ui->timer->setText( " 0" + QString::number(min) + " : " + (sec >=10 ? QString::number(sec) : "0" + QString::number(sec)));
}
