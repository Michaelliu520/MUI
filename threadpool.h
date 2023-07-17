#ifndef THEADPOOL_H
#define THEADPOOL_H
#include<queue>
#include<mutex>
#include<condition_variable>
#include<functional>
#include<thread>
class ThreadPool
{
public:
    explicit ThreadPool(size_t threadCount = 8):pool_(std::make_shared<Pool>()){
        for(int i = 0;i<threadCount;i++){
            std::thread([pool = pool_]{
                std::unique_lock<std::mutex> lock(pool->mtx);
                while(true){
                    if(!pool->tasks.empty()){
                        auto task = std::move(pool->tasks.front());
                        pool->tasks.pop();
                        lock.unlock();
                        task();
                        lock.lock();
                    }
                    else if(pool->isClosed){
                        break;
                    }
                    else{
                        pool->cond.wait(lock);
                    }

                }
            }).detach();
        }
    }

    ThreadPool() = default;
    ThreadPool(ThreadPool&&) = default;

    ~ThreadPool(){
        if(static_cast<bool>(pool_)){
            {
                std::lock_guard<std::mutex> lock(pool_->mtx);
                pool_->isClosed = true;
            }
            pool_->cond.notify_all();
        }
    }
    template<typename F>
    void addTask(F&& task){
        {
            std::lock_guard<std::mutex> lock(pool_->mtx);
            pool_->tasks.emplace(std::forward<F>(task));
        }
        pool_->cond.notify_one();
    }
private:
    struct Pool{
        std::mutex mtx;
        std::condition_variable cond;
        bool isClosed;
        std::queue<std::function<void()>> tasks;
    };
    std::shared_ptr<Pool> pool_;
};

#endif // THEADPOOL_H
