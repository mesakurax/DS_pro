#include "Matrix.h"

void Matrix::compute(int x, int y, int value)
{
	int position = x * columns + y;
	cout << "���ʾ���" << name << "�±�Ϊ: (" << x << "," << y << ")" << endl;

	//ȱʧʱˢ��cache
	if (!InCache(position))
	{
		cout << "����" << name << "ȱʧ, ����Cache" << endl;
		refreshCache(position);
		misses++;
	}
	int index = position - this->start;

	cout << "����" << name << "[" << x << "," << y << "]=" << cache[index];
	cache[index] += value;  //����Ԫ��
	cout << "--->" << cache[index] << endl << endl;
	dirty = 1;   //��λ����Ϊ1
}

int Matrix::getElement(int x, int y)
{
	int position = x * columns + y;
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
	//д��
	if (dirty == 1)
		writeback();

	num = 0;
	// �򿪾����ļ�
	std::ifstream file(filename, std::ios::in|std::ios::binary);  

	this->start = position;

	 // ��λ����ʼԪ�ص�ƫ����λ��
	file.seekg(start*sizeof(int));

	// �����ȡԪ�ص�����
	for (int i = 0; i < Block_Size; i++) {
		file.read(reinterpret_cast<char*>(&cache[i]), sizeof(int));
		num++;

		// ����Ƿ��Ѿ���ȡ���ļ�ĩβ
		if (file.eof()) {
			break;
		}
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

void Matrix::printCache()
{
	std::cout << "Cache"<<name<<" :  ";
	for (int i = 0; i < num; i++) {
		std::cout << cache[i] << " ";
	}
	cout << endl<<endl;
}

void Matrix::writeback()
{
	std::fstream outFile(filename, std::ios::in|std::ios::out | std::ios::binary);


	outFile.seekp((this->start) * sizeof(int));
	cout << "����" << name << "�±�Ϊ" << start << "��Ԫ��: ";

	// ���д��cache��Ԫ��
	for (int i = 0; i < num; i++)
	{
		cout << cache[i] <<"д��" << endl;
		outFile.write(reinterpret_cast<const char*>(&cache[i]), sizeof(int));
	}

	// �ر��ļ�
	outFile.close();

	// �� dirty ��־λ����Ϊ 0����ʾд�ز��������
	dirty = 0;
}
