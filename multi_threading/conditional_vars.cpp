#include <iostream>
#include <queue>
#include <string>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <chrono>
#include <thread>
#include <future>

template <typename... Args>
void LOG(Args... args)
{
    (std::cout << ... << args) << std::endl;
}

// Global critical section
std::queue<int> QUEUE;
std::mutex queue_mutex;
std::condition_variable cv;

void publisher(int N)
{
   for(int i=0; i<N; ++i)
   {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::scoped_lock lock{queue_mutex};
    QUEUE.push(i);
    LOG("Number published ", i);
    cv.notify_one();
   }
}

void subscriber()
{
    int i;
    while(true)
    {
        {
            std::unique_lock lock(queue_mutex);
            cv.wait(lock, [](){ return !QUEUE.empty(); });
            i = QUEUE.front();
            QUEUE.pop();
        }
        LOG("Number received ", i);
    }
}

/**
 * EXAMPLE:2 Handle return types with std::future and std::promise.
 */
void divide(int a, int b, std::promise<float>& p)
{
    if(b == 0)
    {
        auto e = std::runtime_error{"Divide by Zero Exception"};
        p.set_exception(std::make_exception_ptr(e));
    }
    else
    {
        float div = a / b;
        p.set_value(div);   
    }
}

int main(int argc, char** argv)
{
    // std::thread t1{publisher, 100};
    // std::thread t2{subscriber};
    // t1.join();
    // t2.join();

    // Example 2
    std::promise<float> p{};
    std::thread t{divide, 10, 2, std::ref(p)};
    t.detach();
    auto future = p.get_future();
    try
    {
        float ans = future.get();
        LOG("DIV Received ", ans);
    }
    catch(const std::exception& e)
    {
        LOG("Caught ", e.what());
    }
    return 0;
}