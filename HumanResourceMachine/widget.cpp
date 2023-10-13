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

    qIn.clear(), qOut.clear(), qAns.clear(), cmdSet.clear();
    vec.clear(), existVec.clear();
    existCurrentBlock = 0;

    qIn.enqueue(1), qIn.enqueue(2);
    qAns.enqueue(1), qAns.enqueue(2);
    n = 0;
    vec.resize(n);
    existVec.resize(n);
    cmdSet.insert("inbox"), cmdSet.insert("outbox");

    setUpBackground();
}


void Widget::on_levelButton2_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);

    qIn.clear(), qOut.clear(), qAns.clear(), cmdSet.clear();
    vec.clear(), existVec.clear();
    existCurrentBlock = 0;

    qIn.enqueue(3), qIn.enqueue(9), qIn.enqueue(5), qIn.enqueue(1), qIn.enqueue(-2), qIn.enqueue(-2), qIn.enqueue(9), qIn.enqueue(-9);
    qAns.enqueue(-6), qAns.enqueue(6), qAns.enqueue(4), qAns.enqueue(-4), qAns.enqueue(0), qAns.enqueue(0), qAns.enqueue(18), qAns.enqueue(-18);
    n = 3;
    vec.resize(n);
    existVec.resize(n);
    cmdSet.insert("inbox"), cmdSet.insert("outbox"), cmdSet.insert("copyfrom"), cmdSet.insert("copyto");
    cmdSet.insert("add"), cmdSet.insert("sub"), cmdSet.insert("jump"), cmdSet.insert("jumpifzero");

    setUpBackground();
}


void Widget::on_levelButton3_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);

    qIn.clear(), qOut.clear(), qAns.clear(), cmdSet.clear();
    vec.clear(), existVec.clear();
    existCurrentBlock = 0;

    qIn.enqueue(6), qIn.enqueue(2), qIn.enqueue(7), qIn.enqueue(7), qIn.enqueue(-9), qIn.enqueue(3), qIn.enqueue(-3), qIn.enqueue(-3);
    qAns.enqueue(7), qAns.enqueue(-3);
    n = 2;
    vec.resize(n);
    existVec.resize(n);
    cmdSet.insert("inbox"), cmdSet.insert("outbox"), cmdSet.insert("copyfrom"), cmdSet.insert("copyto");
    cmdSet.insert("add"), cmdSet.insert("sub"), cmdSet.insert("jump"), cmdSet.insert("jumpifzero");

    setUpBackground();
}


void Widget::on_levelButton4_clicked()
{
    QString filePathName = QFileDialog::getOpenFileName(this, "打开", "./", "JSON 文件 (*.json)");
    if (filePathName.isEmpty()) {
        QMessageBox::warning(this, "警告", "已取消选择存档！");
    } else {
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
    if (!doing) {
        cmdLines = ui->cmdTextEdit->toPlainText().split("\n");

        m = cmdLines.size();

        if (m == 0) {
            printFailMessage();
            return;
        }

        currentCommand = 1;
        qOut.clear();
        ui->cmdTextEdit->setFocusPolicy(Qt::NoFocus);

        ui->currentStepLabel->setText(cmdLines[currentCommand - 1]);
        ui->confirmNextStepButton->setText("下一步");
        doing = true;
    } else {

        if (currentCommand == m + 1) {
            checkResult();
            return;
        }

        QString cmd = cmdLines[currentCommand - 1].split(' ')[0];
        if (cmdSet.contains(cmd)) {
            if (cmd == "inbox") {
                if (qIn.empty()) {
                    checkResult();
                }
                currentBlock = qIn.front();
                qIn.dequeue();
                existCurrentBlock = true;
            } else if (cmd == "outbox") {
                if (existCurrentBlock == false) {
                    printErrorMessage();
                    return;
                }
                qOut.enqueue(currentBlock);
                existCurrentBlock = false;
            } else if (cmd == "add") {
                int x = cmdLines[currentCommand - 1].split(' ')[1].toInt();
                if (existCurrentBlock == false || x >= vec.size() || existVec[x] == false) {
                    printErrorMessage();
                    return;
                }
                currentBlock += vec[x];
            } else if (cmd == "sub") {
                int x = cmdLines[currentCommand - 1].split(' ')[1].toInt();
                if (existCurrentBlock == false || x >= vec.size() || existVec[x] == false) {
                    printErrorMessage();
                    return;
                }
                currentBlock -= vec[x];
            } else if (cmd == "copyto") {
                qDebug() << cmdLines[currentCommand - 1] << '\n';
                int x = cmdLines[currentCommand - 1].split(' ')[1].toInt();
                if (existCurrentBlock == false || x >= vec.size()) {
                    printErrorMessage();
                    return;
                }
                vec[x] = currentBlock;
                existVec[x] = true;
            } else if (cmd == "copyfrom") {
                int x = cmdLines[currentCommand - 1].split(' ')[1].toInt();
                if (x >= vec.size() || existVec[x] == false) {
                    printErrorMessage();
                    return;
                }
                currentBlock = vec[x];
            } else if (cmd == "jump") {
                int x = cmdLines[currentCommand].split(' ')[1].toInt();
                if (x > m) {
                    printErrorMessage();
                    return;
                }
                currentCommand = x;
                ui->currentStepLabel->setText(cmdLines[currentCommand - 1]);
                return;
            } else if (cmd == "jumpifzero") {
                int x = cmdLines[currentCommand - 1].split(' ')[1].toInt();
                if (existCurrentBlock == false || x > m) {
                    printErrorMessage();
                    return;
                }
                if (currentBlock == 0) {
                    currentCommand = x;
                    ui->currentStepLabel->setText(cmdLines[currentCommand - 1]);
                    return;
                }
            } else {
                printErrorMessage();
                return;
            }
        } else {
            printErrorMessage();
            return;
        }
        currentCommand++;
        if (currentCommand <= m)
            ui->currentStepLabel->setText(cmdLines[currentCommand - 1]);
        else
            ui->confirmNextStepButton->setText("检查");
        drawStatus();
    }
}


void Widget::drawStatus() {
    if (!existCurrentBlock) {
        ui->robotLabel->setText("X");
    } else {
        ui->robotLabel->setText(QString::number(currentBlock));
    }

    QQueue<int> tmp = qIn;
    QString str = "";

    while (!tmp.empty()) {
        str.append(QString::number(tmp.front()));
        str.append('\n');
        tmp.dequeue();
    }

    ui->inputTextLabel->setText(str);

    tmp = qOut;
    str.clear();

    while (!tmp.empty()) {
        str.append(QString::number(tmp.front()));
        str.append('\n');
        tmp.dequeue();
    }

    ui->outputTextLabel->setText(str);

    str.clear();

    for (int i = 0; i < n; i++) {
        if (existVec[i] == 0) {
            str.append("-");
        } else {
            str.append(QString::number(vec[i]));
        }
    }

    ui->vacantTextLabel->setText(str);
}


void Widget::setUpBackground() {

    ui->cmdTextEdit->setFocusPolicy(Qt::ClickFocus);
    ui->cmdTextEdit->clear();
    ui->currentStepLabel->clear();
    ui->confirmNextStepButton->setText("确认");
    drawStatus();
    doing = false;
}

void Widget::printSuccessMessage() {

    bool ok = QMessageBox::information(this, "Success", "任务成功！");
    if (ok) {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void Widget::printFailMessage() {
    bool ok = QMessageBox::critical(this, "Fail", "任务失败！");
    if (ok) {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void Widget::printErrorMessage() {
    QString str = "您第 " + QString::number(currentCommand) + " 行的命令为非法命令。";
    bool ok = QMessageBox::warning(this, "Error", str);
    if (ok) {
        ui->stackedWidget->setCurrentIndex(0);
    }
}


void Widget::checkResult() {
    if (qOut.size() != qAns.size()) {
        printFailMessage();
        return;
    } else {
        while (!qOut.empty()) {
            int a = qOut.front(), b = qAns.front();
            qOut.dequeue(), qAns.dequeue();
            if (a != b) {
                printFailMessage();
                return;
            }
        }
        printSuccessMessage();
        return;
    }
}
