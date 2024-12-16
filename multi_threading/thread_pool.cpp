#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <future>

template <typename... Args>
void LOG(Args... args)
{
    (std::cout << ... << args) << std::endl;
}

/**
 * NOTE: Class design
 * We need:
 *  1. Public method of enqueing tasks.
 *  2. Mutex and conditional var for the tasks queue.
 *  3. List of worker threads.
 *  4. List of tasks.
 * 
 *  Flow:
 *      When we create a thread, we will pass a lambda function to that thread, who keeps waiting
 *      on a conditional_variable to get a task assigned.
 */

class ThreadPool
{
public:
    ThreadPool(size_t num_threads = std::thread::hardware_concurrency())
    {
        for(size_t i=0; i<num_threads; ++i)
        {
            m_workers.push_back(std::thread{
                [this]{ worker();}
            });
        }
    }

    ~ThreadPool()
    {
        {
            std::lock_guard lock(m_queue_mutex);
            m_stop_flag = true;
        }

        m_queue_cv.notify_all();
        
        for(auto& thread: m_workers)
        {
            if(thread.joinable())
            {
                thread.join();
            }
        }
    }

    template <typename F, typename... Args>
    auto enqueue(F&& func, Args... args) -> std::future<decltype(func(args...))>
    {
        using ReturnType = decltype(func(args...));

        /// NOTE: shared_ptr is used here so that we can share ownership between
        /// worker thread and pool. 
        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
                std::bind(std::forward<F>(func), std::forward<Args>(args)...));

        std::future<ReturnType> result = task->get_future();
        {
            std::unique_lock lock(m_queue_mutex);
            if(m_stop_flag)
            {
                throw std::runtime_error("Cannot enqueue on stopped Threadpool");
            }
            
            m_task_queue.push([task]() { (*task)(); });
        }
        m_queue_cv.notify_one();
        return result;
    }

    // Bare minimum version that takes only void() functions.
    // void enqueue(const std::function<void()>& task)
    // {
    //     std::unique_lock lock(m_queue_mutex);
    //     m_task_queue.push(task);
    //     m_queue_cv.notify_one();
    // }

private:
    std::vector<std::thread> m_workers;
    /// QUESTION: Is function<void()> a generic signature that would work for all functions?
    /// What if I pass a function that has return type and some input parameters?
    /// What is genric signature? std::function<return_type(arg1, arg2,....)> ?
    std::queue<std::function<void()>> m_task_queue;
    std::mutex m_queue_mutex;
    std::condition_variable m_queue_cv;
    std::atomic<bool> m_stop_flag{false};

    void worker()
    {
        while(true)
        {
            std::function<void()> task;
            {
                std::unique_lock lock(m_queue_mutex);
                m_queue_cv.wait(lock, [this](){ 
                    return m_stop_flag || !m_task_queue.empty();
                });

                if(m_stop_flag && m_task_queue.empty())
                {
                    return;
                }

                task = std::move(m_task_queue.front());
                m_task_queue.pop();
            }
            task();
        }
    }
};

void dummy_useless_task(size_t ID)
{
    LOG("TASK ID: ", ID, " by thread: ", std::this_thread::get_id());
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

int main()
{
    ThreadPool thread_pool(5);

    /// TODO: make this generic
    std::vector<std::future<void>> results;

    for(size_t i=0; i<15; ++i)
    {
        results.push_back(
            thread_pool.enqueue([i](){ dummy_useless_task(i); })
        );
    }

    for(auto& result: results)
    {
        result.get();
    }

    LOG("All tasks completed. ");
    return 0;
}