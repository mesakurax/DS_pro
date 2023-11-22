#pragma once
#include<fstream>
#include"LoserTree.h"
#include <queue>
#include<string>
#include<limits>
using namespace std;

const int I1_size = 5;    //���뻺������С
const int O1_size = 20;   //�����������С
const int I2_size = 10;    //���뻺������С
const int O2_size = 40;   //�����������С

const int k = 2; //k·�鲢

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
        //�鲢�εĵ���
        for (auto s : temp)
            tree.push(s);

        cout << endl;
    }

    //k·�鲢��
    void MertSort()
    {
        int num = tree.size();
        int p_num = (num - 1) % (k - 1);

        ifstream file(filename, ios::binary | ios::in);

        file.seekg(0, ios::end);  // ����ȡָ�붨λ���ļ�ĩβ
        streampos fileSize = file.tellg();  // ��ȡ�ļ���С

        int file_num = fileSize / sizeof(int);  //��Ԫ����Ŀ
        file.close();

        //������k·�鲢������
        if ((p_num != 0))
        {
            for (int i = 0; i < k - 1 - p_num; i++)
                tree.push(seg(-1, -1, -1));
        }

        while (tree.size() != 1)  //ֻʣһ���鲢���˳�
        {
            int i = 0;
            vector<seg> merge_seg;
            while (i < k && !tree.empty())  //ѡ��k���鲢��ɺ������µĹ鲢�η���
            {
                merge_seg.push_back(tree.top());
                tree.pop();
                i++;
            }

            tree.push(Merge(merge_seg));  //�����µĹ鲢��
            cout << endl;
        }

        ifstream infile("test.bin", ios::in | std::ios::binary);

    }

    //�鲢
    seg Merge(vector<seg> merge_seg)
    {
        // ��� temp.bin �ļ�
        std::ofstream tempFile("temp.bin", std::ios::binary | std::ios::trunc);
        tempFile.close();
        onum1 = 0;
        onum2 = 0;


        mer_s record[k];   //��¼����

        //��ʼ�����뻺����
        for (int i = 0; i < merge_seg.size(); i++)
            record[i] = mer_s(merge_seg[i].start, merge_seg[i].end);


        //��ʼ��������
        vector<int> temp;
        for (int i = 0; i < k; i++)
            temp.push_back(Insert(I1[i], record[i]));
        LoserTree losertree(temp);


        //���鲢δ��ɣ���������Ԫ�ز���int_max
        while (losertree.getWinner() != INT_MAX)
        {
            int index = losertree.getWinnerindex();
            int a = losertree.insert(Insert(I1[index], record[index]));
            OWrite(a);
        }
        cout << endl;

        //��ջ�����
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

        // ����Ӧ�ö�ȡ��Ԫ�ظ���
        int elementsToRead = min(I1_size, p.end - p.start + 1);

        // ��ȡԪ��
        file.read(reinterpret_cast<char*>(a), elementsToRead * sizeof(int));

        // ����ʵ�ʶ�ȡ��Ԫ�ظ���
        int bytesRead = file.gcount() / sizeof(int);

        // ����num��ȷ���䲻����ʵ�ʶ�ȡ��Ԫ�ظ���
        p.start = p.start + bytesRead;
        p.num = min(bytesRead, I1_size);
        p.index = 0;


        // �ر��ļ�
        file.close();
    }

    //��ȡ����������
    int Insert(int* a, mer_s& p)
    {
        if (p.start == -1 || ((p.index >= p.num) && (p.start > p.end)))  //�鲢��Ϊ��
            return INT_MAX;
        if (p.index >= p.num)  //������Ϊ��
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


        file.seekp(0, ios::end);  // ����ȡָ�붨λ���ļ�ĩβ
        streampos fileSize = file.tellp();  // ��ȡ�ļ���С
        int a = fileSize / sizeof(int);  //��Ԫ����Ŀ

        int value;
        while (infile.read(reinterpret_cast<char*>(&value), sizeof(int))) {
            cout << value << " ";
            file.write(reinterpret_cast<const char*>(&value), sizeof(int));
        }
        cout << endl << endl;
        file.seekp(0, ios::end);  // ����ȡָ�붨λ���ļ�ĩβ
        fileSize = file.tellp();  // ��ȡ�ļ���С
        int b = fileSize / sizeof(int);  //��Ԫ����Ŀ

        infile.close();
        file.close();

        return seg(a, b - 1, b - a + 1);
    }

private:
    priority_queue<seg, vector<seg>, comp> tree; //��ѹ鲢��
    string filename;  //�ļ���

    int out1[O1_size];
    int out2[O2_size];
    int onum1;
    int onum2;   //���������

    int I1[k][I1_size];
    int I2[I2_size];//���뻺����

};



