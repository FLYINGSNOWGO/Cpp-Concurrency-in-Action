#include <thread>

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

void f()
{
    int some_local_state=0;
    func my_func(some_local_state);
    std::thread t(my_func);
    /**
     *  try/catch 块确保访问本地状态的线程退出后，函数才结束
     *  try/catch 块能轻易的捕获轻量级错误，但是有些错误不能
    */
    try
    {
        do_something_in_current_thread();
    }
    catch(...)
    {
        t.join();//1
        throw;
    }
    t.join();//2
}

int main()
{
    f();
}
