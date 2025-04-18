// Problem statement

// Imagine at the end of a political conference, Republicans and Democrats are trying to leave the venue and ordering Uber rides at the same time. To avoid conflicts, each ride can have either all Democrats or Republicans or two Democrats and two Republicans.

// All other combinations can result in a fist-fight.

// Your task as the Uber developer is to model the ride requestors as threads. Once an acceptable combination of riders is possible, threads are allowed to proceed to ride.

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include<vector>
#include<string>

class Semaphore
{
private:
    int count = 0;
    std::mutex m;
    std::condition_variable cv;

public:
    Semaphore(int count) : count(count) {}

    void acquire()
    {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [this]()
                { return count > 0; });
        count--;
    }

    void release(int n = 1)
    {
        std::unique_lock<std::mutex> lock(m);
        count += n;
        for (int i = 0; i < n; i++)
        {
            cv.notify_one();
        }
    }
};

class Barrier{
    private:
        int count;
        int waiting;
        std::mutex m;
        std::condition_variable cv;
    public:
        Barrier(int count):count(count), waiting(0){}
        void wait(){
            std::unique_lock<std::mutex>lock(m);
            waiting++;
            if(waiting==count){
                waiting=0;
                cv.notify_all();
            }else{
                cv.wait(lock,[this](){return waiting==0;});
            }
        }
};

class uberRideProblem
{
private:
    int republicans=0;
    int democrats=0;
    Semaphore republicans_semaphore = Semaphore(0);
    Semaphore democrates_semaphore = Semaphore(0);
    Barrier barrier = Barrier(4);
    std::mutex m,coutMutex;
    public:
    void seated(std::string party) {
        std::unique_lock<std::mutex>lc(coutMutex);
        std::cout <<party<<" seated with thread id: "<< std::this_thread::get_id() <<std::endl;
    }

    void drive() {
        std::unique_lock<std::mutex>lc(coutMutex);
        std::cout << "Uber Ride on its way... with ride leader " << std::this_thread::get_id() << "\n";
    }
    void republicans_seated(){
        bool rideLeader = false;
        std::unique_lock<std::mutex>lock(m);
        republicans++;
        if(democrats>=2 and republicans==2){
            republicans_semaphore.release(1);
            democrates_semaphore.release(2);
            republicans-=2;
            democrats-=2;
            rideLeader = true;
        }else if(republicans == 4){
            republicans_semaphore.release(3);
            republicans-=4;
            rideLeader = true;
        }else {
            lock.unlock();
            republicans_semaphore.acquire();
        }
        seated("republican");
        barrier.wait();

        if(rideLeader){
            drive();
        }

    }

    void democrats_seated(){
        bool rideLeader = false;
        std::unique_lock<std::mutex>lock(m);
        democrats++;
        if(democrats==2 and republicans>=2){
            republicans_semaphore.release(2);
            democrates_semaphore.release(1);
            republicans-=2;
            democrats-=2;
            rideLeader = true;
        }else if(democrats == 4){
            democrates_semaphore.release(3);
            democrats-=4;
            rideLeader = true;
        }else {
            lock.unlock();
            democrates_semaphore.acquire();
        }
        seated("democrats");
        barrier.wait();

        if(rideLeader){
            drive();
        }

    }
};

int main() {
    uberRideProblem uber;

    std::vector<std::thread> threads;

    for (int i = 0; i < 6; ++i)
        threads.emplace_back(&uberRideProblem::democrats_seated, &uber);

    for (int i = 0; i < 6; ++i)
        threads.emplace_back(&uberRideProblem::republicans_seated, &uber);

    for (auto& t : threads)
        t.join();

    return 0;
}
