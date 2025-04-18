// Problem statement

// A barber shop consists of a waiting room with n chairs, and a barber chair for giving haircuts.
// If there are no customers to be served, the barber goes to sleep.
// If a customer enters the barbershop and all chairs are occupied, then the customer leaves the shop.
// If the barber is busy, but chairs are available, then the customer sits in one of the free chairs. If the barber is asleep, the customer wakes up the barber.
// Write a program to coordinate the interaction between the barber and the customers.



#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <chrono>

class BarberShopProblem
{
private:
    bool barber_waiting;
    int waiting_seats;
    std::mutex m;
    std::condition_variable cv;
    std::queue<int> customers;

public:
    BarberShopProblem(int waiting_seats) : waiting_seats(waiting_seats), barber_waiting(true) {}
    void barber()
    {
        while (1)
        {
            std::unique_lock<std::mutex> lock(m);
            barber_waiting = true;
            while (customers.empty())
            {
                barber_waiting = true;
                std::cout << "Barber is sleeping...\n";
                cv.wait(lock); // Wait until a customer comes
            }

            // pop the customer;
            barber_waiting = false;
            int cus = customers.front();
            customers.pop();
            std::cout << "cutting hair of customer with id: " << cus << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::cout << "cutting hair of customer finished with id: " << cus << std::endl;
            barber_waiting = true;
            lock.unlock();
        }
    }

    void customer(int id)
    {

        std::unique_lock<std::mutex> lock(m);

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        if (customers.size() == waiting_seats)
        {
            std::cout << "customer leave the shop as not seat available!!, id is : " << id << std::endl;
            return;
        }
        else
        {

            customers.push(id);
            std::cout << "customer seats in the shop waiting room!!, id is : " << id << " qeueue size is: " << customers.size() << std::endl;
            cv.notify_one();
        }
    }
};

int main()
{
    BarberShopProblem barberShopProblem = BarberShopProblem(10);
    std::vector<std::thread> threads;
    std::thread barberThread(&BarberShopProblem::barber, &barberShopProblem);
    for (int i = 0; i < 20; i++)
    {
        threads.push_back(std::thread(&BarberShopProblem::customer, &barberShopProblem, i));
    }
    for (auto &t : threads)
        t.join();
    barberThread.join();
}