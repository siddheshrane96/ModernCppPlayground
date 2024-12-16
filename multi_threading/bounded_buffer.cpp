#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore>
#include <optional>

template <typename... Args>
void LOG(Args... args)
{
    (std::cout << ... << args) << std::endl; 
}

/**
 * LEARNING SEMAPHORES using Bounded buffers.
 * Bounded buffer is a circular buffer where we do not discard old values and wait until there's an empty
 * slot available to add new data.
 */

template <typename T, size_t N>
class BoundedBuffer
{
public:
    void push(const T& data)
    {
        do_push(data);
    }
    void push(T&& data)
    {
        do_push(std::forward<T>(data));
    }

    T pop()
    {
        m_data_full.acquire();
        // Delayed initialization using std::optional.
        std::optional<T> data;
        try{
            std::unique_lock lock(m_data_mutex);
            data = std::move(m_data[m_read_pos]);
            m_read_pos = (m_read_pos + 1) % N;
        }
        catch(...){
            m_data_full.release();
            throw;
        }
        m_data_empty.release();
        return std::move(*data);
    }

private:
    std::array<T, N> m_data;
    std::mutex m_data_mutex;
    size_t m_write_pos{0};
    size_t m_read_pos{0};

    std::counting_semaphore<N> m_data_full{0};
    std::counting_semaphore<N> m_data_empty{N};


    void do_push(auto&& data)
    {
        /// NOTE: acquire reduces the counter.
        m_data_empty.acquire();
        try{
            std::unique_lock lock(m_data_mutex);
            // This won't work because T is class parameter and not the deduced
            // type of data from do_push. Hence use, decltype.
            // m_data[m_write_pos] = std::forward<T>(data);
            m_data[m_write_pos] = std::forward<decltype(data)>(data);
            m_write_pos = (m_write_pos + 1) % N;
        }
        catch(...)
        {
            m_data_empty.release();
            throw;
        }
        m_data_full.release();
    }
};

void producer(BoundedBuffer<int, 5>&buffer, int N=20)
{
    for(int i=0; i<N; ++i)
    {
        buffer.push(i);
        LOG("-->Pushed ",i, " by ", std::this_thread::get_id());
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}

void consumer(BoundedBuffer<int, 5>&buffer, int N=20)
{
    for(int i=0; i<N; ++i)
    {
        int n = buffer.pop();
        LOG("<--Popped ",n, " by ", std::this_thread::get_id());
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main(int argc, char** argv)
{
    BoundedBuffer<int, 5> buffer;
    std::thread t1{producer, std::ref(buffer), 20};
    std::thread t2{consumer, std::ref(buffer), 20};
    t1.join();
    t2.join();
    return 0;
}
