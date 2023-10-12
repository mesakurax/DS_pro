#include "Matrix.h"

void Matrix::compute(int x, int y, int value)
{
	int position = x * rows + y;
	cout << "���ʾ���" << name << "�±�Ϊ: (" << x << "," << y << ")" << endl;

	//ȱʧʱˢ��cache
	if (!InCache(position))
	{
		cout << "����" << name << "ȱʧ, ����Cache" << endl;
		refreshCache(position);
		misses++;
	}
	int index = position - start;

	cout << "����" << name << "[" << x << "," << y << "]=" << cache[index];
	cache[index] += value;  //����Ԫ��
	cout << "--->" << cache[index] << endl << endl;
	dirty = 1;   //��λ����Ϊ1
}

int Matrix::getElement(int x, int y)
{
	int position = x * rows + y;
	cout << "���ʾ���" << name << "�±�Ϊ: (" << x << "," << y << ")" << endl;

	//ȱʧʱˢ��cache
	if (!InCache(position))
	{
		cout << "����" << name << "ȱʧ, ����Cache" << endl;
		refreshCache(position);
		misses++;
	}
	int index = position - start;
	cout << "����Ԫ��Ϊ��" <<name<<"["<<x<<","<<y<<"]="<< cache[index] << endl << endl;
	return cache[index];
}

void Matrix::refreshCache(int position)
{
	//writeback
	if (dirty == 1)
	{
		std::ofstream outFile(filename, std::ios::binary | std::ios::in);  

		outFile.seekp(start*sizeof(int));

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

	this->start = position;

	// �򿪾����ļ�
	std::ifstream file(filename, std::ios::binary);  

	 // ��λ����ʼԪ�ص�ƫ����λ��
	file.seekg(start*sizeof(int));

	// �����ȡԪ�ص�����
	for (int i = 0; i < Block_Size; i++) {
		file.read(reinterpret_cast<char*>(&cache[i]), sizeof(int));
	}

	// �������Ԫ��
	printCache();

	// �ر��ļ�
	file.close();
}

bool Matrix::InCache(int position)
{
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

void Matrix::printCache()
{
	std::cout << "Cache"<<name<<" :  ";
	for (int i = 0; i < Block_Size; i++) {
		std::cout << cache[i] << " ";
	}
	cout << endl<<endl;
}
