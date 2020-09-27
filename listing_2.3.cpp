#include <thread>
/**
 * 使用资源获取【RAII】即初始化方法，等待线程完成
*/
class thread_guard
{
    std::thread& t;
public:
    explicit thread_guard(std::thread& t_):
        t(t_)
    {}
    ~thread_guard()
    {
        /**
         * 这很重要，因为join()只能对给定的对象调用一次，
         *  所以对给已加入的线程再次进行加入操作时，将会导致错误。
        */
        if(t.joinable())//1.首先判断线程是否已加入
        {
            t.join();//2.如果没有会调用join()进行加入
        }
    }
    /**
     * 在函数声明后面=delete，表示该函数的定义为弃置的，即定义无效
     * 这里表示：告诉编译器不自动生成拷贝函数和赋值函数
     * 任何尝试给thread_guard对象赋值的操作
     * 都会引发一个编译错误
    */
    thread_guard(thread_guard const&)=delete;//3
    thread_guard& operator=(thread_guard const&)=delete;
};

void do_something(int& i)
{
    ++i;
}

struct func
{
    int& i;

    func(int& i_):i(i_){}

    void operator()()
    {
        for(unsigned j=0;j<1000000;++j)
        {
            do_something(i);
        }
    }
};

void do_something_in_current_thread()
{}

/**
 * 知识点：局部变量是逆序销毁【堆栈特点：先进后出】
 * thread_guard对象g是第一个被销毁的，这时线程在析
 * 构函数中被加入2到原始线程中。即使do_something_in_current_thread
 * 抛出一个异常，这个销毁依旧会发生
*/
void f()
{
    int some_local_state;
    func my_func(some_local_state);
    std::thread t(my_func);
    thread_guard g(t);
        
    do_something_in_current_thread();
}//4

int main()
{
    f();
}
/**
 * 如果不想等待线程结束，可以分离(detaching)线程，从而避免异常安全(exception-safety)问题
 * std::thread::detach:打破了线程与std::thread对象的联系
 * 即使线程仍然在后台运行着，分离操作也能确保 std::terminate() 在 std::thread 对象销毁才被调用
 * std::thread::detach:表示分离式，该线程独立运行
 * 一旦该线程退出，则释放任何分配的资源，调用detach后，
 * *this不再占有任何线程
*/