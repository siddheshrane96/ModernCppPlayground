#include <thread>
#include <iostream>
#include <random>
#include <array>
#include <vector>
#include <algorithm>
#include <barrier>

template <typename... Args>
void LOG(Args... args)
{
    (std::cout << ... << args) << std::endl;
}

/**
 * RANDOM NUMBER GENERATOR.
 * 1. Engine: Responsible for generating the raw random numbers.
 *      a. std::default_random_engine: A good default for most cases.
 *      b. std::mt19937: A Mersenne Twister engine, which is very popular.
 *      c. std::random_device: Uses a non-deterministic random number source, often tied to hardware.
 * 2. Distribution
 *      a. std::uniform_int_distribution<int>: Uniform distribution for integers.
        b. std::uniform_real_distribution<double>: Uniform distribution for floating-point numbers.
        c. std::normal_distribution<double>: Generates numbers in a bell curve (Gaussian distribution).
        d. std::bernoulli_distribution: For true/false outcomes based on a probability.

    Use std::random_device for non-deterministic seeding (preferred).
    Alternatively, use a fixed seed for reproducibility (e.g., engine.seed(12345)).
 */
int random_int(int min, int max)
{
    // For multithreading, use thread_local variables to ensure each thread has its own engine:
    static thread_local auto engine = std::default_random_engine(std::random_device{}());
    auto dist = std::uniform_int_distribution<int>(min, max);
    return dist(engine);
}

/// A silly program that creates 5 different threads, each thread produces a dice result.
/// Our program has fork-join pattern to check if all dices got number 6.
int main(int argc, char** argv)
{
    constexpr int num_dice = 5;
    std::array<int, num_dice> dices;
    std::vector<std::thread> threads;
    bool done = false;
    int num_turns = 0;
    auto checker = [&]() {
        ++num_turns;
        auto is_six = [](int i){ return i == 6;};
        done = std::all_of(dices.begin(), dices.end(), is_six);
    };
    
    auto bar = std::barrier{num_dice, checker};
    for(int i=0; i<num_dice; ++i)
    {
        threads.emplace_back(
            std::thread{
                [&, i]() {
                    while(!done)
                    {
                        dices[i] = random_int(1,6);
                        bar.arrive_and_wait();
                    }
                }
            }
        );
    }

    for(auto& t: threads)
    {
        t.join();
    }
    std::cout << num_turns << '\n';
    return 0;
}