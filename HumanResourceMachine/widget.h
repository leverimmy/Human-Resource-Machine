#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QQueue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtMultimedia/QSoundEffect>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget {
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_backButton1_clicked();
    void on_backButton2_clicked();
    void on_backButton3_clicked();
    void on_startButton_clicked();
    void on_aboutButton_clicked();
    void on_exitButton_clicked();
    void on_volumnHorizontalSlider_valueChanged(int value);
    void on_levelButton1_clicked();
    void on_levelButton2_clicked();
    void on_levelButton3_clicked();
    void on_levelButton4_clicked();
    void on_confirmNextStepButton_clicked();

private:
    Ui::Widget *ui;
    QSoundEffect *backgroundMusic;
    // 是否已经开始执行指令：开始执行后就不能再修改输入数据
    bool doing;
    // 机器人手上是否有数字
    bool existCurrentBlock;
    // 机器人手上的数字
    int currentBlock;
    // 当前处理的指令序号
    int currentCommand;
    // 空地数
    int n;
    // 指令条数
    int m;
    // 玩家已经过了的关卡
    int level = 0;
    // 当前正在玩的关卡
    int currentLevel;
    // 传送带
    QQueue<int> qIn, qOut, qAns;
    // 当前关卡合法指令集
    QQueue<QString> cmdSet;
    // 玩家输入的所有指令
    QStringList cmdLines;
    // 空地上是否有数字
    QVector<bool> existVec;
    // 空地上的数字
    QVector<int> vec;

    void drawStatus();
    void setUpBackground();
    void printSuccessMessage();
    void printFailMessage();
    void printErrorMessage();
    bool checkResult();
    void renderLevelButton(int);
};
#endif // WIDGET_H
