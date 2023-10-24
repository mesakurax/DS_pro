#pragma once
#include<iostream>
#include<string>
#include<fstream>

using namespace std;

const int Block_Size = 4;

class Matrix
{
public:
	Matrix(int prows, int pcolumns, string pfile)
	{
		rows = prows;
		columns = pcolumns;
		start = 0;
		dirty = 0;
		filename=pfile;
		misses = 0;
		name = filename.substr(0, 1);

		//首次缺失刷新
		refreshCache(0);
		misses++;
	}

	void compute(int x, int y, int value);   //更新矩阵元素

	int getElement(int x, int y);  //获取矩阵元素

	void refreshCache(int position);   //缺失时刷新Cache

	bool InCache(int position);  //判断是否命中Cache

	void printCache();  //打印cache

	void writeback();  //cache写回文件


private:
	int cache[Block_Size];   //缓存Cache，大小为Block_Size
	int rows;  //矩阵行数
	int columns;  //矩阵列数
	string filename;  //存储矩阵的文件名
	int start;  //缓存中第一个元素的下标
	int misses; //命中缺失次数
	int dirty;  //脏位，0为未改，1为改
	string name;   
	int num = 0;  //有效元素数量
};

