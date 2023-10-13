#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

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

    void on_selectFileDirectory_clicked();

    void on_levelButton1_clicked();

    void on_levelButton2_clicked();

    void on_levelButton3_clicked();

    void on_levelButton4_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
