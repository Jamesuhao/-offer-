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
#endif