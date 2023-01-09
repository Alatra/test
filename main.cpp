#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <functional>
#include <chrono>
#include <condition_variable>


class DataManager {
private:
    std::vector<int> datas = {};
    std::mutex mut;
public:
    DataManager() = default;
    void add(int i) {
        std::lock_guard<std::mutex> lock(mut);
        std::cout<<"Push "<<i<<std::endl;
        datas.push_back(i);
    }

    void applyToData(std::function<void(int)> f){
        std::vector<int> temp;
        {
            std::lock_guard<std::mutex> lock(mut);
            temp = std::move(datas);
        }
        std::cout<<"There was "<<temp.back()<<std::endl;
        for(auto data : temp){
            std::invoke(f,data);
        }
    }

    bool hasNewData()
    {
        return !datas.empty();
    }
};

DataManager man;

bool canWork()
{
    return man.hasNewData();
}

std::condition_variable allowNotify;
std::mutex lk;

void worker()
{
    for(int i = 0; i < 101; i++)
        {
            man.add(i);
            if(i == 25 || i == 44 || i == 88)
            {
                allowNotify.notify_one();
            }
        }
    std::cout<<"End of the worker"<<std::endl;
}


void printer()
{
    std::unique_lock<std::mutex> aa(lk);
    while(true)
        {
            if(!canWork())
            {
                std::cout<<"Go sleep"<<std::endl;
                allowNotify.wait(aa,[](){return canWork();});
                std::cout<<"Wake up"<<std::endl;
            }
            man.applyToData([](int a){std::cout<<a<<std::endl;});
        }
}

int main()
{
    std::thread b(printer);
    std::thread a(worker);
    a.join();
    b.join();
}
