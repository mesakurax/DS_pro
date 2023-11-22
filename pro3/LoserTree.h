#pragma once

#include <iostream>
#include <vector>

using namespace std;

class LoserTree
{
public:

	LoserTree(const vector<int>& data) //构造函数
	{
		size = data.size();
		tree.resize(size);
		leaves.resize(size);

		for (int i = 0; i < size; i++)
		{
			leaves[i] = data[i];
			tree[i] = -1;
		}
		for (int i = 0; i < size; i++)  //初始化败者树
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
	int size;  //叶子节点数目
	vector<int> leaves;   //叶子
	vector<int> tree;   //败者树

	//把叶子节点作为初始胜者
	void adjust(int index)
	{

		int parent = (index + size) / 2;  //叶子节点的父亲
		while (parent > 0)
		{
			if (tree[parent] == -1)
			{
				swap(index, tree[parent]);
				break;
			}

			if (leaves[index] > leaves[tree[parent]])  //如果初始胜者大于失败者
				swap(index, tree[parent]); //胜者和败者进行交换

			parent /= 2;  //继续向上
		}

		tree[0] = index;    //将最终胜者存储在根节点
	}
};


