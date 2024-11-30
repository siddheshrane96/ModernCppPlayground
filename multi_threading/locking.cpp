#include <iostream>
#include <mutex>
#include <thread>
#include <cassert>

template <typename... Args>
void LOG(Args... args)
{
    (std::cout << ... << args) << std::endl;
}


/**
 * Example 1: Simple locking.
 */
int shared_data = 0;
std::mutex shared_data_mutex;

void increment_data(int N)
{
    std::lock_guard lock(shared_data_mutex);
    /**
     * NOTE:
     * Both std::lock_guard and std::scoped_lock give same results here.
     * But std::lock_guard is simplest implementation that can only lock
     * one mutex and has no mechanism to avoid deadlock.
     * 
     * std::scoped_lock on other hand can lock multiple mutexes. Also, it
     * internally uses std::lock that allows it to lock multiple locks
     * simultaneously.
     * eg: std::scoped_lock lock(mutex1, mutex2);
     */
    for(size_t i=0; i<N; ++i)
    {
        shared_data += 1;
    }
}

/**
 * Example 2: Avoiding deadlock.
 * NOTE: Deadlock occurs when a thread acquires more than one locks. One of
 * the ways to avoid this is to acquire both locks simultaneously. That's
 * achieved with std::lock.
 */
struct Account
{
    int _balance {0}; // critical code that will be shared.
    std::mutex _mut; 
};

void transfer_money(Account& from_, Account& to_, int amount)
{
    // We want to create locks for both mutexes in from_ and to_
    // But we should not lock them immediately when we acquire them. This can
    // be achieved with, std::unique_lock and std::defer_lock
    std::unique_lock<std::mutex> lock1{from_._mut, std::defer_lock}; // Acquire mutex but don't lock it.
    std::unique_lock<std::mutex> lock2{to_._mut, std::defer_lock};

    // Lock them simultaneously.
    std::lock(lock1, lock2);
    from_._balance -= amount;
    to_._balance += amount;
}

int main(int argc, char** argv)
{
    std::thread t1{increment_data, 4000000};
    std::thread t2{increment_data, 4000000};

    t1.join();
    t2.join();
    LOG("Shared data ", shared_data);

    /**
     * Example 2: Transfer money with simultaneous lock.
     */
    Account from{1000};
    Account to{1000};
    transfer_money(from, to, 500);
    LOG("Balance from ", from._balance, " to ", to._balance);
    return 0;
}