#include <thread>
#include <iostream>

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
            do_something(i);//1.潜在访问隐患：悬空引用
        }
    }
};

/*
*使用一个能访问局部变量的函数去创建线程是一个糟糕的主意(除非十
*分确定线程会在函数完成前结束)。此外，可以通过加入的
*方式来确保线程在函数完成前结束。
*/
void oops()
{
    int some_local_state=0;
    func my_func(some_local_state);
    std::thread my_thread(my_func);
    /*
    *std::thread::detach:表示分离式，该线程独立运行
    *一旦该线程退出，则释放任何分配的资源，调用detach后，
    *this不再占有任何线程
    */
    // my_thread.detach();//2.不等待线程结束
    /**
     * std::thread::join:让调用者等待线程结束，并且清理了线程相关的存储部
      *  分，使std::thread 对象将不再与已经完成的线程有任何关联。这意味着，只能对一个线程使用一次join();一
      *  旦已经使用过join()， std::thread 对象就不能再次加入了，当对其使用joinable()时，将返回否（false）
      * 当在线程运行之后产生异常，在join()调用之前抛出，就意味着很这次调用会被跳过。
      * 避免应用被抛出的异常所终止，就需要作出一个决定。通常，当倾向于在无异常的情况下使用join()时，需要在
      *  异常处理过程中调用join()，从而避免生命周期的问题
    */
    my_thread.join();
    if (!my_thread.joinable())
    {
        std::cout << "不能再加入" << std::endl;
    }
    
}//3.新线程可能还在运行

int main()
{
    oops();
}
