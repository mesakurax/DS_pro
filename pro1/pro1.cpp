#include"Matrix.h"
#include<time.h>
using namespace std;

//创建矩阵文件
void creatMatrix(int rows, int columns, string filename)
{
    // 打开二进制文件进行写入
    std::ofstream file(filename, std::ios::binary);

    int num = 0;
    srand(static_cast<int>(time(0)));

    // 逐个元素写入文件
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            num = rand() % 11;
            file.write(reinterpret_cast<char*>(&num), sizeof(int));
        }
    }

    // 关闭文件
    file.close();
}

//从文件打印矩阵
void printMatrix(int rows, int columns, string filename)
{
    // 打开二进制文件进行读取
    std::ifstream file(filename, std::ios::binary);

    if (!file)
    {
        std::cerr << "无法打开文件 " << filename << std::endl;
        return;
    }

    int num = 0;

    // 逐个元素读取文件
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            file.read(reinterpret_cast<char*>(&num), sizeof(int));
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

    // 关闭文件
    file.close();
}

//矩阵
int main()
{
    creatMatrix(100, 100, "A.bin");
    creatMatrix(100, 100, "B.bin");
}


