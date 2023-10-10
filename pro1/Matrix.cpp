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

		// ���д��cache��Ԫ��
		for (int i = 0; i < Block_Size; i++)
		{
			outFile.write(reinterpret_cast<const char*>(&cache[i]), sizeof(int));
		}

		// �ر��ļ�
		outFile.close();

		// �� dirty ��־λ����Ϊ 0����ʾд�ز��������
		dirty = 0;
	}

	start = position;

	// �򿪾����ļ�
	std::ifstream file(filename, std::ios::binary);  

	 // ��λ����ʼԪ�ص�ƫ����λ��
	file.seekg(start);

	// �����ȡԪ�ص�����
	for (int i = 0; i < Block_Size; i++) {
		file.read(reinterpret_cast<char*>(&cache[i]), sizeof(int));
	}

	// �������Ԫ��
	std::cout << "Cache ����Ϊ:  ";
	for (int i = 0; i < Block_Size; i++) {
		std::cout << cache[i] << " ";
	}
	std::cout << std::endl;

	// �ر��ļ�
	file.close();
}

bool Matrix::InCache(int x, int y)
{
	int position = x * columns + y;   //��λԪ�����ļ��е�λ��

	//�ж��Ƿ�����
	if ((position > start || position == start) &&
		(position < start + Block_Size))
		return true;

	return false;
}

void Matrix::Dirty()
{
	dirty = 1;
}
