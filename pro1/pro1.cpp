#include"Matrix.h"
#include<time.h>
using namespace std;

//矩阵长和宽
const int A_rows = 10;
const int A_columns = 3;
const int B_rows = 3;
const int B_columns = 10;

//创建矩阵文件（元素为0，1，2）
void creatMatrix(int rows, int columns, string filename,bool result)
{
    // 打开二进制文件进行写入
    std::ofstream file(filename, std::ios::out|std::ios::binary);

    int num = 0;
    srand(static_cast<int>(time(0)));

    // 逐个元素写入文件
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
         
            num = result?(rand() % 3):0;
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
    std::ifstream file(filename, std::ios::in|std::ios::binary);

    if (!file)
    {
        std::cerr << "无法打开文件 " << filename << std::endl;
        return;
    }

    

    // 逐个元素读取文件
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            int num;
            file.read(reinterpret_cast<char*>(&num), sizeof(int));
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
    cout << endl;
    // 关闭文件
    file.close();
}

//矩阵乘法
void ikj(Matrix A,Matrix B,Matrix Z)
{
    for (int i = 0; i < A_rows; i++)
        for (int k = 0; k < A_columns; k++)
            for (int j = 0; j < B_columns; j++)
                Z.compute(i,j,A.getElement(i, k) * B.getElement(k,j));

    Z.writeback();
    printMatrix(A_rows, B_columns, "Z.bin");
}

int main()
{
    //生成测试矩阵
    creatMatrix(A_rows, A_columns, "A.bin",true);
    creatMatrix(B_rows, B_columns, "B.bin",true);
    creatMatrix(A_rows, B_columns, "Z.bin", false);
    printMatrix(A_rows, A_columns, "A.bin");
    printMatrix(B_rows, B_columns, "B.bin");
    printMatrix(A_rows, B_columns, "Z.bin");

   
    Matrix A(A_rows, A_columns, "A.bin");
    Matrix B(B_rows, B_columns, "B.bin");
    Matrix Z(A_rows, B_columns, "Z.bin"); 
    
    //计算结果
    ikj(A, B, Z);
}


