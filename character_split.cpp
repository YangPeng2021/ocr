#include "character_split.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

vector<Mat> horizontalProjectionMat(Mat srcImg)//水平投影
{
    Mat binImg;
    blur(srcImg, binImg, Size(3, 3));
    threshold(binImg, binImg, 0, 255, THRESH_OTSU);

    int white=0;
    int black=0;
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            if(binImg.at<uchar>(i,j)==0)
                black++;
            else white++;
        }
        for(int j=binImg.cols-1;j>(binImg.cols-11);j--)
            if(binImg.at<uchar>(i,j)==0)
                black++;
            else white++;
    }
    for(int i=binImg.rows-11;i<(binImg.rows-1);i++)
    {
        for(int j=0;j<10;j++)
        {
            if(binImg.at<uchar>(i,j)==0)
                black++;
            else white++;
        }
        for(int j=binImg.cols-1;j>(binImg.cols-11);j--)
            if(binImg.at<uchar>(i,j)==0)
                black++;
            else white++;
    }
    if(black>white) binImg=255-binImg;

    int perPixelValue = 0;//每个像素的值
    int width = srcImg.cols;
    int height = srcImg.rows;
    int* projectValArry = new int[height];//创建一个储存每行白色像素个数的数组
    memset(projectValArry, 0, height * 4);//初始化数组
    for (int col = 0; col < height; col++)//遍历每个像素点
    {
        for (int row = 0; row < width; row++)
        {
            perPixelValue = binImg.at<uchar>(col, row);
            if (perPixelValue == 0)//如果是白底黑字
            {
                projectValArry[col]++;
            }
        }
    }
    Mat horizontalProjectionMat(height, width, CV_8UC1);//创建画布
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            perPixelValue = 255;
            horizontalProjectionMat.at<uchar>(i, j) = perPixelValue;//设置背景为白色
        }
    }
    for (int i = 0; i < height; i++)//水平直方图
    {
        for (int j = 0; j < projectValArry[i]; j++)
        {
            perPixelValue = 0;
            horizontalProjectionMat.at<uchar>(i, width - 1 - j) = perPixelValue;//设置直方图为黑色
        }
    }
    //imshow("水平投影",horizontalProjectionMat);
    vector<Mat> roiList;//用于储存分割出来的每个字符
    int startIndex = 0;//记录进入字符区的索引
    int endIndex = 0;//记录进入空白区域的索引
    bool inBlock = false;//是否遍历到了字符区内
    for (int i = 0; i <srcImg.rows; i++)
    {
        if (!inBlock && projectValArry[i] != 0)//进入字符区
        {
            inBlock = true;
            startIndex = i;
        }
        else if (inBlock && projectValArry[i] == 0)//进入空白区
        {
            endIndex = i;
            inBlock = false;
            Mat roiImg = binImg(Range(startIndex, endIndex + 1), Range(0, srcImg.cols));//从原图中截取有图像的区域
            roiList.push_back(roiImg);
        }
    }
    delete[] projectValArry;
    return roiList;
}
vector<Mat> verticalProjectionMat(Mat srcImg)//垂直投影
{
    Mat binImg;
    blur(srcImg, binImg, Size(3, 3));
    threshold(binImg, binImg, 0, 255, THRESH_OTSU);
    //imshow("thre",binImg);


    /*Mat roi1=binImg(Rect(0,0,5,5));
    cout<< "1"<<endl;
    cout<<binImg.rows<<" " <<binImg.cols<<endl;
    Mat roi2=binImg(Range(0,5),Range(binImg.cols-5,binImg.cols));
    cout<<"2"<<endl;
    Mat roi3=binImg(Range(binImg.rows-5,binImg.rows),Range(0,5));
    cout<<"3"<<endl;
    Mat roi4=binImg(Range(binImg.rows-5,binImg.rows),Range(binImg.cols-5,binImg.cols));
    cout<<"4"<<endl;
    white=countNonZero(roi1)+countNonZero(roi2)+countNonZero(roi3)+countNonZero(roi4);
    cout << white << endl;
    if(white<50) binImg=255-binImg;*/

    int perPixelValue;//每个像素的值
    int width = srcImg.cols;
    int height = srcImg.rows;
    int* projectValArry = new int[width];//创建用于储存每列白色像素个数的数组
    memset(projectValArry, 0, width * 4);//初始化数组
    for (int col = 0; col < width; col++)
    {
        for (int row = 0; row < height;row++)
        {
            perPixelValue = binImg.at<uchar>(row, col);
            if (perPixelValue == 0)//如果是白底黑字
            {
                projectValArry[col]++;
            }
        }
    }
    Mat verticalProjectionMat(height, width, CV_8UC1);//垂直投影的画布
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            perPixelValue = 255;  //背景设置为白色
            verticalProjectionMat.at<uchar>(i, j) = perPixelValue;
        }
    }
    for (int i = 0; i < width; i++)//垂直投影直方图
    {
        for (int j = 0; j < projectValArry[i]; j++)
        {
            perPixelValue = 0;  //直方图设置为黑色
            verticalProjectionMat.at<uchar>(height - 1 - j, i) = perPixelValue;
        }
    }
    //imshow("垂直投影",verticalProjectionMat);
    waitKey(0);
    vector<Mat> roiList;//用于储存分割出来的每个字符
    int startIndex = 0;//记录进入字符区的索引
    int endIndex = 0;//记录进入空白区域的索引
    bool inBlock = false;//是否遍历到了字符区内
    for (int i = 0; i < srcImg.cols; i++)//cols=width
    {
        if (!inBlock && projectValArry[i] != 0)//进入字符区
        {
            inBlock = true;
            startIndex = i;
        }
        else if (projectValArry[i] == 0 && inBlock)//进入空白区
        {
            endIndex = i;
            inBlock = false;
            Mat roiImg = srcImg(Range(0, srcImg.rows), Range(startIndex, endIndex + 1));
            roiList.push_back(roiImg);
        }
    }
    delete[] projectValArry;
    return roiList;
}
character_split::character_split()
{

}

vector<Mat> character_split::chara_split(cv::Mat srcImg)
{
    vector<Mat> characters;
    cvtColor(srcImg,srcImg,COLOR_BGR2GRAY);
    char szName[30] = { 0 };
    vector<Mat> b = horizontalProjectionMat(srcImg);//先进行水平投影
    int c=0;
    //cout<<b.size()<<endl;
    for (int i = 0; i < b.size(); i++)
    {
        //cout<<"i="<<i<<endl;
        vector<Mat> a = verticalProjectionMat(b[i]);//垂直投影
        //cout<<a.size()<<endl;
        for (int j = 0; j < a.size(); j++)
        {
            if(a[j].rows<=a[j].cols)
                continue;
            sprintf(szName,"%d.jpg",++c);
            //cout<<"j="<<j<<endl;
            //imshow(szName,a[j]);
            Mat img = Mat(a[j]);
            imwrite(szName, img);//保存切分的结果
            Mat mat2(img.rows,img.rows,CV_8U);
            mat2=Scalar(255);
            Mat roi=mat2(Range(0,img.rows),Range((img.rows-img.cols)/2,(img.rows-img.cols)/2+img.cols));//range是前闭后开，不用减一
            img.copyTo(roi);
            resize(mat2,mat2,Size(16,16));

            characters.push_back(mat2);
        }
    }

    waitKey(0);
    return characters;
}
