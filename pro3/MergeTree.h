#pragma once
#include<fstream>
#include"LoserTree.h"
#include <queue>
#include<string>
#include<limits>
using namespace std;

const int I1_size = 5;    //输入缓冲区大小
const int O1_size = 20;   //输出缓冲区大小
const int I2_size = 10;    //输入缓冲区大小
const int O2_size = 40;   //输出缓冲区大小

const int k = 2; //k路归并

struct seg {
    int start;
    int end;
    int length;
    seg(int a, int b, int c)
    {
        start = a;
        end = b;
        length = c;
    }
};

struct comp {
    bool operator()(seg s1, seg s2) {
        return s1.length > s2.length;
    }
};

struct mer_s {
    int start;
    int end;

    int index;
    int num;
    mer_s(int a = -1, int b = -1, int c = 0, int d = 0)
    {
        start = a;
        end = b;
        index = c;
        num = d;
    }
};

class MergeTree
{
public:
    MergeTree(string filename, vector<seg> temp)
    {
        this->filename = filename;
        //归并段的导入
        for (auto s : temp)
            tree.push(s);

        cout << endl;
    }

    //k路归并树
    void MertSort()
    {
        int num = tree.size();
        int p_num = (num - 1) % (k - 1);

        ifstream file(filename, ios::binary | ios::in);

        file.seekg(0, ios::end);  // 将读取指针定位到文件末尾
        streampos fileSize = file.tellg();  // 获取文件大小

        int file_num = fileSize / sizeof(int);  //总元素数目
        file.close();

        //不满足k路归并添加虚段
        if ((p_num != 0))
        {
            for (int i = 0; i < k - 1 - p_num; i++)
                tree.push(seg(-1, -1, -1));
        }

        while (tree.size() != 1)  //只剩一个归并段退出
        {
            int i = 0;
            vector<seg> merge_seg;
            while (i < k && !tree.empty())  //选出k个归并完成后生成新的归并段返回
            {
                merge_seg.push_back(tree.top());
                tree.pop();
                i++;
            }

            tree.push(Merge(merge_seg));  //加入新的归并段
            cout << endl;
        }

        ifstream infile("test.bin", ios::in | std::ios::binary);

    }

    //归并
    seg Merge(vector<seg> merge_seg)
    {
        // 清空 temp.bin 文件
        std::ofstream tempFile("temp.bin", std::ios::binary | std::ios::trunc);
        tempFile.close();
        onum1 = 0;
        onum2 = 0;


        mer_s record[k];   //记录数组

        //初始化输入缓冲区
        for (int i = 0; i < merge_seg.size(); i++)
            record[i] = mer_s(merge_seg[i].start, merge_seg[i].end);


        //初始化败者树
        vector<int> temp;
        for (int i = 0; i < k; i++)
            temp.push_back(Insert(I1[i], record[i]));
        LoserTree losertree(temp);


        //当归并未完成，即树顶的元素不是int_max
        while (losertree.getWinner() != INT_MAX)
        {
            int index = losertree.getWinnerindex();
            int a = losertree.insert(Insert(I1[index], record[index]));
            OWrite(a);
        }
        cout << endl;

        //清空缓冲区
        if (onum1 != 0)
            writeO2();
        if (onum2 != 0)
            writeback();

        return writefile();
    }


    void IRead(int* a, mer_s& p)
    {
        fstream file(filename, ios::binary | ios::in);
        file.seekg(p.start * sizeof(int));

        // 计算应该读取的元素个数
        int elementsToRead = min(I1_size, p.end - p.start + 1);

        // 读取元素
        file.read(reinterpret_cast<char*>(a), elementsToRead * sizeof(int));

        // 计算实际读取的元素个数
        int bytesRead = file.gcount() / sizeof(int);

        // 更新num，确保其不超过实际读取的元素个数
        p.start = p.start + bytesRead;
        p.num = min(bytesRead, I1_size);
        p.index = 0;


        // 关闭文件
        file.close();
    }

    //读取缓冲区数据
    int Insert(int* a, mer_s& p)
    {
        if (p.start == -1 || ((p.index >= p.num) && (p.start > p.end)))  //归并段为空
            return INT_MAX;
        if (p.index >= p.num)  //缓冲区为空
            IRead(a, p);
        int temp = a[p.index];
        p.index++;

        // cout << p.start << " " << p.end << " " <<p.num << " " << p.index << endl;
        return temp;
    }

    void OWrite(int element)
    {
        out1[onum1] = element;
        onum1++;

        if (onum1 == O1_size)
        {
            writeO2();
            onum1 = 0;
        }
    }

    void writeO2()
    {
        for (int i = 0; i < onum1; i++)
        {
            out2[onum2] = out1[i];
            onum2++;

            if (onum2 == O2_size)
            {
                writeback();
                onum2 = 0;
            }
        }
    }

    void writeback()
    {
        fstream file("temp.bin", ios::binary | ios::out | ios::app);
        for (int i = 0; i < onum2; i++)
            file.write(reinterpret_cast<const char*>(&out2[i]), sizeof(int));
        file.close();
    }

    seg writefile()
    {
        ifstream infile("temp.bin", std::ios::binary | ios::in);

        ofstream file(filename, std::ios::binary | ios::out | ios::app);


        file.seekp(0, ios::end);  // 将读取指针定位到文件末尾
        streampos fileSize = file.tellp();  // 获取文件大小
        int a = fileSize / sizeof(int);  //总元素数目

        int value;
        while (infile.read(reinterpret_cast<char*>(&value), sizeof(int))) {
            cout << value << " ";
            file.write(reinterpret_cast<const char*>(&value), sizeof(int));
        }
        cout << endl << endl;
        file.seekp(0, ios::end);  // 将读取指针定位到文件末尾
        fileSize = file.tellp();  // 获取文件大小
        int b = fileSize / sizeof(int);  //总元素数目

        infile.close();
        file.close();

        return seg(a, b - 1, b - a + 1);
    }

private:
    priority_queue<seg, vector<seg>, comp> tree; //最佳归并树
    string filename;  //文件名

    int out1[O1_size];
    int out2[O2_size];
    int onum1;
    int onum2;   //输出缓冲区

    int I1[k][I1_size];
    int I2[I2_size];//输入缓冲区

};



