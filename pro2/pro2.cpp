#include <iostream>
#include"Buffer.h"

//生成测试文件
void creat(string filename, int num)
{
    // 打开二进制文件进行写入
    fstream file(filename, std::ios::out | std::ios::binary);

    srand(static_cast<int>(time(0)));

    // 逐个元素写入文件
    for (int i = 0; i < num; i++) {
        int num = rand() % 1000;
        file.write(reinterpret_cast<char*>(&num), sizeof(int));
    }
    file.close();
}

//打印
void PrintBinaryFile(string filename)
{
    ifstream file(filename, ios::binary);
    if (!file)
    {
        cerr << "Failed to open the file: " << filename << std::endl;
        return;
    }

    int value;
    while (file.read(reinterpret_cast<char*>(&value), sizeof(int)))
        std::cout << value << " ";   
    cout << endl;
    file.close();
}


int main()
{
    creat("test.bin", 41);
    PrintBinaryFile("test.bin");
    cout << endl;

    Buffer a("test.bin", 3);
    a.Generation();
    cout << "初始归并段: " << endl;
    PrintBinaryFile("test.bin");

    cout << endl;
    a.Merge();
}

