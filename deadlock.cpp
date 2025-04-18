#include<iostream>
#include<thread>
#include<mutex>
#include<chrono>


std::mutex m1,m2;
void fn1(int x){
    m1.lock();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    m2.lock();
    std::cout<<x<<std::endl;
    m1.unlock();
    m2.unlock();
}
void fn2(int x){
    m2.lock();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    m1.lock();
    std::cout<<x<<std::endl;
    m2.unlock();
    m1.unlock();
}
int main(){
    std::thread t1(fn1,1);
    std::thread t2(fn2,2);
    t1.join();
    t2.join();
    // The program will deadlock here because t1 and t2 are waiting for each other to release the locks
    // This is a classic deadlock situation where two threads are waiting for each other to release the locks
    // To avoid deadlock, we can use std::lock to lock both mutexes at once
    return 0;
}