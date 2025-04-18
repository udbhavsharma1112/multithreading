#include<iostream>
#include<deque>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<chrono>
#include<stdexcept>
#include<semaphore>
#include<string>

// std::deque<int> buffer;
// int buffer_size = 50;
// std::mutex m;
// std::condition_variable cv;

// void producer(int item){
//     try {
//         while(item){
//             std::unique_lock<std::mutex> ul(m);
//             cv.wait(ul, [](){ return buffer.size() < buffer_size; });
//             buffer.push_back(item);
//             std::cout << "item produced: " << item << std::endl;
//             item--;
//             cv.notify_one();
//         }
//     } catch (const std::exception& e) {
//         std::cerr << "Producer encountered an error: " << e.what() << std::endl;
//     } catch (...) {
//         std::cerr << "Producer encountered an unknown error." << std::endl;
//     }
// }

// void consumer(){
//     try {
//         while(true){
//             std::unique_lock<std::mutex> ul(m);
//             cv.wait(ul, [](){ return buffer.size() > 0; });
//             int item = buffer.front();
//             buffer.pop_front();
//             std::cout << "item consumed: " << item << std::endl;
//             cv.notify_one();
//             if(item == 1) break;
//         }
//     } catch (const std::exception& e) {
//         std::cerr << "Consumer encountered an error: " << e.what() << std::endl;
//     } catch (...) {
//         std::cerr << "Consumer encountered an unknown error." << std::endl;
//     }
// }

// int main(){
//     try {
//         std::thread t1(producer, 20);
//         std::thread t2(consumer);
//         t1.join();
//         t2.join();
//     } catch (const std::exception& e) {
//         std::cerr << "Main thread encountered an error: " << e.what() << std::endl;
//     } catch (...) {
//         std::cerr << "Main thread encountered an unknown error." << std::endl;
//     }
//     return 0;
// }


//using semaphore

std::binary_semaphore producer_semaphore{1};
std::counting_semaphore<5> consumer_semaphore{0};
const unsigned int buffer_size=5;
std::deque<int> buffer;

void producer(){
    while(1){
        producer_semaphore.acquire();
        std::cout<<"enter item to produce "<<std::endl;
        int item;
        std::cin>>item;
        buffer.push_back(item);
        std::cout<<"produced item: "<<item<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        consumer_semaphore.release();
    }
}

void consumer(std::string s){
    while(1){
        consumer_semaphore.acquire();
        int item = buffer.front();
        buffer.pop_front();
        std::cout<<"consumed item from "<<s<<": "<<item<<std::endl;
        producer_semaphore.release();
    }
}

int main(){

    std::thread t1(producer);
    std::thread t7(producer);
    std::thread t3(consumer,"2");
    std::thread t2(consumer,"1");
    std::thread t4(consumer,"3");
    std::thread t5(consumer,"4");
    std::thread t6(consumer,"5");
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    return 0;
}