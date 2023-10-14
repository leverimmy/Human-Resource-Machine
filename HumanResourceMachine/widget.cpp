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
    try {
        QFile openFile("./userdata.txt");
        openFile.open(QIODevice::ReadOnly);
        QByteArray fileContents = openFile.readAll();
        openFile.close();
        level = fileContents.toInt();
    } catch (...) {
        level = 0;
    }

    renderLevelButton(level);
    ui->stackedWidget->setCurrentIndex(1);
}


void Widget::on_aboutButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void Widget::on_exitButton_clicked()
{
    // todo: 存储 level 信息到本地

    QFile writeFile("./userdata.txt");
    writeFile.open(QIODevice::WriteOnly);
    writeFile.write(QString::number(level).toLatin1());
    writeFile.close();

    close();
}


void Widget::on_volumnHorizontalSlider_sliderMoved(int position)
{
    ui->volumnValue->setText(QString::number(position));
}


void Widget::on_levelButton1_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);

    qIn.clear(), qOut.clear(), qAns.clear(), cmdSet.clear();
    vec.clear(), existVec.clear();
    existCurrentBlock = 0;

    currentLevel = 1;
    qIn.enqueue(1), qIn.enqueue(2);
    qAns.enqueue(1), qAns.enqueue(2);
    n = 0;
    vec.resize(n);
    existVec.resize(n);
    cmdSet.enqueue("inbox"), cmdSet.enqueue("outbox");

    setUpBackground();
}


void Widget::on_levelButton2_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);

    qIn.clear(), qOut.clear(), qAns.clear(), cmdSet.clear();
    vec.clear(), existVec.clear();
    existCurrentBlock = 0;

    currentLevel = 2;
    qIn.enqueue(3), qIn.enqueue(9), qIn.enqueue(5), qIn.enqueue(1), qIn.enqueue(-2), qIn.enqueue(-2), qIn.enqueue(9), qIn.enqueue(-9);
    qAns.enqueue(-6), qAns.enqueue(6), qAns.enqueue(4), qAns.enqueue(-4), qAns.enqueue(0), qAns.enqueue(0), qAns.enqueue(18), qAns.enqueue(-18);
    n = 3;
    vec.resize(n);
    existVec.resize(n);
    cmdSet.enqueue("inbox"), cmdSet.enqueue("outbox"), cmdSet.enqueue("copyfrom"), cmdSet.enqueue("copyto");
    cmdSet.enqueue("add"), cmdSet.enqueue("sub"), cmdSet.enqueue("jump"), cmdSet.enqueue("jumpifzero");

    setUpBackground();
}


void Widget::on_levelButton3_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);

    qIn.clear(), qOut.clear(), qAns.clear(), cmdSet.clear();
    vec.clear(), existVec.clear();
    existCurrentBlock = 0;

    currentLevel = 3;
    qIn.enqueue(6), qIn.enqueue(2), qIn.enqueue(7), qIn.enqueue(7), qIn.enqueue(-9), qIn.enqueue(3), qIn.enqueue(-3), qIn.enqueue(-3);
    qAns.enqueue(7), qAns.enqueue(-3);
    n = 2;
    vec.resize(n);
    existVec.resize(n);
    cmdSet.enqueue("inbox"), cmdSet.enqueue("outbox"), cmdSet.enqueue("copyfrom"), cmdSet.enqueue("copyto");
    cmdSet.enqueue("add"), cmdSet.enqueue("sub"), cmdSet.enqueue("jump"), cmdSet.enqueue("jumpifzero");

    setUpBackground();
}


void Widget::on_levelButton4_clicked()
{
    QString filePathName = QFileDialog::getOpenFileName(this, "打开", "./", "JSON 文件 (*.json)");
    if (filePathName.isEmpty()) {
        QMessageBox::warning(this, "警告", "已取消选择关卡！");
    } else {
        ui->fileDirectory->setText(filePathName);
        QFile openFile(filePathName);
        openFile.open(QIODevice::ReadOnly);
        QByteArray fileContents = openFile.readAll();
        openFile.close();

        // todo: fileContents 导入之后需要判断格式是否合法

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
            if (checkResult()) {
                level = currentLevel;
                if (level >= 1) {
                    ui->levelButton2->setDisabled(false);
                } else if (level >= 2) {
                    ui->levelButton3->setDisabled(false);
                }
            }
            return;
        }

        QString cmd = cmdLines[currentCommand - 1].split(' ')[0];
        if (valid(cmd)) {
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
    QString str = "", aStr = "";

    str.append("输入序列：[");
    QQueue<int> tmp1 = qIn;

    while (!tmp1.empty()) {
        str.append(QString::number(tmp1.front()));
        tmp1.dequeue();
        if(!tmp1.empty()) {
            str.append(" ,");
        }
    }

    str.append("]\n");


    aStr.append(str), str.clear();

    str.append("目标输出序列：[");
    tmp1 = qAns;

    while (!tmp1.empty()) {
        str.append(QString::number(tmp1.front()));
        tmp1.dequeue();
        if(!tmp1.empty()) {
            str.append(" ,");
        }
    }

    str.append("]\n");


    aStr.append(str), str.clear();

    str.append("可用空地数：" + QString::number(n) + "\n");

    str.append("可用指令集：[");
    QQueue<QString> tmp2 = cmdSet;

    while (!tmp2.empty()) {
        str.append(tmp2.front());
        tmp2.dequeue();
        if(!tmp2.empty()) {
            str.append(", ");
        }
    }

    str.append("]\n");


    aStr.append(str), str.clear();

    ui->announcementLabel->setText(aStr);
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


bool Widget::checkResult() {
    if (qOut.size() != qAns.size()) {
        printFailMessage();
        return false;
    } else {
        while (!qOut.empty()) {
            int a = qOut.front(), b = qAns.front();
            qOut.dequeue(), qAns.dequeue();
            if (a != b) {
                printFailMessage();
                return false;
            }
        }
        printSuccessMessage();
        return true;
    }
}


bool Widget::valid(QString test) {
    for (QString &a : cmdSet) {
        if (a == test)
            return true;
    }
    return false;
}


void Widget::renderLevelButton(int l) {
    ui->levelButton2->setDisabled(true);
    ui->levelButton3->setDisabled(true);
    if (level >= 1)
        ui->levelButton2->setDisabled(false);
    if (level >= 2)
        ui->levelButton3->setDisabled(false);
}
