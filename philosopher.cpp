#include <iostream>
#include <thread>
#include <semaphore>
#include <chrono>
#include <vector>

class Philosopher
{
private:
    std::vector<std::unique_ptr<std::binary_semaphore>> fork_semaphores;
    std::mutex m1,m2;

public:
    Philosopher() {
        for (int i = 0; i < 5; ++i) {
            fork_semaphores.push_back(std::make_unique<std::binary_semaphore>(1));
        }
    }

    void thinking(int id)
    {
        std::unique_lock<std::mutex> lock(m1);
        std::cout << id << " philosopher is thinking" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::cout << id << " philosopher is thinking finished" << std::endl;
    }
    void eating(int id)
    {
        std::unique_lock<std::mutex> lock(m2);
        std::cout << id << " philosopher is eating" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::cout << id << " philosopher is eating finished" << std::endl;
    };
    void job(int id)
    {
        while (1)
        {
            thinking(id);
            if (id == 4)
            {
                fork_semaphores[0]->acquire();
                fork_semaphores[4]->acquire();
            }
            else
            {
                fork_semaphores[id]->acquire();
                fork_semaphores[id + 1]->acquire();
            }

            eating(id);

            if (id == 4)
            {
                fork_semaphores[0]->release();
                fork_semaphores[4]->release();
            }
            else
            {
                fork_semaphores[id]->release();
                fork_semaphores[id + 1]->release();
            }
        }
    }
};

int main()
{
    Philosopher phi = Philosopher();
    std::vector<std::thread> philosophers;
    for (int i = 0; i < 5; i++)
    {
        philosophers.push_back(std::thread(&Philosopher::job, &phi, i));
    }
    for (auto &t : philosophers)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
}