#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "Processing.h"

using namespace std;

extern uint8_t grayImageData[IMG_WIDTH*IMG_HEIGHT];     // 原始灰度图像
extern uint8_t binaryImageData[IMG_WIDTH*IMG_HEIGHT];   // 二值化后图像
extern  uint8_t  DetectedRingsNum;

extern  snake_detectedRing detectedRings[4];


void TimeCount(double finish, double start){

    double duration = 1000*(double)(finish - start)/ CLOCKS_PER_SEC; //输出单位ms

    std::cout<<"running time:"<< duration <<"ms"<<std::endl;  //the totol time cost

}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QString pathStr="CodePics/self.png";
    QImage qimage;
    bool imgLoaded = qimage.load(pathStr);

    if(imgLoaded == false)
    {
        qDebug() << "error loading image";
    }


    // 灰度化操作
    QImage convertedImage = qimage.convertToFormat(QImage::Format_Grayscale8,Qt::MonoOnly);

    int width = convertedImage.width();
    int height= convertedImage.height();


    uint8_t * data = convertedImage.bits();

    FuncDataInit(); // 全局变量初始化
    memcpy(grayImageData,data,sizeof(uint8_t)*IMG_WIDTH*IMG_HEIGHT); // 将采集到的图像数据拷贝进入数据池



    //执行
    clock_t start, finish; //timer

    start = clock();

    FuncProcessing(); // 开始二维码定位和解码操作

    finish = clock();

    TimeCount(finish,start);


    std::cout<< "detected Ring Number: " <<(int)DetectedRingsNum<<std::endl;

    QImage binaryImage(binaryImageData,width,height,QImage::Format_Grayscale8);


    ui->label1->setPixmap(QPixmap::fromImage(qimage));
    ui->label1->setScaledContents(true);
    ui->label2->setPixmap(QPixmap::fromImage(convertedImage));
    ui->label2->setScaledContents(true);
    ui->label3->setPixmap(QPixmap::fromImage(binaryImage));
    ui->label3->setScaledContents(true);

}



MainWindow::~MainWindow()
{
    delete ui;
}
