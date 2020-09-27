#include <iostream>
/**
* 管理线程的函数和类在<thread>中，声明，
* 而保护共享数据的函数和类在其他头文件中声明
*/
#include <thread>//1
/**
* 写信息的代码被移动到了一个独立的函数中
* 知识点：每个线程都必须具有一个初始函数，
* 新线程的执行从初始化函数开始
* 对于应用程序来说，初始线程【主线程】的初始函数是main，
* 对于其他线程，可以在std::thread对象的构造函数中指定
*/
void hello()//2
{
    std::cout<<"Hello Concurrent World\n";
}

int main()
{
    /**
	* 被命名为t的std::thread对象拥有新函数Hello()
	* 作为t的初始函数
	*/
    std::thread t(hello);//3
    /**
* std::thread::join作用：使调用线程等待与std::thread对象相关联的线程
* 即：主线程在初始化函数中等待t线程初始化Hello执行，执行完毕后
* 主线程继续执行std::thread::join后的代码，直到结束
*/
    t.join();//4
}
