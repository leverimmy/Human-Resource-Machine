#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    // 设置背景音乐
    backgroundMusic = new QSoundEffect();
    // 读取背景音乐
    backgroundMusic->setSource(QUrl::fromLocalFile(":/audio/res/bgm.wav"));
    // 循环播放
    backgroundMusic->setLoopCount(QSoundEffect::Infinite);
    // 设置背景音乐音量大小为 50
    backgroundMusic->setVolume(0.5f);
    // 播放背景音乐
    backgroundMusic->play();
    // 设置当前页面为 0 号页面（即主页面）
    ui->stackedWidget->setCurrentIndex(0);
}


Widget::~Widget() {
    delete ui;
}


void Widget::on_backButton1_clicked() {
    ui->stackedWidget->setCurrentIndex(0);
}


void Widget::on_backButton2_clicked() {
    ui->stackedWidget->setCurrentIndex(0);
}


void Widget::on_backButton3_clicked() {
    ui->stackedWidget->setCurrentIndex(1);
}


void Widget::on_startButton_clicked() {
    // 尝试从默认路径中读入玩家存档
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


void Widget::on_aboutButton_clicked() {
    ui->stackedWidget->setCurrentIndex(2);
}


void Widget::on_exitButton_clicked() {
    // 退出游戏时，需要对当前游戏进度进行存档
    QFile writeFile("./userdata.txt");
    writeFile.open(QIODevice::WriteOnly);
    writeFile.write(QString::number(level).toLatin1());
    writeFile.close();

    close();
}


void Widget::on_volumnHorizontalSlider_valueChanged(int value) {
    // 更改屏幕上显示的音量大小
    ui->volumnValue->setText(QString::number(value));
    // 设置背景音乐音量大小
    backgroundMusic->setVolume(value / 100.0f);
}


// 开始第一关游戏
void Widget::on_levelButton1_clicked() {
    ui->stackedWidget->setCurrentIndex(3);
    // 重置
    qIn.clear(), qOut.clear(), qAns.clear(), cmdSet.clear();
    vec.clear(), existVec.clear(), cmdLines.clear();
    existCurrentBlock = 0;
    // 设置第一关的游戏配置
    currentLevel = 1;
    qIn.enqueue(1), qIn.enqueue(2);
    qAns.enqueue(1), qAns.enqueue(2);
    n = 0;
    vec.resize(n);
    existVec.resize(n);
    cmdSet.enqueue("inbox"), cmdSet.enqueue("outbox");
    // 绘制背景
    setUpBackground();
}


// 开始第二关游戏
void Widget::on_levelButton2_clicked() {
    ui->stackedWidget->setCurrentIndex(3);
    // 重置
    qIn.clear(), qOut.clear(), qAns.clear(), cmdSet.clear();
    vec.clear(), existVec.clear(), cmdLines.clear();
    existCurrentBlock = 0;
    // 设置第二关的游戏配置
    currentLevel = 2;
    qIn.enqueue(3), qIn.enqueue(9), qIn.enqueue(5), qIn.enqueue(1), qIn.enqueue(-2), qIn.enqueue(-2), qIn.enqueue(9), qIn.enqueue(-9);
    qAns.enqueue(-6), qAns.enqueue(6), qAns.enqueue(4), qAns.enqueue(-4), qAns.enqueue(0), qAns.enqueue(0), qAns.enqueue(18), qAns.enqueue(-18);
    n = 3;
    vec.resize(n);
    existVec.resize(n);
    cmdSet.enqueue("inbox"), cmdSet.enqueue("outbox"), cmdSet.enqueue("copyfrom"), cmdSet.enqueue("copyto");
    cmdSet.enqueue("add"), cmdSet.enqueue("sub"), cmdSet.enqueue("jump"), cmdSet.enqueue("jumpifzero");
    // 绘制背景
    setUpBackground();
}


// 开始第三关游戏
void Widget::on_levelButton3_clicked() {
    ui->stackedWidget->setCurrentIndex(3);
    // 重置
    qIn.clear(), qOut.clear(), qAns.clear(), cmdSet.clear();
    vec.clear(), existVec.clear(), cmdLines.clear();
    existCurrentBlock = 0;
    // 设置第三关的游戏配置
    currentLevel = 3;
    qIn.enqueue(6), qIn.enqueue(2), qIn.enqueue(7), qIn.enqueue(7), qIn.enqueue(-9), qIn.enqueue(3), qIn.enqueue(-3), qIn.enqueue(-3);
    qAns.enqueue(7), qAns.enqueue(-3);
    n = 3;
    vec.resize(n);
    existVec.resize(n);
    cmdSet.enqueue("inbox"), cmdSet.enqueue("outbox"), cmdSet.enqueue("copyfrom"), cmdSet.enqueue("copyto");
    cmdSet.enqueue("add"), cmdSet.enqueue("sub"), cmdSet.enqueue("jump"), cmdSet.enqueue("jumpifzero");
    // 绘制背景
    setUpBackground();
}

// 开始第四关游戏
void Widget::on_levelButton4_clicked() {
    // 重置
    qIn.clear(), qOut.clear(), qAns.clear(), cmdSet.clear();
    vec.clear(), existVec.clear(), cmdLines.clear();
    existCurrentBlock = 0;
    // 设置第四关游戏配置
    currentLevel = 4;
    // 从 JSON 文件中读取关卡配置
    QString filePathName = QFileDialog::getOpenFileName(this, "打开", "./", "JSON 文件 (*.json)");
    if (filePathName.isEmpty()) {
        QMessageBox::warning(this, "警告", "已取消选择关卡！");
    } else {
        QFile openFile(filePathName);
        openFile.open(QIODevice::ReadOnly);
        QByteArray fileContents = openFile.readAll();
        openFile.close();

        QJsonObject jsonObj = QJsonDocument::fromJson(fileContents).object();

        QJsonArray jsonArray = jsonObj.value("input").toArray();

        for (const QJsonValue& element : jsonArray)
            qIn.enqueue(element.toInt());

        jsonArray = jsonObj.value("output").toArray();
        for (const QJsonValue& element : jsonArray)
            qAns.enqueue(element.toInt());

        n = jsonObj.value("vacancy").toInt();
        vec.resize(n);
        existVec.resize(n);

        jsonArray = jsonObj.value("cmd").toArray();
        for (const QJsonValue& element : jsonArray) {
            switch (element.toInt()) {
            case 0:
                cmdSet.enqueue("inbox");
                break;
            case 1:
                cmdSet.enqueue("outbox");
                break;
            case 2:
                cmdSet.enqueue("copyfrom");
                break;
            case 3:
                cmdSet.enqueue("copyto");
                break;
            case 4:
                cmdSet.enqueue("add");
                break;
            case 5:
                cmdSet.enqueue("sub");
                break;
            case 6:
                cmdSet.enqueue("jump");
                break;
            case 7:
                cmdSet.enqueue("jumpifzero");
                break;
            default:
                break;
            }
        }
        ui->stackedWidget->setCurrentIndex(3);
        setUpBackground();
    }
}


// 判断一个字符串是否为自然数
int myToInt(QString str) {
    for (auto x : str) {
        // 即：不能出现除数字以外的字符
        if (!x.isDigit())
            return -1;
    }
    return str.toInt();
}

void Widget::on_confirmNextStepButton_clicked() {
    // 如果还没有开始执行指令
    if (!doing) {
        // 开始读入指令
        QStringList allCmdLinesWithoutTrim = ui->cmdTextEdit->toPlainText().split("\n");

        for (const QString& element : allCmdLinesWithoutTrim) {
            QString contains;
            int len = element.length();
            for (int i = 0; i < len; i++) {
                // 连续的两个空格，则前一个省略
                if (element[i] == ' ' && i < len - 1 && element[i + 1] == ' ')
                    continue;
                // 否则直接接到 contains 里
                else
                    contains.append(element[i]);
            }
            // 再除去首尾空格
            contains = contains.trimmed();
            // 如果 contains 非空
            if (contains.size())
                cmdLines.push_back(contains);
        }
        // 指令数量
        m = cmdLines.size();
        // 输入的指令为空
        if (m == 0) {
            printFailMessage();
            return;
        }
        currentCommand = 1;
        qOut.clear();
        // 设置输入框不可修改
        ui->cmdTextEdit->setFocusPolicy(Qt::NoFocus);
        ui->currentStepLabel->setText(cmdLines[currentCommand - 1]);
        ui->confirmNextStepButton->setText("下一步");
        doing = true;
    } else {
        if (currentCommand == m + 1) {
            // 如果通关，则更新玩家进度
            if (checkResult()) {
                if (currentLevel != 4)
                    level = currentLevel;
                renderLevelButton(level);
            }
            return;
        }
        // 当前指令
        QString cmd = cmdLines[currentCommand - 1].split(' ')[0];
        // 参数个数
        int argc = cmdLines[currentCommand - 1].split(' ').size() - 1;
        if (cmdSet.contains(cmd)) {
            if (cmd == "inbox") {
                if (argc != 0) {
                    printErrorMessage();
                    return;
                }
                if (qIn.empty()) {
                    checkResult();
                }
                currentBlock = qIn.front();
                qIn.dequeue();
                existCurrentBlock = true;
            } else if (cmd == "outbox") {
                if (existCurrentBlock == false || argc != 0) {
                    printErrorMessage();
                    return;
                }
                qOut.enqueue(currentBlock);
                existCurrentBlock = false;
            } else if (cmd == "add") {
                int x = myToInt(cmdLines[currentCommand - 1].split(' ')[1]);
                if (existCurrentBlock == false || x >= vec.size() || x < 0 || existVec[x] == false || argc != 1) {
                    printErrorMessage();
                    return;
                }
                currentBlock += vec[x];
            } else if (cmd == "sub") {
                int x = myToInt(cmdLines[currentCommand - 1].split(' ')[1]);
                if (existCurrentBlock == false || x >= vec.size() || x < 0 || existVec[x] == false || argc != 1) {
                    printErrorMessage();
                    return;
                }
                currentBlock -= vec[x];
            } else if (cmd == "copyto") {
                int x = myToInt(cmdLines[currentCommand - 1].split(' ')[1]);
                if (existCurrentBlock == false || x >= vec.size() || x < 0 || argc != 1) {
                    printErrorMessage();
                    return;
                }
                vec[x] = currentBlock;
                existVec[x] = true;
            } else if (cmd == "copyfrom") {
                int x = myToInt(cmdLines[currentCommand - 1].split(' ')[1]);
                if (x >= vec.size() || x < 0 || existVec[x] == false || argc != 1) {
                    printErrorMessage();
                    return;
                }
                currentBlock = vec[x];
                existCurrentBlock = true;
            } else if (cmd == "jump") {
                int x = myToInt(cmdLines[currentCommand - 1].split(' ')[1]);
                if (x > m || x <= 0 || argc != 1) {
                    printErrorMessage();
                    return;
                }
                currentCommand = x;
                ui->currentStepLabel->setText(cmdLines[currentCommand - 1]);
                return;
            } else if (cmd == "jumpifzero") {
                int x = myToInt(cmdLines[currentCommand - 1].split(' ')[1]);
                if (existCurrentBlock == false || x > m || x <= 0 || argc != 1) {
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
        // 如果当前不是最后一步
        if (currentCommand <= m) {
            ui->currentStepLabel->setText(cmdLines[currentCommand - 1]);
        } else { // 否则“下一步”按钮变为“检查”按钮，并且屏幕上显示的“当前指令”为空
            ui->currentStepLabel->clear();
            ui->confirmNextStepButton->setText("检查");
        }
        drawStatus();
    }
}


// 在背景中填入数据
void Widget::drawStatus() {
    // 机器人手上拿的数字
    if (!existCurrentBlock) {
        ui->robotLabel->setText("无");
    } else {
        ui->robotLabel->setText(QString::number(currentBlock));
    }
    // 输入传送带
    QString inString = "";
    for (auto element : qIn) {
        inString.append(QString::number(element));
        inString.append('\n');
    }
    ui->inputTextLabel->setText(inString);
    // 输出传送带
    QString outString = "";
    for (auto element : qOut) {
        outString.append(QString::number(element));
        outString.append('\n');
    }
    ui->outputTextLabel->setText(outString);
    // 空地可用情况
    QString vacantSpacesString = "";
    if (!n) {
        vacantSpacesString = "本关卡无可用空地";
    } else {
        for (int i = 0; i < n; i++) {
            existVec[i] ? vacantSpacesString.append(QString::number(vec[i])) : vacantSpacesString.append("空");
            vacantSpacesString.append(" ");
        }
    }
    ui->vacantTextLabel->setText(vacantSpacesString);
}


void Widget::setUpBackground() {
    // 重置
    ui->cmdTextEdit->setFocusPolicy(Qt::ClickFocus);
    ui->cmdTextEdit->clear();
    ui->currentStepLabel->clear();
    ui->confirmNextStepButton->setText("确认");
    // 输入序列
    QString inString = "输入序列：[";
    for (int i = 0; i < qIn.size(); i++) {
        int element = qIn.at(i);
        inString.append(QString::number(element));
        i + 1 != qIn.size() ? inString.append(", ") : inString.append("]\n");
    }
    // 输出序列
    QString outString = "目标输出序列：[";
    for (int i = 0; i < qAns.size(); i++) {
        int element = qAns.at(i);
        outString.append(QString::number(element));
        i + 1 != qAns.size() ? outString.append(", ") : outString.append("]\n");
    }
    // 空地可用情况
    QString vacantSpacesString = "可用空地数：" + QString::number(n) + "\n";
    // 当前关卡指令集
    QString cmdSetString = "可用指令集：[";
    for (int i = 0; i < cmdSet.size(); i++) {
        QString element = cmdSet.at(i);
        cmdSetString.append(element);
        i + 1 != cmdSet.size() ? cmdSetString.append(", ") : cmdSetString.append("]\n");
    }
    ui->announcementLabel->setText(inString + outString + vacantSpacesString + cmdSetString);
    drawStatus();
    doing = false;
}


// 输出“任务成功”的消息提示
void Widget::printSuccessMessage() {
    bool ok = QMessageBox::information(this, " ", "Success: 任务成功！");
    // 如果点击“确认”按钮，就返回关卡选择页面
    if (ok) {
        ui->stackedWidget->setCurrentIndex(1);
    }
}


// 输出“任务失败”的消息提示
void Widget::printFailMessage() {
    // 设置提示框内容
    QPushButton *okbtn = new QPushButton("重新开始");
    QPushButton *cancelbtn = new QPushButton("返回主菜单");
    QMessageBox *mymsgbox = new QMessageBox;

    mymsgbox->setIcon(QMessageBox::Warning);
    mymsgbox->setWindowTitle(" ");
    mymsgbox->setText("Fail: 任务失败！");
    mymsgbox->addButton(okbtn, QMessageBox::AcceptRole);
    mymsgbox->addButton(cancelbtn, QMessageBox::RejectRole);
    mymsgbox->setWindowFlags(Qt::Widget);
    mymsgbox->setWindowFlags(Qt::WindowStaysOnTopHint);
    mymsgbox->show();
    mymsgbox->exec();

    // 处理提示框按钮事件
    // 玩家选择“重新开始”以重玩本关卡
    if (mymsgbox->clickedButton() == okbtn) {
        if (currentLevel == 1)
            on_levelButton1_clicked();
        if (currentLevel == 2)
            on_levelButton2_clicked();
        if (currentLevel == 3)
            on_levelButton3_clicked();
        if (currentLevel == 4)
            ui->stackedWidget->setCurrentIndex(1);
    } else { // 否则返回主菜单
        ui->stackedWidget->setCurrentIndex(0);
    }
}


// 输出命令包含“语法错误”的消息提示
void Widget::printErrorMessage() {
    // 设置提示框内容
    QString str = "Error: 您第 " + QString::number(currentCommand) + " 行的命令为非法命令。";
    QPushButton *okbtn = new QPushButton("重新开始");
    QPushButton *cancelbtn = new QPushButton("返回主菜单");
    QMessageBox *mymsgbox = new QMessageBox;

    mymsgbox->setIcon(QMessageBox::Warning);
    mymsgbox->setWindowTitle(" ");
    mymsgbox->setText(str);
    mymsgbox->addButton(okbtn, QMessageBox::AcceptRole);
    mymsgbox->addButton(cancelbtn, QMessageBox::RejectRole);
    mymsgbox->setWindowFlags(Qt::Widget);
    mymsgbox->setWindowFlags(Qt::WindowStaysOnTopHint);
    mymsgbox->show();
    mymsgbox->exec();

    // 处理提示框按钮事件
    // 玩家选择“重新开始”以重玩本关卡
    if (mymsgbox->clickedButton() == okbtn) {
        if (currentLevel == 1)
            on_levelButton1_clicked();
        if (currentLevel == 2)
            on_levelButton2_clicked();
        if (currentLevel == 3)
            on_levelButton3_clicked();
        if (currentLevel == 4)
            ui->stackedWidget->setCurrentIndex(1);
    } else { // 否则返回主菜单
        ui->stackedWidget->setCurrentIndex(0);
    }
}


// 检查当前输出传送带上的数字是否与答案一样
bool Widget::checkResult() {
    // 如果两个队列中包含元素个数都不同，那肯定不相同
    if (qOut.size() != qAns.size()) {
        printFailMessage();
        return false;
    } else {
        // 否则挨个取出队首，进行比对
        while (!qOut.empty()) {
            int a = qOut.front(), b = qAns.front();
            qOut.dequeue(), qAns.dequeue();
            // 如果不相等，则答案不一样
            if (a != b) {
                printFailMessage();
                return false;
            }
        }
        printSuccessMessage();
        return true;
    }
}


// 渲染关卡选择按钮
void Widget::renderLevelButton(int l) {
    // 首先重置
    ui->levelButton2->setDisabled(true);
    ui->levelButton3->setDisabled(true);
    // 假定玩家已经完成了第 l 关，那么他/她应该能够打开 [1, l+1] 的关卡
    if (l >= 1)
        ui->levelButton2->setDisabled(false);
    if (l >= 2)
        ui->levelButton3->setDisabled(false);
    // 注意，第 4 关的按钮永远是 able 的，因为它是自定义关卡
}
