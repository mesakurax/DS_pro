#pragma once

#include <iostream>
#include <vector>

using namespace std;

class LoserTree
{
public:

	LoserTree(const vector<int>& data) //���캯��
	{
		size = data.size();
		tree.resize(size);
		leaves.resize(size);

		for (int i = 0; i < size; i++)
		{
			leaves[i] = data[i];
			tree[i] = -1;
		}
		for (int i = 0; i < size; i++)  //��ʼ��������
			adjust(i);

	}

	int getWinner() { return leaves[tree[0]]; }

	int getWinnerindex() { return tree[0]; }

	int insert(int element)
	{
		int a = getWinner();
		leaves[tree[0]] = element;
		adjust(tree[0]);

		return a;
	}

private:
	int size;  //Ҷ�ӽڵ���Ŀ
	vector<int> leaves;   //Ҷ��
	vector<int> tree;   //������

	//��Ҷ�ӽڵ���Ϊ��ʼʤ��
	void adjust(int index)
	{

		int parent = (index + size) / 2;  //Ҷ�ӽڵ�ĸ���
		while (parent > 0)
		{
			if (tree[parent] == -1)
			{
				swap(index, tree[parent]);
				break;
			}

			if (leaves[index] > leaves[tree[parent]])  //�����ʼʤ�ߴ���ʧ����
				swap(index, tree[parent]); //ʤ�ߺͰ��߽��н���

			parent /= 2;  //��������
		}

		tree[0] = index;    //������ʤ�ߴ洢�ڸ��ڵ�
	}
};


