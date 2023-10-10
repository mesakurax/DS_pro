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
		misses = 1;   //��һ��ȱʧ

		cout << "��ʼ�����" << endl;
	}



	int getElement(int x, int y);  //��Cache��ȡԪ��

	void refreshCache(int position);   //ȱʧʱˢ��Cache

	bool InCache(int x, int y);  //�ж��Ƿ�����Cache

	void Dirty();  //�޸�dirtyλ


private:
	int cache[Block_Size];   //����Cache����СΪBlock_Size
	int rows;  //��������
	int columns;  //��������
	string filename;  //�洢������ļ���
	int start;  //�����е�һ��Ԫ�����ļ��е���ʼλ��
	int misses; //����ȱʧ����
	int dirty;  //��λ��0Ϊδ�ģ�1Ϊ��
};

