#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QComboBox>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <QMouseEvent>
#include "character_split.h"
#include "detect.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private slots:
    void myupdate();
    
    void on_opencamera_clicked();

    void on_openfile_clicked();

    void on_pushButton_clicked();

    void on_stop_clicked();

    void myupdate2();

    void on_close_clicked();

    void print_s();

    void str_split(QString qstr);

    void on_logo_clicked();

    void label_show(QImage image);

    void baidu_ocr();

    void on_pushButton_2_clicked();

protected:

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    int x1,y1,x2,y2;
    void myupdate3();
    cv::Mat mat;
    cv::Rect m_rect;

private:
    Ui::MainWindow *ui;
    character_split d;
    detect c;
    QTimer *m_pTimer=new QTimer(this);
    QTimer *qtimer=new QTimer(this);

    //mylabel *PICTURE=new mylabel(this);
    cv::VideoCapture cap;

};

#endif // MAINWINDOW_H
