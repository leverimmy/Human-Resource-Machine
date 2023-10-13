#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_backButton1_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void Widget::on_backButton2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void Widget::on_backButton3_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void Widget::on_startButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void Widget::on_aboutButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void Widget::on_exitButton_clicked()
{
    close();
}


void Widget::on_volumnHorizontalSlider_sliderMoved(int position)
{
    ui->volumnValue->setText(QString::number(position));
}


void Widget::on_selectFileDirectory_clicked()
{
    QString filePathName = QFileDialog::getOpenFileName(this, "打开", "./", "JSON 文件 (*.json)");
    if (filePathName.isEmpty()) {
        QMessageBox::warning(this, "警告", "已取消选择存档！");
    }
    else {
        ui->fileDirectory->setText(filePathName);
        QFile openFile(filePathName);
        openFile.open(QIODevice::ReadOnly);
        QByteArray fileContents = openFile.readAll();
        openFile.close();

        // fileContents 导入之后需要判断格式是否合法
    }
}


void Widget::on_levelButton1_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void Widget::on_levelButton2_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void Widget::on_levelButton3_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void Widget::on_levelButton4_clicked()
{
    QString filePathName = QFileDialog::getOpenFileName(this, "打开", "./", "JSON 文件 (*.json)");
    if (filePathName.isEmpty()) {
        QMessageBox::warning(this, "警告", "已取消选择存档！");
    }
    else {
        ui->fileDirectory->setText(filePathName);
        QFile openFile(filePathName);
        openFile.open(QIODevice::ReadOnly);
        QByteArray fileContents = openFile.readAll();
        openFile.close();

        // fileContents 导入之后需要判断格式是否合法

        ui->stackedWidget->setCurrentIndex(3);
    }


}
