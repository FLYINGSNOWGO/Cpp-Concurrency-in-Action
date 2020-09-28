#include <thread>
#include <utility>
//为了确保线程程序退出前完成
class scoped_thread
{
    std::thread t;
public:
    explicit scoped_thread(std::thread t_)://1
        t(std::move(t_))
    {
        //把检查放在了构造函数中，并且当线
        //程不可加入时，抛出异常
        if(!t.joinable())//2
            throw std::logic_error("No thread");
    }
    ~scoped_thread()
    {
        t.join();//3
    }
    scoped_thread(scoped_thread const&)=delete;
    scoped_thread& operator=(scoped_thread const&)=delete;
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

void f()
{
    int some_local_state;
    /**
     * 新线程是直接传递到scoped_thread中，而非创建一个独立的命名变量
     * std::thread(func(some_local_state))是一个未命名的临时对象
     * 会自动std::move
     * 
    */
    scoped_thread t(std::thread(func(some_local_state)));//4
        
    do_something_in_current_thread();
    /**
     * 当主线程到达f()函数的末尾时，scoped_thread对象将会销毁，
     * 然后加入到的构造函数创建的线程对象中去
    */
}//5

int main()
{
    f();
}
