#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

void write()
{
    FileStorage fs;
    bool ret = fs.open("../config/write.yaml", FileStorage::WRITE);
    if (!ret)
    {
        std::cout << "open file faild" << std::endl;
        return;
    }

    Mat m = (Mat_<int>(3, 3) << 1000, 0, 320, 0, 1000, 240, 0, 0, 1);
    fs << "m" << m;
    fs.release();
}

void read()
{
    FileStorage fs;
    bool ret = fs.open("../config/read.yaml", FileStorage::READ);
    if (!ret)
    {
        std::cout << "open file faild" << std::endl;
        return;
    }

    std::cout << "open file success" << std::endl;

    int a, b, c, d;

    a = fs["a"];
    d = fs["c"]["d"];

    std::cout << "a = " << a << std::endl;
    std::cout << "d = " << d << std::endl;

    fs.release();
}

int main(int argc, char *argv[])
{
    read();
    write();
    return 0;
}
