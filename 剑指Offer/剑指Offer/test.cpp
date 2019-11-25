#if 0
//面试题1：赋值运算符函数
/*
题目信息：如下为类型CMyString的声明，请为该类型添加赋值运算符函数
class CMyString
{
public:
	CMyString(char* pData = nullptr);
	CMyString(const CMyString& str);
	~CMyString();
private:
	char* m_pData;
};
*/
/*
本题考察点：
1.是否把返回值的类型声明为该类型的引用，并在函数结束前返回实力自身的引用(即*this),支持连续赋值。
2.是否把传入的参数类型声明为const引用，从而减少拷贝，提高代码效率，赋值时不修改传入参数的值，所以声明为const。
3.是否在分配新内存之前释放实例自身已有的内存，不释放，容易造成内存泄漏。
4.是否检查自赋值操作，如果自身给自身赋值，在释放内存后将无法赋值，因为内存已经被释放掉了。
*/
//解答：
//方法一:常规写法
//返回值为自身的引用，支持连续赋值
//参数为cosnt引用，减少拷贝，如果传对象实例，从形参到实参会调用拷贝构造
CMyString& operator=(const CMystring& str)
{
	//检查自赋值，否则释放空间时会导致自身被释放，后面的程序将因为内存已被释放而崩溃。
	if (this == &str)
	{
		return *this;
	}
	//释放实例原有空间，防止内存泄露
	delete[]m_pData;
	m_pData = nullptr;
	//开辟空间
	m_pData = new char[strlen(str.m_pData) + 1];
	//赋值
	strcpy(m_pData, str.m_pData);
	//返回自身的引用
	return *this;
}
//方法二：现代写法
/*
函数解释：
在这个函数中，我们先创建一个临时对象str，接着把str.m_pData和实例自身的m_pData做交换。
由于str是一个局部变量，当函数结束时，就会自动调用str的析构函数，把str所指向的内存释放掉，
由于str.m_pData指向的内存就是实例之前m_pData的内存，这就相当于自动调用析构函数释放实例的内存。
*/
//返回值为自身的引用，支持连续赋值
//参数创建一个临时变量
CMyString& operator=(string str)
{
	if (this != &str)
	{
		//交换指针指向
		swap(m_pData, str.m_pData);
	}
	return *this;
}


//数据结构：
/*
数组、字符串、链表、树、栈及队列。
数组、字符串：内存连续，存储数字/字符。
链表、树：需要操作大量指针。
栈：和递归紧密相关。
队列：和广度优先遍历紧密相关。
*/

//数组：
/*
优点：顺序存储，空间连续，查找时间复杂度为O(1)
缺点：增加删除时间复杂度为O(n)
*/
//面试题2：二维数组的查找
/*
题目信息：
在一个二维数组中，每一行都按照从左到右递增的顺序排序，每一列都按照从上到下递增的顺序排序。
请完成一个函数，输入这样的一个二维数组和一个整数，判断数组中是否含有该整数。
例如下面的二维数组。如果在这个数组三种查找数字7，则返回true;
如果查找数字5，由于数组不含有该数字，则返回false。
1  2  8    9
2  4  9    12
4  7  10  13
6  8  11  15
解题思路：根据从左向右递增，从上到下递增的规则。
首先选取数组中右上角的数字，如果该数字等于要查找的数字，查找过程结束；
如果该数字大于要查找的数字，剔除这个数字所在的列；
如果该数字小于要查找的数字，剔除这个数字所在的行；
不断缩小范围，直到找到要查找的数字，或者查找范围为空。
*/
//解题代码：
bool Find(int* matrix, int rows, int cols, int num)
{
	if (matrix != nullptr && row > 0 && col > 0)
	{
		int row = 0;
		int col = cols - 1;
		while (row<rows && col>=0)
		{
			//该数字等于要查找的数字
			if (matrix[row][col] == num)
			{
				return true;
			}
			//该数字大于要查找的数字
			else if (matrix[row][col] > num)
			{
				--col;
			}
			//该数字小于要查找的数字
			else
			{
				++row;
			}
		}
	}
	return false;
}
//当然也可以选取左下角的数字进行查找，但是不能选取左上角或者右下角，否则将无法缩小范围。

//字符串：
//面试题3：替换空格
/*
题目信息：
请实现一个函数，把字符串中的每个空格替换成"%20"。
例如输入"We are happy."，则输出"We%20are%20happy."。
解题思路：假设不能开辟新的空间，并且输入字符串后面有足够多的空余内存。
O(n^2):
如果从头开始遍历字符串进行替换空格的操作，那么如果空格数大于一个，那么从替换第二个空格开始每替换一次空格，
该空格后的字符串被挪动的次数就要加一，这样的操作时间复杂度太高。
所以我们应该换一种思路，从后向前替换。
O(n):
首先，我们需要先遍历遍字符出统计原字符串的长度len和替换后的字符串的长度newlen。
newlen=len+2*Blankcount。(Blankcount代表空格的个数，两个长度均包含'\0'）
接下来，我们开始从后向前遍历字符串并且替换空格。
我们定义两个指针p和q，p指向原字符串的末尾，q指向替换后的字符串的末尾，
向前移动p和q，逐个将p所指向的字符复制到q所指向的位置，直到碰到空格为止,
在q位置以及q的前两个位置插入''%20''，然后p和q再次同时向前移动，重复上述过程，
直至p和q相等时，则替换结束。
*/
//代码实现：


//length为字符数组string的总容量
void ReplaceBlank(char string[], int length)
{
	if (string == nullptr && length <= 0)
	{
		return;
	}
	//原字符串长度
	int len = 0;
	//空格的个数
	int Blankcount = 0;
	int i = 0;
	//统计原字符串的长度和空格的个数
	while (string[i] != '\0')
	{
		++len;
		if (string[i] == ' ')
		{
			++Blankcount;
		}
		++i;
	}
	//替换后字符串长度
	int newlen = len + 2 * Blankcount;
	if (newlen > length)
	{
		return;
	}
	//指向原字符串的末尾
	int index = len;
	//指向替换后的新字符串的末尾
	int newindex = newlen;
	while (indexlen >= 0 && newindexlen > indexlen)
	{
		//遇到空格开始替换并且更新newindexlen
		if (string[indexlen] == ' ')
		{
			string[newindex--] = '0';
			string[newindex--] = '2';
			string[newindex--] = '%';
		}
		//逐个将indexlen所指向的字符复制到newindexlen所指向的位置
		else
		{
			string[newindex--] = string[index];
		}
		//更新indexlen
		--index;
	}
}
#endif