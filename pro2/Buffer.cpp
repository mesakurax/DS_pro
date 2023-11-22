#include"Buffer.h"


void Buffer::Generation()
{
	fstream file(filename, ios::binary | ios::in | ios::out);

	//声明一个block
	vector<int> block(length);

	//读写指针
	streampos readPos;
	streampos writePos;
	
	//从文件中读取length个数据，直至读取完毕
	while (file.read(reinterpret_cast<char*>(block.data()), length*sizeof(int)))
	{
		int bytesRead = file.gcount() / sizeof(int);  //读取元素的个数	
		InsertSort(block, bytesRead); //排序

		file.seekp(-static_cast<int>(sizeof(int) * bytesRead), ios::cur);  //找到写回的位置

		file.write(reinterpret_cast<char*>(block.data()), bytesRead * sizeof(int)); //写回

		readPos = file.tellg();
		writePos = file.tellp();
		file.seekg(readPos);
	}

	// 处理最后一次读取不足长度的情况
	file.clear();
	int lastBytesRead = file.gcount() / sizeof(int);
	if (lastBytesRead > 0)
	{
		InsertSort(block, lastBytesRead); //排序

		file.seekp(writePos);  //找到写回的位置

		file.write(reinterpret_cast<char*>(block.data()), lastBytesRead * sizeof(int)); //写回
	}
}

void Buffer::InsertSort(std::vector<int>& block, int num)
{
	if (num == 1)
		return;
	for (int i = 1; i < num; i++)
	{
		int key = block[i];  //记录元素
		int j = i - 1;

		while (j >= 0 && block[j] > key)
		{
			block[j + 1] = block[j];  //如果前面元素比key大就向右移动，直到碰到第一个小于或者等于key的元素
			j--;
		}
		block[j + 1] = key;  //插入key
	}
}

void Buffer::Merge()
{
	fstream file(filename, ios::binary | ios::in|ios::out);

	file.seekg(0, ios::end);  // 将读取指针定位到文件末尾
	streampos fileSize = file.tellg();  // 获取文件大小

	int file_num = fileSize / sizeof(int);  //总元素数目
	file.close();

	int cur_len = length;  //当前归并段长度

	//只剩一个归并段时退出
	while (cur_len < file_num)
	{
		cout << "\n归并段长度为: " << 2 * cur_len << endl << endl;
		int merge_num = ceil(static_cast<double>(file_num) / (2 * cur_len));  //归并次数

		for (int i = 0; i < merge_num; i++)  
		{
			// 清空 temp.bin 文件
			std::ofstream tempFile("temp.bin", std::ios::binary | std::ios::trunc);
			tempFile.close();

			//记录每次归并两个归并段的起始位置和终点位置
			int off_0 = i * cur_len*2;
			int end_0=off_0+ +cur_len-1;
			int off_1 = off_0 + cur_len;
			if (off_1 >= file_num)
				break;  //只有一个归并段，退出
			int end_1 = ((off_1 + cur_len) >= file_num) ? (file_num - 1) : (off_1 + cur_len - 1);


			cout <<"归并段1："<<off_0 << " " << end_0 << endl;
			cout <<"归并段2："<<off_1 << " " << end_1 << endl;
		  
			//两个input buffer的指针
			int index0 = -1;
			int index1 = -1;

			//当两个归并段至少有一个完成时退出
			while(!((off_0>end_0 && index0==-1)||(off_1 > end_1 && index1 == -1)))
			{
				
				//若input为空，则进行读入
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

				//归并操作，直至有一个input为空
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

				//为空时设index为-1
				if (index0 == inum_0)
					index0 = -1;
				if (index1 == inum_1)
					index1 = -1;
			}

			//若其中一个归并段完成，则直接导入另一个归并段以及input里的内容到Out
			if (off_0 > end_0)
			{
				//先导入input里的
				if(index1!=-1)
					for (int j = index1; j < inum_1; j++)
						write(I1[j]);

				//还未读进来的也导入
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

	// 计算应该读取的元素个数
	int elementsToRead = min(I_size, end - offset + 1);

	// 读取元素
	file.read(reinterpret_cast<char*>(I0), elementsToRead * sizeof(int));

	// 计算实际读取的元素个数
	int bytesRead = file.gcount() / sizeof(int);

	// 更新 inum_1，确保其不超过实际读取的元素个数
	inum_0 = min(bytesRead, length);

	// 关闭文件
	file.close();
}

void Buffer::read1(int offset, int end)
{
	fstream file(filename, ios::binary | ios::in);
	file.seekg(offset * sizeof(int));

	// 计算应该读取的元素个数
	int elementsToRead = min(I_size, end - offset + 1);

	// 读取元素
	file.read(reinterpret_cast<char*>(I1), elementsToRead * sizeof(int));

	// 计算实际读取的元素个数
	int bytesRead = file.gcount() / sizeof(int);

	// 更新 inum_1，确保其不超过实际读取的元素个数
	inum_1 = min(bytesRead, length);

	// 关闭文件
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
	cout << "归并完成: ";
	while (infile.read(reinterpret_cast<char*>(&value), sizeof(int))) {
		cout << value << " ";
		file.write(reinterpret_cast<const char*>(&value), sizeof(int));
	}
	cout << endl << endl;
	infile.close();
	file.close();
}
