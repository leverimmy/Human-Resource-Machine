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
    ui->stackedWidget->setCurrentIndex(1);
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

    qIn.enqueue(1), qIn.enqueue(2);
    qAns.enqueue(1), qAns.enqueue(2);
    n = 0;
    vec.resize(n);
    cmdSet.insert("inbox"), cmdSet.insert("outbox");

    setUpBackground();
}


void Widget::on_levelButton2_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);

    qIn.enqueue(3), qIn.enqueue(9), qIn.enqueue(5), qIn.enqueue(1), qIn.enqueue(-2), qIn.enqueue(-2), qIn.enqueue(9), qIn.enqueue(-9);
    qAns.enqueue(-6), qAns.enqueue(6), qAns.enqueue(4), qAns.enqueue(-4), qAns.enqueue(0), qAns.enqueue(0), qAns.enqueue(18), qAns.enqueue(-18);
    n = 3;
    vec.resize(n);
    cmdSet.insert("inbox"), cmdSet.insert("outbox"), cmdSet.insert("copyfrom"), cmdSet.insert("copyto");
    cmdSet.insert("add"), cmdSet.insert("sub"), cmdSet.insert("jump"), cmdSet.insert("jumpifzero");

    setUpBackground();
}


void Widget::on_levelButton3_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);

    qIn.clear(), qOut.clear(), qAns.clear(), cmdSet.clear();
    vec.clear();

    qIn.enqueue(6), qIn.enqueue(2), qIn.enqueue(7), qIn.enqueue(7), qIn.enqueue(-9), qIn.enqueue(3), qIn.enqueue(-3), qIn.enqueue(-3);
    qAns.enqueue(7), qAns.enqueue(-3);
    n = 2;
    vec.resize(n);
    cmdSet.insert("inbox"), cmdSet.insert("outbox"), cmdSet.insert("copyfrom"), cmdSet.insert("copyto");
    cmdSet.insert("add"), cmdSet.insert("sub"), cmdSet.insert("jump"), cmdSet.insert("jumpifzero");

    setUpBackground();
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

void Widget::on_confirmNextStepButton_clicked()
{
    cmdLines = ui->cmdTextEdit->toPlainText().split("\n");

    m = cmdLines.size();

    qOut.clear();

    ui->cmdTextEdit->setFocusPolicy(Qt::NoFocus);
    ui->confirmNextStepButton->setDisabled(true);
}


void Widget::setUpBackground() {

    ui->robotLabel->setText("X");

    QQueue<int> tmp = qIn;
    QString str;

    while (!tmp.empty()) {
        str.append(QString::number(tmp.front()));
        str.append('\n');
        tmp.dequeue();
    }

    ui->inputTextLabel->setText(str);
}
