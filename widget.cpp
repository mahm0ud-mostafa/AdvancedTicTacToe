#include "widget.h"
#include "./ui_widget.h"
#include <QMessageBox>

char current_player='X';
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->pushButton_11, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_12, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_13, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_14, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_15, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_16, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_17, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_18, &QPushButton::clicked, this, &Widget::handleButtonClick);
    connect(ui->pushButton_19, &QPushButton::clicked, this, &Widget::handleButtonClick);
/*connected buttons of x o board to one signal*/
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void Widget::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void Widget::on_pushButton_4_clicked()
{
     ui->stackedWidget->setCurrentIndex(3);
}


void Widget::on_pushButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void Widget::on_pushButton_9_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void Widget::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}


void Widget::on_pushButton_10_clicked()
{

    ui->stackedWidget->setCurrentIndex(1);
}


void Widget::on_pushButton_7_clicked()
{   current_player='X';
    ui->label_11->setText(QString(current_player));
    ui->stackedWidget->setCurrentIndex(5);
}


void Widget::on_pushButton_8_clicked()
{   current_player='O';
    ui->label_11->setText(QString(current_player));
    ui->stackedWidget->setCurrentIndex(5);
}


void Widget::on_pushButton_6_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::handleButtonClick()
{
    // Identify which button was clicked
    QPushButton *Clicked_button = qobject_cast<QPushButton *>(sender());//pointer to the clicked button
    Clicked_button->setText(QString(current_player));
    if(current_player=='X')
        current_player='O';
    else if (current_player=='O')
        current_player='X';
    ui->label_11->setText(QString(current_player));
    bool flag1 =false;
    if((!ui->pushButton_18->text().isEmpty())&&(QString(ui->pushButton_18->text()))==(QString(ui->pushButton_15->text()))&&((QString(ui->pushButton_18->text()))==(QString(ui->pushButton_16->text())))&&(QString(ui->pushButton_15->text()))==(QString(ui->pushButton_16->text())))
            flag1=true;
    if((!ui->pushButton_17->text().isEmpty())&&(QString(ui->pushButton_17->text()))==(QString(ui->pushButton_11->text()))&&((QString(ui->pushButton_17->text()))==(QString(ui->pushButton_12->text())))&&(QString(ui->pushButton_11->text()))==(QString(ui->pushButton_12->text())))
        flag1=true;
    if((!ui->pushButton_19->text().isEmpty())&&(QString(ui->pushButton_19->text()))==(QString(ui->pushButton_13->text()))&&((QString(ui->pushButton_19->text()))==(QString(ui->pushButton_14->text())))&&(QString(ui->pushButton_13->text()))==(QString(ui->pushButton_14->text())))
        flag1=true;
    if((!ui->pushButton_19->text().isEmpty())&&(QString(ui->pushButton_18->text()))==(QString(ui->pushButton_17->text()))&&((QString(ui->pushButton_18->text()))==(QString(ui->pushButton_19->text())))&&(QString(ui->pushButton_17->text()))==(QString(ui->pushButton_19->text())))
        flag1=true;
    if((!ui->pushButton_11->text().isEmpty())&&(QString(ui->pushButton_15->text()))==(QString(ui->pushButton_11->text()))&&((QString(ui->pushButton_11->text()))==(QString(ui->pushButton_13->text())))&&(QString(ui->pushButton_15->text()))==(QString(ui->pushButton_13->text())))
        flag1=true;
    if((!ui->pushButton_14->text().isEmpty())&&(QString(ui->pushButton_14->text()))==(QString(ui->pushButton_16->text()))&&((QString(ui->pushButton_14->text()))==(QString(ui->pushButton_12->text())))&&(QString(ui->pushButton_12->text()))==(QString(ui->pushButton_16->text())))
        flag1=true;
    if((!ui->pushButton_11->text().isEmpty())&&(QString(ui->pushButton_18->text()))==(QString(ui->pushButton_11->text()))&&((QString(ui->pushButton_11->text()))==(QString(ui->pushButton_14->text())))&&(QString(ui->pushButton_18->text()))==(QString(ui->pushButton_14->text())))
        flag1=true;
    if((!ui->pushButton_16->text().isEmpty())&&(QString(ui->pushButton_19->text()))==(QString(ui->pushButton_11->text()))&&((QString(ui->pushButton_19->text()))==(QString(ui->pushButton_16->text())))&&(QString(ui->pushButton_11->text()))==(QString(ui->pushButton_16->text())))
        flag1=true;


    if(flag1)
    {
       QMessageBox::information(this, "congratulation", "__YOU WIN__");
    }
    if((!ui->pushButton_11->text().isEmpty())&&(!ui->pushButton_12->text().isEmpty())&&(!ui->pushButton_13->text().isEmpty())&&(!ui->pushButton_14->text().isEmpty())&&(!ui->pushButton_15->text().isEmpty())&&(!ui->pushButton_16->text().isEmpty())&&(!ui->pushButton_17->text().isEmpty())&&(!ui->pushButton_18->text().isEmpty())&&(!ui->pushButton_19->text().isEmpty()))
        QMessageBox::information(this, "game over", "__Draw__");
}

void Widget::on_pushButton_20_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->pushButton_18->setText(" ");
    ui->pushButton_17->setText(" ");
    ui->pushButton_16->setText(" ");
    ui->pushButton_15->setText(" ");
    ui->pushButton_14->setText(" ");
    ui->pushButton_13->setText(" ");
    ui->pushButton_12->setText(" ");
    ui->pushButton_11->setText(" ");
    ui->pushButton_19->setText(" ");
}

