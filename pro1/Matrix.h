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

		//�״�ȱʧˢ��
		refreshCache(0);
		misses++;
	}

	void compute(int x, int y, int value);   //���¾���Ԫ��

	int getElement(int x, int y);  //��ȡ����Ԫ��

	void refreshCache(int position);   //ȱʧʱˢ��Cache

	bool InCache(int position);  //�ж��Ƿ�����Cache

	void printCache();  //��ӡcache

	void writeback();  //cacheд���ļ�


private:
	int cache[Block_Size];   //����Cache����СΪBlock_Size
	int rows;  //��������
	int columns;  //��������
	string filename;  //�洢������ļ���
	int start;  //�����е�һ��Ԫ�ص��±�
	int misses; //����ȱʧ����
	int dirty;  //��λ��0Ϊδ�ģ�1Ϊ��
	string name;   
	int num = 0;  //��ЧԪ������
};

