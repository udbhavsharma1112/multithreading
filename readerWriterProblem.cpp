#include<iostream>
#include<thread>
#include<mutex>
#include<chrono>
#include<semaphore>
#include<queue>
#include<stdexcept>
#include<vector>

std::mutex resource_mutex;
int reader_count=0;
std::mutex reader_count_mutex;

void writerWorker(int id){
    
    try{
        std::unique_lock<std::mutex> lock(resource_mutex);
        std::cout<<id<<" is writing"<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::cout<<id<<" writing finished"<<std::endl;
        lock.unlock();
    } catch(const std::exception& e){
        std::cout<<"Got Error in writer "<<e.what()<<std::endl;
    } catch(...){
        std::cout<<"Got Unknown Error in writer"<<std::endl;
    }
}

void readerWorker(int id){
    try{
        {
            std::unique_lock<std::mutex> counter_lock(reader_count_mutex);
            reader_count++;
            if(reader_count==1){
                resource_mutex.lock();
            }
        }
        std::cout<<"Reader "<<id<<" is reading"<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        {
            std::unique_lock<std::mutex> counter_lock(reader_count_mutex);
            std::cout<<"Reader "<<id<<" is finished"<<std::endl;
            reader_count--;
            if(reader_count==0){
                resource_mutex.unlock();
            }
        }
    } catch(const std::exception& e){
        std::cout<<"Got error in reader "<<e.what()<<std::endl;
    } catch(...){
        std::cout<<"Got error unknown in reader "<<std::endl;
    }
}

int main(){

    std::vector<std::thread>reader;
    std::vector<std::thread>writer;

    for(int i=0;i<10;i++){
        reader.push_back(std::thread(readerWorker,i));
        writer.push_back(std::thread(writerWorker,i));
    }
    for(auto &r:reader){
        if(r.joinable()){
            r.join();
        }
    }
    for(auto &w:writer){
        if(w.joinable()){
            w.join();
        }
    }


    
    return 0;
}

