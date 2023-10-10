#include "Matrix.h"

int Matrix::getElement(int x, int y)
{
	int position = x * rows + y;
	if (!InCache(x, y))
	{
		refreshCache(position);
		misses++;
	}
	int index = position - start;
	return cache[index];
}

void Matrix::refreshCache(int position)
{
	//writeback
	if (dirty == 1)
	{
		std::ofstream outFile(filename, std::ios::binary | std::ios::in);  

		outFile.seekp(start);

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

	start = position;

	// 打开矩阵文件
	std::ifstream file(filename, std::ios::binary);  

	 // 定位到初始元素的偏移量位置
	file.seekg(start);

	// 逐个读取元素到数组
	for (int i = 0; i < Block_Size; i++) {
		file.read(reinterpret_cast<char*>(&cache[i]), sizeof(int));
	}

	// 输出数组元素
	std::cout << "Cache 更新为:  ";
	for (int i = 0; i < Block_Size; i++) {
		std::cout << cache[i] << " ";
	}
	std::cout << std::endl;

	// 关闭文件
	file.close();
}

bool Matrix::InCache(int x, int y)
{
	int position = x * columns + y;   //定位元素在文件中的位置

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
