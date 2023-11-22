#include"Buffer.h"


void Buffer::Generation()
{
	fstream file(filename, ios::binary | ios::in | ios::out);

	//����һ��block
	vector<int> block(length);

	//��дָ��
	streampos readPos;
	streampos writePos;
	
	//���ļ��ж�ȡlength�����ݣ�ֱ����ȡ���
	while (file.read(reinterpret_cast<char*>(block.data()), length*sizeof(int)))
	{
		int bytesRead = file.gcount() / sizeof(int);  //��ȡԪ�صĸ���	
		InsertSort(block, bytesRead); //����

		file.seekp(-static_cast<int>(sizeof(int) * bytesRead), ios::cur);  //�ҵ�д�ص�λ��

		file.write(reinterpret_cast<char*>(block.data()), bytesRead * sizeof(int)); //д��

		readPos = file.tellg();
		writePos = file.tellp();
		file.seekg(readPos);
	}

	// �������һ�ζ�ȡ���㳤�ȵ����
	file.clear();
	int lastBytesRead = file.gcount() / sizeof(int);
	if (lastBytesRead > 0)
	{
		InsertSort(block, lastBytesRead); //����

		file.seekp(writePos);  //�ҵ�д�ص�λ��

		file.write(reinterpret_cast<char*>(block.data()), lastBytesRead * sizeof(int)); //д��
	}
}

void Buffer::InsertSort(std::vector<int>& block, int num)
{
	if (num == 1)
		return;
	for (int i = 1; i < num; i++)
	{
		int key = block[i];  //��¼Ԫ��
		int j = i - 1;

		while (j >= 0 && block[j] > key)
		{
			block[j + 1] = block[j];  //���ǰ��Ԫ�ر�key��������ƶ���ֱ��������һ��С�ڻ��ߵ���key��Ԫ��
			j--;
		}
		block[j + 1] = key;  //����key
	}
}

void Buffer::Merge()
{
	fstream file(filename, ios::binary | ios::in|ios::out);

	file.seekg(0, ios::end);  // ����ȡָ�붨λ���ļ�ĩβ
	streampos fileSize = file.tellg();  // ��ȡ�ļ���С

	int file_num = fileSize / sizeof(int);  //��Ԫ����Ŀ
	file.close();

	int cur_len = length;  //��ǰ�鲢�γ���

	//ֻʣһ���鲢��ʱ�˳�
	while (cur_len < file_num)
	{
		cout << "\n�鲢�γ���Ϊ: " << 2 * cur_len << endl << endl;
		int merge_num = ceil(static_cast<double>(file_num) / (2 * cur_len));  //�鲢����

		for (int i = 0; i < merge_num; i++)  
		{
			// ��� temp.bin �ļ�
			std::ofstream tempFile("temp.bin", std::ios::binary | std::ios::trunc);
			tempFile.close();

			//��¼ÿ�ι鲢�����鲢�ε���ʼλ�ú��յ�λ��
			int off_0 = i * cur_len*2;
			int end_0=off_0+ +cur_len-1;
			int off_1 = off_0 + cur_len;
			if (off_1 >= file_num)
				break;  //ֻ��һ���鲢�Σ��˳�
			int end_1 = ((off_1 + cur_len) >= file_num) ? (file_num - 1) : (off_1 + cur_len - 1);


			cout <<"�鲢��1��"<<off_0 << " " << end_0 << endl;
			cout <<"�鲢��2��"<<off_1 << " " << end_1 << endl;
		  
			//����input buffer��ָ��
			int index0 = -1;
			int index1 = -1;

			//�������鲢��������һ�����ʱ�˳�
			while(!((off_0>end_0 && index0==-1)||(off_1 > end_1 && index1 == -1)))
			{
				
				//��inputΪ�գ�����ж���
				if (index0 == -1)
				{
					read0(off_0, end_0);
					off_0 += inum_0;
					index0 = 0;
				}
				if (index1 == -1)
				{
					read1(off_1, end_1);
					off_1 += inum_1;
					index1 = 0;
				}

				//�鲢������ֱ����һ��inputΪ��
				while (index0 < inum_0 && index1 < inum_1) 
				{
					if (I0[index0] <= I1[index1])
					{
						write(I0[index0]);
						index0++;
					}
					else
					{

						write(I1[index1]);
						index1++;
					}
				}

				//Ϊ��ʱ��indexΪ-1
				if (index0 == inum_0)
					index0 = -1;
				if (index1 == inum_1)
					index1 = -1;
			}

			//������һ���鲢����ɣ���ֱ�ӵ�����һ���鲢���Լ�input������ݵ�Out
			if (off_0 > end_0)
			{
				//�ȵ���input���
				if(index1!=-1)
					for (int j = index1; j < inum_1; j++)
						write(I1[j]);

				//��δ��������Ҳ����
				while (off_1 <= end_1)
				{
					read1(off_1, end_1);
					off_1 += inum_1;
					for (int j = 0; j < inum_1; j++)
						write(I1[j]);
				}
			}
			if (off_1 > end_1)
			{
				if (index0 != -1)
				{
					for (int j = index0; j < inum_0; j++)
						write(I0[j]);
				}
				while (off_0 <= end_0)
				{
					read0(off_0, end_0);
					off_0 += inum_0;
					for (int j = 0; j < inum_0; j++)
						write(I0[j]);
				}
			}

			writeback();
			writeInfile(i * cur_len * 2);
		}
		cur_len *= 2;
	}
}

void Buffer::read0(int offset, int end)
{
	fstream file(filename, ios::binary | ios::in);
	file.seekg(offset * sizeof(int));

	// ����Ӧ�ö�ȡ��Ԫ�ظ���
	int elementsToRead = min(I_size, end - offset + 1);

	// ��ȡԪ��
	file.read(reinterpret_cast<char*>(I0), elementsToRead * sizeof(int));

	// ����ʵ�ʶ�ȡ��Ԫ�ظ���
	int bytesRead = file.gcount() / sizeof(int);

	// ���� inum_1��ȷ���䲻����ʵ�ʶ�ȡ��Ԫ�ظ���
	inum_0 = min(bytesRead, length);

	// �ر��ļ�
	file.close();
}

void Buffer::read1(int offset, int end)
{
	fstream file(filename, ios::binary | ios::in);
	file.seekg(offset * sizeof(int));

	// ����Ӧ�ö�ȡ��Ԫ�ظ���
	int elementsToRead = min(I_size, end - offset + 1);

	// ��ȡԪ��
	file.read(reinterpret_cast<char*>(I1), elementsToRead * sizeof(int));

	// ����ʵ�ʶ�ȡ��Ԫ�ظ���
	int bytesRead = file.gcount() / sizeof(int);

	// ���� inum_1��ȷ���䲻����ʵ�ʶ�ȡ��Ԫ�ظ���
	inum_1 = min(bytesRead, length);

	// �ر��ļ�
	file.close();
}

void Buffer::write(int value)
{
	Out[onum] = value;
	onum++;

	if (onum == O_size)
		writeback();
}

void Buffer::writeback()
{
	fstream file("temp.bin", ios::binary | ios::out | ios::app);
	file.seekp(ios::end);
	for (int i = 0; i < onum; i++)
		file.write(reinterpret_cast<const char*>(&Out[i]), sizeof(int));
	file.close();
	onum = 0;
}

void Buffer::writeInfile(int start)
{
	ifstream infile("temp.bin", std::ios::binary);
	
	fstream file(filename, std::ios::in | std::ios::out | std::ios::binary);;

	file.seekp(start*sizeof(int));

	int value;
	cout << "�鲢���: ";
	while (infile.read(reinterpret_cast<char*>(&value), sizeof(int))) {
		cout << value << " ";
		file.write(reinterpret_cast<const char*>(&value), sizeof(int));
	}
	cout << endl << endl;
	infile.close();
	file.close();
}
