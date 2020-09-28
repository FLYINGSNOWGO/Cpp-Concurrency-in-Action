#include <thread>
#include <numeric>
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>
/**
 * 代码中将整体工作拆分成小任务交给每个线程去做，其中设置最
小任务数，是为了避免产生太多的线程。
*/
template<typename Iterator,typename T>
struct accumulate_block
{
    void operator()(Iterator first,Iterator last,T& result)
    {
        //std::accumulate累加并将结果存入result
        //必须都是向前迭代器(forward iterato
        //r)，而 std::accumulate 可以在只传入迭代器(input iterators)的情况下工作
        result=std::accumulate(first,last,result);
    }
};

template<typename Iterator,typename T>
T parallel_accumulate(Iterator first,Iterator last,T init)
{
    unsigned long const length=std::distance(first,last);
    //如果输入的范围为空，就会得到init的值
    if(!length)//1
        return init;
    //最小任务数
    unsigned long const min_per_thread=25;
    unsigned long const max_threads=
        (length+min_per_thread-1)/min_per_thread;//2

    //硬件并发数
    unsigned long const hardware_threads=
        std::thread::hardware_concurrency();

    unsigned long const num_threads=//3
        std::min(hardware_threads!=0?hardware_threads:2,max_threads);

    //4
    unsigned long const block_size=length/num_threads;//4

    std::vector<T> results(num_threads);
    std::vector<std::thread>  threads(num_threads-1);//5

    Iterator block_start=first;
    for(unsigned long i=0;i<(num_threads-1);++i)
    {
        Iterator block_end=block_start;
        //block_end迭代器指向当前块的末尾
        std::advance(block_end,block_size);//6
        threads[i]=std::thread(//7
            accumulate_block<Iterator,T>(),
            block_start,block_end,std::ref(results[i]));
        //当迭代器指向当前块的末尾时，启动下一个块
        block_start=block_end;//8
    }
    //启动所有线程后，⑨中的线程会处理最终块的结果。对于分配不均，
    //因为知道最终块是哪一个，那么这个块中有
    //多少个元素就无所谓了
    accumulate_block<Iterator,T>()(block_start,last,results[num_threads-1]);//9
    
    std::for_each(threads.begin(),threads.end(),
        std::mem_fn(&std::thread::join));//10

    return std::accumulate(results.begin(),results.end(),init);//11
}
/**
 * 需要明确：T类型的加法运算不满足结合律
 * (比如，对于float型或double型，在进行加法操作
时，系统很可能会做截断操作)
 * 因为不能直接从一个线程中返回一个值，所以需要传
递results容器的引用到线程中去std::ref
另一个办法，通过地址来获取线程执行的结果；第4章中，我们将使用期望(fu
tures)完成这种方案。
*/
int main()
{
    std::vector<int> vi;
    for(int i=0;i<10;++i)
    {
        vi.push_back(10);
    }
    int sum=parallel_accumulate(vi.begin(),vi.end(),5);
    std::cout<<"sum="<<sum<<std::endl;
}
