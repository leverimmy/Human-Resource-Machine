# Human Resource Machine

Repository for the codes of Fundamentals of Programming, 2023 Fall

**目录**

- [Human Resource Machine](#human-resource-machine)
	- [构建环境](#构建环境)
	- [设计思路](#设计思路)
	- [工程结构](#工程结构)
	- [具体实现](#具体实现)
	  - [设置关卡信息](#设置关卡信息)
	  - [执行玩家输入的指令](#执行玩家输入的指令)
	  - [判断输入的指令是否合法](#判断输入的指令是否合法)
	- [界面设计](#界面设计)
	- [游戏测试](#游戏测试)
	  - [inbox](#inbox)
	  - [outbox](#outbox)
	  - [add](#add)
	  - [sub](#sub)
	  - [copyto](#copyto)
	  - [copyfrom](#copyfrom)
	  - [jump](#jump)
	  - [jumpifzero](#jumpifzero)
	  - [从文件读入指令](#从文件读入指令)
	  - [其他异常情况](#其他异常情况)
	- [自由创新关卡](#自由创新关卡)
	- [小组分工](#小组分工)
	

## 构建环境

Qt Creator 12.0.1 (Enterprise)

Qt 6.6.0 以及配套版本的 MinGW 编译器

## 设计思路

我们选择使用 Qt Creator 进行大作业程序的编写。它提供了一条设计图形界面并将其与程序整合起来的简单途径。

在设计思路上，我们主要使用队列来模拟传送带，并使用向量来存储空地的状态。具体而言，我们在 `qIn` 与 `qAns` 两个队列中存储了当前关卡的输入序列与目标序列，并用 `qOut` 队列来存储当前的输出序列。在指令执行完成后，程序将比对 `qAns` 与 `qOut`，并以此判断关卡是否通过。

## 工程结构

工程结构如下表所示。

```
- HumanResourceMachine
	- Header Files
		- widget.h
	- Source Files
		- main.cpp
		- widget.cpp
			- void on_backButton1_clicked()
			- void on_backButton2_clicked()
			- void on_backButton3_clicked()
			- void on_startButton_clicked()
			- void on_aboutButton_clicked()
			- void on_exitButton_clicked()
			- void on_volumnHorizontalSilder_valueChanged()
			- void on_levelButton1_clicked()
			- void on_levelButton2_clicked()
			- void on_levelButton3_clicked()
			- void on_levelButton4_clicked()
			- void on_confirmNextStepButton_clicked()
			- void drawStatus()
			- void setUpBackground()
			- void printSuccessMessage()
			- void printFailMessage()
			- void printErrorMessage()
			- bool checkResult()
			- int myToInt(QString)
			- void renderLevelButton(int)
	- Resources.qrc
	- widget.ui
```

- `widget.h`
- `main.cpp`
- `widget.cpp`  中实现了程序的大部分功能。
  - `on_backButton1_clicked()`、`on_backButton2_clicked()`、`on_backButton3_clicked()` 在按下不同页面的`返回`键时被调用。它们将程序的界面设置为上一级页面。
  - `on_startButton_clicked()` 在按下`开始游戏`键时被调用。它将试图读取储存于程序可执行文件所在目录中的存档 `userdata.txt`，并根据其解锁可游玩的关卡。随后，它将程序界面设置为关卡选择页面。
  - `on_aboutButton_clicked()` 在按下`设置`键时被调用。它将程序的界面设置为设置页面。
  - `on_exitButton_clicked()` 在按下`退出游戏`键时被调用。它将目前已经通关的关卡编号写入 `userdata.txt` 中，随后关闭程序。
  - `on_volumnHorizontalSlider_valueChanged()` 在设置界面的音量滑块移动时被调用。它将程序背景音乐的音量设置为与滑块位置相对应的音量。
  - `on_levelButton1_clicked()`、`on_levelButton2_clicked()`、`on_levelButton3_clicked()` 分别在按下`第一关`、`第二关`、`第三关`键时被调用。它们将程序的界面设置为游戏页面，并根据大作业要求中的内容设置关卡信息。
  - `on_levelButton4_clicked()` 在 `第四关` 键被按下时被调用。它将弹出一个文件选择对话框并试图读取一个 `.json` 文件。若读取成功，则它将程序的界面设置为游戏页面，并根据 `.json` 文件中的信息设置关卡信息。
  - `on_confirmNextStepButton_clicked()` 在 `下一步`/`检查结果` 键被按下时被调用。它将读入一行游戏页面文本框中玩家输入的信息并检查其合法性，随后执行相关操作。
  - `drawStatus()` 在游戏页面更新时被调用。它将根据当前关卡目前的游戏状态，在页面上显示相关信息（如当前输入序列、目标输出序列、空地状态）。
  - `setUpBackground()` 在程序界面被设置为游戏页面时被调用。它将根据当前关卡的初始信息，在页面上显示相关信息（如可用指令集、可用空地数等）。
  - `printSuccessMessage()` 在关卡成功通过时被调用。它弹出一个对话框，显示当前关卡已成功完成。
  - `printFailMessage()` 在关卡失败时被调用。它弹出一个对话框，显示关卡失败，并提供 `重新开始` 与 `返回主菜单` 两个选项。
  - `printErrorMessage()` 在输入指令非法时被调用。它弹出一个对话框，显示关卡在当前位置出现了非法指令，并提供 `重新开始` 与 `返回主菜单` 两个选项。
  - `checkResult()` 在输入的指令执行完成后被调用。它比对当前输出序列与目标序列，并相应调用 `printSuccessMessage()` 或 `printErrorMessage()`。
  - `myToInt(QString)` 在程序检查输入的指令参数时被调用。它检查当前指令的参数是否合法，若合法则返回参数，否则返回 `-1`。
  - `renderLevelButton(int)` 在程序被设置为选关页面时被调用。它根据当前已通关的关卡来设置当前哪些关卡可以游玩。
- `Resources.qrc` 是程序的资源文件，储存了程序的背景音乐、字体与图片等。
- `widget.ui` 中存储了程序图形界面的相关设置。

## 具体实现

### 设置关卡信息

以 `on_levelButton4_clicked()` 为例。

```c++
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
```

它所接受的 `.json` 文件应格式如下：

```json
{
    "input": [1, 2, 7, 7, 9, 3, 3, 3],
    "output": [7, -3],
    "vacancy": 6,
    "cmd": [0, 1, 2, 3, 4, 5]
}
```

仓库中的 `example.json` 即为此例。其中 `input`表示初始输入序列，`output` 表示目标输出序列，`vacancy` 表示可用空地数量，`cmd` 表示可用指令集。

指令集中的数字对应关系如下：

| 指令集中数字 |    指令    |
| :----------: | :--------: |
|      0       |   inbox    |
|      1       |   outbox   |
|      2       |  copyfrom  |
|      3       |   copyto   |
|      4       |    add     |
|      5       |    sub     |
|      6       |    jump    |
|      7       | jumpifzero |

成功读取后，函数将 `input` 序列存入队列 `qIn` 中，将 `output` 序列存入队列 `qAns` 中，根据 `vacancy` 的值设置向量 `vec` 与 `existVec` 的大小，并根据 `cmd` 设置关卡的可用指令集。

最后，函数将程序界面设置为游戏页面，然后调用 `setUpBackground()` 函数展示关卡信息。

### 执行玩家输入的指令

代码如下：

```c++
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
```

若还未读入指令（第一次点击 `确认` 键），函数将读入输入文本框中的全部内容，并去除其中空行与多余的空格，将其分割后存入列表 `cmdLines` 中，随后将 `确认` 键的文本改为 `下一步`。

若已经读入了指令，函数将按行进行以下操作：首先检查输入的指令是否在本关卡的可用指令集中，随后调用 `myToInt(QString)` 函数判断参数是否合法并获取参数；若未出现异常则执行指令，并在最后一条指令执行完成后调用 `checkResult()` 函数确认关卡是否成功通过。

### 判断输入的指令是否合法

```c++
// 判断一个字符串是否为自然数
int myToInt(QString str) {
    for (auto x : str) {
        // 即：不能出现除数字以外的字符
        if (!x.isDigit())
            return -1;
    }
    return str.toInt();
}
```

`myToInt(QString)` 函数用于判断指令的参数是否合法。本次大作业中所有指令的参数最多只有一个，且不存在除数字外的字符（无小数点、无负号），可根据这一点进行判断。

## 界面设计

程序共有四个界面。进入游戏将显示主界面：

<img src="./pics/MainPage.png" width = 400>


通过此界面可以前往关卡选择界面，设置界面或者退出游戏。

从关卡选择界面可以前往四个关卡，或者返回主界面。

<img src="./pics/LevelPage.png" width = 400>


游戏界面显示了关卡的当前状态。玩家可以在页面中央的文本框中输入指令。

<img src="./pics/GamePage.png" width = 400>


设置界面可以调整游戏背景音乐的音量。

<img src="./pics/AboutPage.png" width = 400>


## 游戏测试

游戏可以正确执行每条指令，并能够在其出错时识别出来。

### inbox

<img src="./pics/Inbox1.png" width = 400>


<img src="./pics/Inbox2.png" width = 400>


### outbox

<img src="./pics/Outbox1.png" width = 400>


<img src="./pics/Outbox2.png" width = 400>


当前积木不存在时，程序会报错。

<img src="./pics/OutboxErr1_1.png" width = 400>


<img src="./pics/OutboxErr1_2.png" width = 400>


### add

<img src="./pics/Add1.png" width = 400>


<img src="./pics/Add2.png" width = 400>


当前积木不存在时，程序会报错。

<img src="./pics/AddErr1_1.png" width = 400>


<img src="./pics/AddErr1_2.png" width = 400>


目标空地不存在积木时，程序会报错。

<img src="./pics/AddErr2_1.png" width = 400>


<img src="./pics/AddErr2_2.png" width = 400>


### sub

<img src="./pics/Sub1.png" width = 400>


<img src="./pics/Sub2.png" width = 400>


当前积木不存在时，程序会报错。

<img src="./pics/SubErr1_1.png" width = 400>


<img src="./pics/SubErr1_2.png" width = 400>


目标空地不存在积木时，程序会报错。

<img src="./pics/SubErr2_1.png" width = 400>


<img src="./pics/SubErr2_2.png" width = 400>


### copyto

<img src="./pics/Copyto1.png" width = 400>


<img src="./pics/Copyto2.png" width = 400>


当前积木不存在时，程序会报错。

<img src="./pics/CopytoErr1_1.png" width = 400>


<img src="./pics/CopytoErr1_2.png" width = 400>


目标空地不存在时，程序会报错。

<img src="./pics/CopytoErr2_1.png" width = 400>


<img src="./pics/CopytoErr2_2.png" width = 400>


### copyfrom

<img src="./pics/Copyfrom1.png" width = 400>


<img src="./pics/Copyfrom2.png" width = 400>


目标空地没有积木时，程序会报错。

<img src="./pics/CopyfromErr1_1.png" width = 400>


<img src="./pics/CopyfromErr1_2.png" width = 400>


目标空地不存在时，程序会报错。

<img src="./pics/CopyfromErr2_1.png" width = 400>


<img src="./pics/CopyfromErr2_2.png" width = 400>


### jump

<img src="./pics/Jump1.png" width = 400>


<img src="./pics/Jump2.png" width = 400>


<img src="./pics/Jump3.png" width = 400>


目标指令不存在时，程序会报错。

<img src="./pics/JumpErr1_1.png" width = 400>


<img src="./pics/JumpErr1_2.png" width = 400>


### jumpifzero

<img src="./pics/Jumpifzero1.png" width = 400>


<img src="./pics/Jumpifzero2.png" width = 400>


<img src="./pics/Jumpifzero3.png" width = 400>


当前方块不为0时，程序不会做任何操作。

<img src="./pics/Jumpifzero4.png" width = 400>


<img src="./pics/Jumpifzero5.png" width = 400>


目标指令不存在时，程序会报错。

<img src="./pics/JumpifzeroErr1_1.png" width = 400>


<img src="./pics/JumpifzeroErr1_2.png" width = 400>


当前积木不存在时，程序会报错。

<img src="./pics/JumpifzeroErr2_1.png" width = 400>


<img src="./pics/JumpifzeroErr2_2.png" width = 400>

### 从文件读入指令

本程序可以从一个 `.txt` 文件中读入指令。

<img src="./pics/FileIn1.png" width = 400>

在文本框为空时直接点击确认，则会出现弹窗，可以选择文件。

<img src="./pics/FileIn2.png" width = 400>

一个事例是当前目录下的 `program.txt` 文件：

```
inbox
copyto 0
inbox
copyto 1
copyfrom 0
sub 1
outbox
copyfrom 1
sub 0
outbox
inbox
copyto 0
inbox
copyto 1
copyfrom 0
sub 1
outbox
copyfrom 1
sub 0
outbox
inbox
copyto 2
sub 2
outbox
inbox
sub 2
outbox
inbox
copyto 0
inbox
copyto 1
copyfrom 0
sub 1
outbox
copyfrom 0
add 1
add 1
add 1
outbox
```

加载文件后，文件内的指令将会显示在文本框中，可以继续执行。

<img src="./pics/FileIn3.png" width = 400>

### 其他异常情况

出现未定义指令

<img src="./pics/Err1_1.png" width = 400>


<img src="./pics/Err1_2.png" width = 400>


指令参数错误（为负数）

<img src="./pics/Err2_1.png" width = 400>


<img src="./pics/Err2_2.png" width = 400>


指令参数错误（不为整数）

<img src="./pics/Err3_1.png" width = 400>


<img src="./pics/Err3_2.png" width = 400>


指令参数错误（参数数量过多）

<img src="./pics/Err4_1.png" width = 400>


<img src="./pics/Err4_2.png" width = 400>


除此以外，本程序还可以自动去除玩家输入中的空行以及多余的空格，成功识别出指令及参数。

<img src="./pics/Space1.png" width = 400>


<img src="./pics/Space2.png" width = 400>


<img src="./pics/Space3.png" width = 400>


<img src="./pics/Space4.png" width = 400>


## 自由创新关卡

我们所设计的自由创新关卡如下：

<img src="./pics/NewLevel1.png" width = 400>


若要游玩这一关卡，可以参考当前目录下的事例 `extra_level.json`，文件内容如下：

```json
{
    "input": [1],
    "output": [2147483647],
    "vacancy": 3,
    "cmd": [0, 1, 2, 3, 4, 5, 6, 7]
}
```

仓库中的 `extra_level.json` 即为此例。这一关卡的思路很简单，就是通过累加的方式将 `1` 变为`2147483647`。目标过大，直接 `add` 显然无法解决问题，考虑利用空位存储当前数字，每次将其加倍，然而也需要三十余次加倍的过程。为节省输入指令的量，可以考虑使用 `jump` 与 `jumpifzero`。

一个可行的输入指令序列如下：

```
inbox
copyto 2
copyto 0
copyto 1
add 0
jumpifzero 8
jump 3
copyfrom 1
sub 2
outbox
```

每次 `jump` 后，前两块空地，以及当前积木的数字都会加倍，而2号空地则保持 `1` 不变。

<img src="./pics/NewLevel2.png" width = 400>


这一进程将持续到数字自然溢出，变为 `-2147483648`。

<img src="./pics/NewLevel3.png" width = 400>


<img src="./pics/NewLevel4.png" width = 400>


<img src="./pics/NewLevel5.png" width = 400>


随后，在执行 `add 0` 后，当前积木将变为 `0`，满足 `jumpifzero` 的要求。

<img src="./pics/NewLevel6.png" width = 400>


最后，通过 `copyfrom 1` 后 `sub 2` 将当前积木由 `-2147483648` 变为 `2147483647` 后输出，全过程仅需 10 行代码。

## 小组分工

在本次大作业的开发过程中，熊泽恩负责 70% 的代码以及后续问题的调试，汪宇萌负责 30% 的代码、测试以及 UI 设计。

在整个开发过程中，我们遇到了不少困难与挑战。然而，通过一步步地查找资料、不断测试调试，我们最终摸索出了解决方法，完成了本次大作业的开发。这一经历是十分难忘并且充满教育意义的。我们增进了对应用开发的了解，更熟练地掌握了各类开发工具的使用，也亲身体会到了开发过程中复现问题、调试代码的重要与艰辛。

最后，我们由衷感谢清华大学计算机系 [徐明星](https://www.bnrist.tsinghua.edu.cn/info/1091/2842.htm) 老师的指导，以及助教们的答疑。	
