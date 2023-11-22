#include"MergeTree.h"
using namespace std;

const int buffer = 4;


//生成测试文件
void creat(string filename, int num)
{
    // 打开二进制文件进行写入
    ofstream file(filename, std::ios::out | std::ios::binary);

    srand(static_cast<int>(time(0)));

    // 逐个元素写入文件
    for (int i = 0; i < num; i++) {
        int num = rand() % 1000;
        file.write(reinterpret_cast<char*>(&num), sizeof(int));
    }
    file.close();
}

//打印
void PrintBinaryFile(string filename)
{
    ifstream file(filename, ios::binary);
    if (!file)
    {
        cerr << "Failed to open the file: " << filename << std::endl;
        return;
    }

    int value;
    while (file.read(reinterpret_cast<char*>(&value), sizeof(int)))
        std::cout << value << " ";
    cout << endl;
    file.close();
}

//找第一个比element大的元素
int findNextGreaterElement(vector<int> arr, int element) {

    int minIndex = -1;

    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] > element) {
            if (minIndex == -1 || arr[i] < arr[minIndex]) {
                minIndex = i;
            }
        }
    }
    return minIndex;
}

//置换—选择排序算法生成初始归并段
vector<seg> InMerge(string inname,string outname)
{
    ifstream fin(inname, ios::binary|ios::out );
    ofstream fout(outname,ios::binary| ios::in );


    vector<int> a;
    vector<seg> result;

    int index = 0;
    int data;
    int start = 0;
    int end = -1;
    int minMax = INT_MIN;

    cout << "初始归并段及位置: " << endl;
    while(fin.read(reinterpret_cast<char*>(&data), sizeof(int)))
    {
    
        if (a.size() < buffer)   //先填满向量a
            a.push_back(data);
        
  
        else {    
        
            if (findNextGreaterElement(a, minMax) == -1)  //进入新归并段
            {
                minMax = INT_MIN;
                result.push_back(seg(start, end, end - start + 1));  //记录上一归并段位置
                cout << " 位置: "<<start << " " << end << endl;
                start = end + 1;
            }
            
            index = findNextGreaterElement(a, minMax);  //找到向量中符合条件的该归并段元素
            fout.write(reinterpret_cast<char*>(&a[index]), sizeof(int));
            cout << a[index] << " ";    
            minMax = a[index];

            a[index] = data; //插入新元素
            end++;
           
        }
    }

    //处理剩余的元素
    while (!a.empty())
    {
        if (findNextGreaterElement(a, minMax) == -1)
        {
            minMax = INT_MIN;
            result.push_back(seg(start, end, end - start + 1));
            cout << " 位置: " << start << " " << end << endl;
            start = end + 1;
            
        }

        index = findNextGreaterElement(a, minMax);
        fout.write(reinterpret_cast<char*>(&a[index]), sizeof(int));
        cout << a[index] << " ";
        a.erase(a.begin() + index);
        end++;
    }
    result.push_back(seg(start, end, end - start + 1));
    cout << " 位置: " << start << " " << end << endl;

    fin.close();
    fout.close();

    return result;
}


int main() {
   
    creat("test.bin", 100);  
    PrintBinaryFile("test.bin");
    cout << endl;

    MergeTree s("TEST.bin", InMerge("test.bin" ,"TEST.bin"));
    s.MertSort();
    cout << "\n\n归并结果为:\n";
    PrintBinaryFile("temp.bin");
}