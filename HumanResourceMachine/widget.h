#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QQueue>

#define N 6

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
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

    void on_volumnHorizontalSlider_sliderMoved(int position);

    void on_levelButton1_clicked();

    void on_levelButton2_clicked();

    void on_levelButton3_clicked();

    void on_levelButton4_clicked();

    void on_confirmNextStepButton_clicked();

private:
    Ui::Widget *ui;
    bool doing;
    bool existCurrentBlock;
    int currentBlock, currentCommand;
    int n, m, level = 0, currentLevel;
    QQueue<int> qIn, qOut, qAns;
    QQueue<QString> cmdSet;
    QStringList cmdLines;
    QVector<bool> existVec;
    QVector<int> vec;

    void drawStatus();
    void setUpBackground();
    void printSuccessMessage();
    void printFailMessage();
    void printErrorMessage();
    bool checkResult();
    bool valid(QString);
    void renderLevelButton(int);
};
#endif // WIDGET_H
