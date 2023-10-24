#pragma once
#include<string>
#include<vector>
#include<algorithm>
#include <iostream>
#include<fstream>
#include<cmath>

using namespace std;

const int I_size = 10;
const int O_size = 10;

class Buffer
{
public:
	Buffer(std::string filename, int length)
	{
		inum_1 = 0;
		inum_0 = 0;
		onum = 0;
		this->filename = filename;
		this->length = length;
	}

	void Generation();  //���ɳ���Ϊn�Ķ�������ʼ�鲢��

	void InsertSort(std::vector<int>& block, int length);  //��������

	void Merge();  //�鲢����
	
	void read0(int offset,int end); //���ļ����ȡ�鲢�ε�buffer input����
	void read1(int offset, int end); 
	void write(int value);  //д��Out,Out���˾�д����ʱ�ļ�
	void writeback();  //ȫ��д����ʱ�ļ�
	void writeInfile(int start); //��ʱ�ļ�д��Դ�ļ�

//private:
	int I0[I_size];  //input buffer�����
	int I1[I_size];  
	int Out[O_size];
	int inum_0;   //I0Ԫ����Ŀ
	int inum_1;
	int onum;  //output bufferԪ����Ŀ
	int length;  //�鲢�γ���
	std::string filename;
};