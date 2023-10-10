#pragma once
#include<iostream>
#include<string>
#include<fstream>

using namespace std;

const int Block_Size = 10;

class Matrix
{
public:
	Matrix(int prows, int pcolumns, string pfile) :rows(rows), columns(pcolumns), filename(pfile)
	{
		start = 0;
		misses = 1;   //第一次缺失

		cout << "初始化完成" << endl;
	}



	int getElement(int x, int y);  //从Cache获取元素

	void refreshCache(int position);   //缺失时刷新Cache

	bool InCache(int x, int y);  //判断是否命中Cache

	void Dirty();  //修改dirty位


private:
	int cache[Block_Size];   //缓存Cache，大小为Block_Size
	int rows;  //矩阵行数
	int columns;  //矩阵列数
	string filename;  //存储矩阵的文件名
	int start;  //缓存中第一个元素在文件中的起始位置
	int misses; //命中缺失次数
	int dirty;  //脏位，0为未改，1为改
};

