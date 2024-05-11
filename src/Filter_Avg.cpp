#include <opencv2/opencv.hpp>
using namespace cv;

int main(int argc, char *argv[])
{
    Mat dst, img = imread("/home/ros/code_/code/test_socket/src/scans.pgm", IMREAD_COLOR);
    imshow("origin", img);

    // 方框滤波：区域内的灰度值求平均赋值给锚点
    boxFilter(img, dst, img.depth(), Size(3, 3), Point2d(-1, -1), false);
    imshow("box", dst);

    // 均值滤波：区域内的灰度值求平均并 归一化(平均值 / 区域的像素个数) 赋值给锚点
    blur(img, dst, Size(3, 3)); //==boxFilter(img, dst, img.depth(), Size(3, 3), Point2d(-1, -1), true);
    imshow("blur", dst);

    // 中值滤波：区域内的灰度值求中值赋值给锚点
    medianBlur(img, dst, 3);
    imshow("median", dst);

    // 返回内核矩阵
    Mat kernal = getStructuringElement(MORPH_RECT, Size(3, 3));
    erode(img, dst, kernal);
    imshow("erode", dst);

    // 形态学运算：
    // 闭运算(先膨胀，后腐蚀，可以去掉小的暗点)
    // 开运算(先腐蚀，后膨胀，可以去掉小的亮点)
    // 顶帽(原图-开运算之后的图，结果为被腐蚀掉的亮点)
    // 黑帽(闭运算之后的图-原图，结果为被腐蚀掉的暗点)
    // 膨胀(用区域中的最大灰度值代替锚点值)
    // 腐蚀(用区域中的最小灰度值代替锚点值)
    morphologyEx(img, dst, MORPH_CLOSE, kernal);
    imshow("close", dst);

    waitKey(0);

    return 0;
}
