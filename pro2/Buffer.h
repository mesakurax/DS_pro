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

	void Generation();  //生成长度为n的多个有序初始归并段

	void InsertSort(std::vector<int>& block, int length);  //插入排序

	void Merge();  //归并排序
	
	void read0(int offset,int end); //从文件里读取归并段到buffer input里面
	void read1(int offset, int end); 
	void write(int value);  //写入Out,Out满了就写进临时文件
	void writeback();  //全部写进临时文件
	void writeInfile(int start); //临时文件写回源文件

//private:
	int I0[I_size];  //input buffer缓存块
	int I1[I_size];  
	int Out[O_size];
	int inum_0;   //I0元素数目
	int inum_1;
	int onum;  //output buffer元素数目
	int length;  //归并段长度
	std::string filename;
};