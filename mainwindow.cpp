#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "character_split.h"
#include "detect.h"
#include <opencv2/opencv.hpp>
#include "ocr.h"
#include "my_opencv.hpp"
#include <QTimer>
#include <QScrollBar>
#include <QComboBox>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <ctime>
#include <stdlib.h>
#include <cstring>
#include <vector>
#include <iostream>
#include <QRect>
#include <QLabel>
#include "ELM-master/elm_in_elm_model.h"

using namespace std;
using namespace cv;

std::string app_id = "15900738";
std::string api_key = "GTViyE9DlgFkK0A3QXzwITRN";
std::string secret_key = "Z2wAMGZ9WxjAWMk3NyuVerubM9q7wgQw";

aip::Ocr client(app_id, api_key, secret_key);

bool stop_button;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    ui->textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    ui->textEdit->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->maximumHeight());
    //滚条设置在最后

    ui->textEdit->setLineWrapMode(QTextEdit::NoWrap);

    ui->textEdit_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    ui->textEdit_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    ui->textEdit_2->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->maximumHeight());
    //滚条设置在最后

    ui->textEdit_2->setLineWrapMode(QTextEdit::NoWrap);

    cap.open(0);


    m_pTimer->setInterval(100);
    qtimer->setInterval(100);

    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(myupdate()));
    connect(qtimer,SIGNAL(timeout()),this,SLOT(myupdate2()));

    //connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(print_s()));
}

MainWindow::~MainWindow()
{
    cap.release();

    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons()==Qt::LeftButton)
    {
        //imshow("mat",mat);
        //cv::Point tmpPoint;
        float ratio_x=mat.cols/(float)ui->PICTURE->width();
        float ratio_y=mat.rows/(float)ui->PICTURE->height();
        x1=ratio_x*(event->pos().x()-ui->PICTURE->pos().x());
        y1=ratio_y*(event->pos().y()-ui->PICTURE->pos().y());
        m_rect.x=x1;
        m_rect.y=y1;
    }

}
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons()==Qt::LeftButton)
    {
        float ratio_x=mat.cols/(float)ui->PICTURE->width();
        float ratio_y=mat.rows/(float)ui->PICTURE->height();
        x2=ratio_x*(event->pos().x()-ui->PICTURE->pos().x());
        y2=ratio_y*(event->pos().y()-ui->PICTURE->pos().y());

        m_rect.width=x2-m_rect.x;
        m_rect.height=y2-m_rect.y;
        mat=imread("/var/tmp/image.jpg");
        cv::rectangle(mat,m_rect,cv::Scalar(0,0,255),3,8,0);
        myupdate3();
    }
}

QImage Mat2Qimage(cv::Mat cvImg)
{
    QImage qImg;
    if(cvImg.channels()==3)
        {
            cv::cvtColor(cvImg,cvImg,CV_BGR2RGB);//转为RGB格式
            qImg = QImage((const uchar*)cvImg.data,
                          cvImg.cols,cvImg.rows,
                          cvImg.cols*cvImg.channels(), //bytesPerLine,即每行的数据量,此处用处是使数据对齐
                          QImage::Format_RGB888); //简单地转换一下为QImage对象
        }
        else
        {
            qImg = QImage((const uchar*)cvImg.data,
                          cvImg.cols,cvImg.rows,
                          cvImg.cols*cvImg.channels(),
                          QImage::Format_Indexed8);
        }

    return qImg;
}

void MainWindow::str_split(QString qstr)
{
    QString qstr1,qstr2;
    QStringList s1=qstr.split(",");
    for(int i=0;i<s1.count();i++)
    {
        if(i!=0&&i!=1&&i!=(s1.count()-1))
        {
            QStringList s2=s1.at(i).split("{");
            for(int j=0;j<s2.count();j++)
            {
                if(j!=0)
                {
                    QStringList s3=s2.at(j).split("}");
                    for(int k=0;k<s3.count();k++)
                    {
                        if(k==0)
                        {
                            qstr1=s3.at(k);
                            qstr2=qstr1.mid(8);
                            ui->textEdit_2->setTextColor(Qt::red);
                            ui->textEdit_2->setFontPointSize(15);
                            ui->textEdit_2->append(qstr2);
                        }

                    }

                }

            }

        }
    }
}
void MainWindow::myupdate3()
{
    QImage img=Mat2Qimage(mat);

    label_show(img);

}
void MainWindow::print_s()
{

}

void MainWindow::label_show(QImage image)
{
    QImage image1=image.scaled(ui->PICTURE->size(),Qt::IgnoreAspectRatio);

    ui->PICTURE->setPixmap(QPixmap::fromImage(image1));
}
void MainWindow::baidu_ocr()
{
    Json::Value result;
    std::string img_str;

    double runtime=cv::getTickCount();
    aip::get_file_content("/var/tmp/image.jpg",&img_str);

    // 调用通用文字识别（高精度版）
    result = client.accurate_basic(img_str, aip::null);

    Json::FastWriter writer;
    string str = writer.write(result);
    runtime=(cv::getTickCount()-runtime)/cv::getTickFrequency();
    QString r=QString::number(runtime);
    ui->textEdit_4->setPlainText(r);

    QString qstr = QString::fromStdString(str);

    str_split(qstr);
    ui->textEdit->append(qstr);
}
void MainWindow::myupdate()
{
    Mat frame;
    QImage image;
    cap.read(frame);
    image=Mat2Qimage(frame);

    label_show(image);

    imwrite("/var/tmp/image.jpg", frame);
}

void MainWindow::myupdate2()
{
    Mat frame;
    cap.read(frame);

    QImage image;
    image=Mat2Qimage(frame);
    imwrite("/var/tmp/image.jpg", frame);
    label_show(image);

    baidu_ocr();

}

void MainWindow::on_opencamera_clicked()
{
     m_pTimer->start();

}

void MainWindow::on_openfile_clicked()
{
        QString path=QFileDialog::getOpenFileName(this,tr("选择目标图片"),".",tr("ImageFiles(*.jpg *.png)"));
        QImage* img=new QImage;
        QImage* scaledimg=new QImage;

        double runtime=cv::getTickCount();

        if(!(img->load(path)))
        {
            QMessageBox::information(this,tr("打开图像失败"),tr("打开图像失败!"));
            delete img;
        }
        QSize lasize=ui->PICTURE->size();

        * scaledimg=img->scaled(lasize,Qt::IgnoreAspectRatio);

        ui->PICTURE->setPixmap(QPixmap::fromImage(*scaledimg));

        *img=img->convertToFormat(QImage::Format_RGB888);
        Mat image = cv::Mat(img->height(), img->width(), CV_8UC3, (void*)img->bits(), img->bytesPerLine());
        cv::cvtColor(image, image, CV_BGR2RGB);
        runtime=(cv::getTickCount()-runtime)/cv::getTickFrequency();
        QString r=QString::number(runtime);
        ui->textEdit_3->setPlainText(r);
        image.copyTo(mat);

        imwrite("/var/tmp/image.jpg",image);
}

void MainWindow::on_pushButton_clicked()
{
    qtimer->start();
}

void MainWindow::on_stop_clicked()
{
    m_pTimer->stop();

    Mat image_1=imread("../baidu_api_2/test.jpg");

    QImage qimage=Mat2Qimage(image_1);

    label_show(qimage);

    baidu_ocr();
}


void MainWindow::on_close_clicked()
{
    qtimer->stop();
}

void MainWindow::on_logo_clicked()
{
    QString path=QFileDialog::getOpenFileName(this,tr("选择logo"),".",tr("ImageFiles(*.jpg *.png)"));
    QImage* img=new QImage;
    QImage* scaledimg=new QImage;
    if(!(img->load(path)))
    {
        QMessageBox::information(this,tr("打开图像失败"),tr("打开图像失败!"));
        delete img;
    }
    QSize lasize_3=ui->label_3->size();
    * scaledimg=img->scaled(lasize_3,Qt::IgnoreAspectRatio);
    /*int Owidth=img->width(),Oheight=img->height();
    int Fwidth,Fheight;       //缩放后的图片大小
    ui->label_3->setGeometry(50,600,100,100);
    int Mul;            //记录图片与label大小的比例，用于缩放图片
    if(Owidth/400>=Oheight/300)
        Mul=Owidth/400;
    else
        Mul=Oheight/300;
    Fwidth=Owidth/Mul;
    Fheight=Oheight/Mul;
    *scaledimg=img->scaled(Fwidth,Fheight,Qt::KeepAspectRatio);*/

    ui->label_3->setPixmap(QPixmap::fromImage(*scaledimg));
}

void MainWindow::on_pushButton_2_clicked()
{
    Mat image=imread("/var/tmp/image.jpg");

    if(ui->comboBox->currentIndex()==0)
    {
        if(ui->comboBox_2->currentIndex()==1)
        {
            Mat dst=image(Rect(x1,y1,x2-x1,y2-y1));
            imwrite("/var/tmp/image.jpg",dst);
        }
        else if(ui->comboBox_2->currentIndex()==0)
        {
        }
         baidu_ocr();
    }
    else if(ui->comboBox->currentIndex()==1)
    {
        ELM_IN_ELM_Model model;
        model.load("/home/manjasaka/Desktop/opencv/baidu_api_2/model");
        string chars,charss;
        if(ui->comboBox_2->currentIndex()==1)
        {
            Mat image2,mask;

            image2=image(Rect(x1,y1,x2-x1,y2-y1));//提取感兴趣区域
            imwrite("/var/tmp/image.jpg",image2);

            Mat image2_copy;
            image2.copyTo(image2_copy);

            Mat image3=c.det(image2);

            //使detect区域和原图像区域叠加在一起
            cvtColor(image3,mask,CV_BGR2GRAY);
            image3.copyTo(image2,mask);

            QImage img=Mat2Qimage(image);
            label_show(img);

            vector<Mat> charcters=d.chara_split(image2_copy);
            //cout<<"yes"<<endl;
            double runtime=cv::getTickCount();
            for (int i=0;i<charcters.size();i++)
            {

                model.query(charcters[i],chars);
                charss.append(chars);
            }
            QString qstr = QString::fromStdString(charss);
            ui->textEdit->append(qstr);
            runtime =(cv::getTickCount()-runtime)/cv::getTickFrequency();
            QString qstr2 = QString::number(runtime);
            ui->textEdit_5->setPlainText(qstr2);
        }
        else if(ui->comboBox_2->currentIndex()==0)
        {
            image=c.det(image);
            QImage img=Mat2Qimage(image);
            label_show(img);
            vector<Mat> charcters=d.chara_split(image);
            for (int i=0;i<charcters.size();i++)
            {
                model.query(charcters[i],chars);
                QString qstr = QString::fromStdString(chars);
                ui->textEdit->append(qstr);
            }
        }
    }

}
