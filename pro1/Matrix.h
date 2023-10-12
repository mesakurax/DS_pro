#pragma once
#include<iostream>
#include<string>
#include<fstream>

using namespace std;

const int Block_Size = 1;

class Matrix
{
public:
	Matrix(int prows, int pcolumns, string pfile) :rows(prows), columns(pcolumns), filename(pfile)
	{
		start = 0;
		dirty = 0;
		misses = 0;
		name = filename.substr(0, 1);

		refreshCache(0);
		misses++;
	}

	void compute(int x, int y, int value);   //���¾���Ԫ��

	int getElement(int x, int y);  //��ȡ����Ԫ��

	void refreshCache(int position);   //ȱʧʱˢ��Cache

	bool InCache(int position);  //�ж��Ƿ�����Cache

	void Dirty();  //�޸�dirtyλ

	void printCache();  //��ӡcache


private:
	int cache[Block_Size];   //����Cache����СΪBlock_Size
	int rows;  //��������
	int columns;  //��������
	string filename;  //�洢������ļ���
	int start;  //�����е�һ��Ԫ�ص��±�
	int misses; //����ȱʧ����
	int dirty;  //��λ��0Ϊδ�ģ�1Ϊ��
	string name;   
};

