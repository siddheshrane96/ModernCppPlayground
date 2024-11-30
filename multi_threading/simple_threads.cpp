#include <iostream>
#include <thread>
#include <chrono>

template <typename... Args>
void LOG(Args... args)
{
    (std::cout << ... << args) << std::endl;
}

void talker()
{
    LOG(std::this_thread::get_id(), " IS TALKING NON STOP!");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    LOG(std::this_thread::get_id(), " IS DONE TALKING!");
}

/**
 * EXAMPLE JTHREADS
 * std::jthread has support for stopping a thread using a stop token.
 * This is something that we had to implement manually before C++20 when using std::thread.
 * Instead of terminating the app when it is being destructed in a non-joinable state, the destructor
 * of std::jthread will send a stop request and join the thread on destruction.
 */
void talker_non_stop(std::stop_token stoken)
{
    while(!stoken.stop_requested())
    {
        LOG(std::this_thread::get_id(), "TALK TALK...");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    LOG("Stop request received! Stopping thread ", std::this_thread::get_id());
}

int main()
{
    // This keeps changing with every new run.
    LOG("Main Thread ID ", std::this_thread::get_id());
    
    std::thread t1(talker);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    LOG("Doing some work!");
    /**
     * If you want to make thread non-blocking, use t1.detach()
     */
    t1.join();
    
    // Print total number of hardware threads
    LOG("HW threads: ",std::thread::hardware_concurrency());

    /** 
        C++20 introducted std::jthreads
        std::jthread has support for stopping a thread using a stop token.
        This is something that we had to implement manually before C++20 when using std::thread.
        Instead of terminating the app when it is being destructed in a non-joinable state,
        the destructor of std::jthread will send a stop request and join the thread on destruction.
     */

    /**
     * Example 2: Need for jthread. Uncomment [1] to see how thread aborts if not joined.
     *  will see output
     *  "terminate called without an active exception Aborted (core dumped)"
     */
    // std::thread t2(talker); // [1] Causes Abort
    std::jthread t2(talker); // Works nicely (only available in C++ 20)

    // Another example of jthread
    auto j2_thread = std::jthread(talker_non_stop);
    LOG("Main sleeping again");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    /// This is unnecessary as jthread will call request_stop on destruction anyways.
    j2_thread.request_stop();
    return 0;
}