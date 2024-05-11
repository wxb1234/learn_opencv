#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <string.h>

using namespace cv;
using namespace std;

void test_origin_2_left()
{

    // 假设屏幕的宽度和高度为 1920x1080
    int screen_width = 1920;
    int screen_height = 1080;

    // 坐标点 (-1, -1) 在以屏幕中心点为原点的坐标系中
    float x_center_origin = -1.0;
    float y_center_origin = -1.0;

    // 将坐标转换为以屏幕左上角为原点的坐标系中
    float x_top_left_origin = x_center_origin + screen_width / 2.0;
    float y_top_left_origin = y_center_origin + screen_height / 2.0;

    // 输出转换后的坐标
    cout << "Transformed point: (" << x_top_left_origin << ", " << y_top_left_origin << ")" << endl;
}

void add_text(Mat &img)
{
    float half_width = img.cols / 2.0;
    float half_height = img.rows / 2.0;

    int mul = 15;

    float x1 = -23.2827 * mul + half_width;
    float y1 = 1.33788 * mul + half_height;

    float x2 = -12.6288 * mul + half_width;
    float y2 = -24.8073 * mul + half_height;

    float x3 = -6.22942 * mul + half_width;
    float y3 = -42.0642 * mul + half_height;

    float x4 = -0.0492583 * mul + half_width;
    float y4 = -59.0627 * mul + half_height;

    float x5 = -4.55429 * mul + half_width;
    float y5 = -58.9276 * mul + half_height;

    float x6 = -8.73283 * mul + half_width;
    float y6 = -61.3013 * mul + half_height;

    float x7 = -1.13874 * mul + half_width;
    float y7 = -64.8416 * mul + half_height;

    float x8 = -4.74402 * mul + half_width;
    float y8 = -69.67 * mul + half_height;

    float x9 = 0.0057202 * mul + half_width;
    float y9 = -69.1492 * mul + half_height;

    vector<Point2f> points;

    points.push_back(Point2f(x1, y1));
    points.push_back(Point2f(x2, y2));
    points.push_back(Point2f(x3, y3));
    points.push_back(Point2f(x4, y4));
    points.push_back(Point2f(x5, y5));
    points.push_back(Point2f(x6, y6));
    points.push_back(Point2f(x7, y7));
    points.push_back(Point2f(x8, y8));
    points.push_back(Point2f(x9, y9));

    cout << "points = " << points << endl;

    vector<Point2f>::iterator begin = points.begin();
    vector<Point2f>::iterator end = points.end();

    int i = 0;
    for (; begin != end; begin++)
    {
        char room[20] = "room";
        char tmp[5];
        sprintf(tmp, "%d", i);
        i++;
        strcat(room, tmp);
        putText(img, room, Point2f((*begin).x, (*begin).y), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0, 0, 255), 1);
    }
}

void pgm_to_png(const char *path)
{
    Mat img = imread(path, IMREAD_COLOR);

    cout << "depth:" << img.depth() << endl;
    

    if (img.empty())
    {
        cout << "img is empty error" << endl;
        return;
    }

    // 获取图像的宽度和高度
    int height = img.rows;
    int width = img.cols;

    cout << "img height = " << height << endl;
    cout << "img width = " << width << endl;

    Mat black = imread("/home/ros/code_/code/test_socket/src/2.png", IMREAD_COLOR); // 327 471

    // 对原图感兴趣的区域,想要操作的区域
    Mat roi = img(Rect(0, 0, 100, 100));

    // 把mask拷贝到感兴趣的区域中
    Mat target = roi.clone();
    // 设置颜色
    target.setTo(Scalar(0, 0, 125));

    // 掩膜必须是灰度图
    Mat mask_g;
    // 转换为灰度图颜色空间
    cvtColor(target, mask_g, CV_BGR2GRAY);

    // 将目标图像拷贝到感兴趣的图片区域中
    target.copyTo(roi, mask_g);

    cout << "=================" << endl;

    int channels = black.channels();

    double start = (double)static_cast<int>(getTickCount());
    int count = 0;

    // 遍历black图
    for (int k = 0; k < black.rows; k++)
    {
        uchar *data = black.ptr<uchar>(k);
        for (int i = 0; i < black.cols; i++)
        {
            for (int j = 0; j < channels; j++)
            {
                // printf("data[%d] = %d\n", j, data[j]);
                count++;
            }
        }
    }

    // 遍历耗时
    double end = (double)static_cast<int>(getTickCount());
    double time = (end - start) / getTickFrequency();
    printf("time = %f\n", time);
    printf("count = %d\n", count);

    printf("========black attributes========\n");
    printf("black width = %d\n", black.cols);
    printf("black height = %d\n", black.rows);
    printf("channel = %d\n", channels);
    cout << "black rows = " << black.rows << " cols = " << black.cols << endl;

    // 添加房间编号
    add_text(img);

    namedWindow("test", WindowFlags::WINDOW_NORMAL);
    imshow("test", img);
    waitKey(0);

    // 写入磁盘
    bool success = imwrite("/home/ros/code_/map.png", img);

    if (!success)
    {
        cout << "!success" << endl;
        return;
    }
}

int main(int argc, char *argv[])
{
    pgm_to_png("/home/ros/code_/code/test_socket/src/scans.pgm");
    return 0;
}
