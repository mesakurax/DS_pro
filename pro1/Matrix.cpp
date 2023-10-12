#include "Matrix.h"

void Matrix::compute(int x, int y, int value)
{
	int position = x * rows + y;
	cout << "访问矩阵" << name << "下标为: (" << x << "," << y << ")" << endl;

	//缺失时刷新cache
	if (!InCache(position))
	{
		cout << "矩阵" << name << "缺失, 更新Cache" << endl;
		refreshCache(position);
		misses++;
	}
	int index = position - start;

	cout << "矩阵" << name << "[" << x << "," << y << "]=" << cache[index];
	cache[index] += value;  //更新元素
	cout << "--->" << cache[index] << endl << endl;
	dirty = 1;   //脏位设置为1
}

int Matrix::getElement(int x, int y)
{
	int position = x * rows + y;
	cout << "访问矩阵" << name << "下标为: (" << x << "," << y << ")" << endl;

	//缺失时刷新cache
	if (!InCache(position))
	{
		cout << "矩阵" << name << "缺失, 更新Cache" << endl;
		refreshCache(position);
		misses++;
	}
	int index = position - start;
	cout << "访问元素为：" <<name<<"["<<x<<","<<y<<"]="<< cache[index] << endl << endl;
	return cache[index];
}

void Matrix::refreshCache(int position)
{
	//writeback
	if (dirty == 1)
	{
		std::ofstream outFile(filename, std::ios::binary | std::ios::in);  

		outFile.seekp(start*sizeof(int));

		// 逐个写回cache的元素
		for (int i = 0; i < Block_Size; i++)
		{
			outFile.write(reinterpret_cast<const char*>(&cache[i]), sizeof(int));
		}

		// 关闭文件
		outFile.close();

		// 将 dirty 标志位重置为 0，表示写回操作已完成
		dirty = 0;
	}

	this->start = position;

	// 打开矩阵文件
	std::ifstream file(filename, std::ios::binary);  

	 // 定位到初始元素的偏移量位置
	file.seekg(start*sizeof(int));

	// 逐个读取元素到数组
	for (int i = 0; i < Block_Size; i++) {
		file.read(reinterpret_cast<char*>(&cache[i]), sizeof(int));
	}

	// 输出数组元素
	printCache();

	// 关闭文件
	file.close();
}

bool Matrix::InCache(int position)
{
	//判断是否命中
	if ((position > start || position == start) &&
		(position < start + Block_Size))
		return true;

	return false;
}

void Matrix::Dirty()
{
	dirty = 1;
}

void Matrix::printCache()
{
	std::cout << "Cache"<<name<<" :  ";
	for (int i = 0; i < Block_Size; i++) {
		std::cout << cache[i] << " ";
	}
	cout << endl<<endl;
}
