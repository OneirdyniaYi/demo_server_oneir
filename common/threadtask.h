#ifndef __THREAD_TASK_H__
#define __THREAD_TASK_H__

#include <cstdint>
#include <mutex>
#include <thread>
#include <memory>
#include <functional>
#include <atomic>
#include <condition_variable>
#include <queue>


#include "noncopyable.h"

namespace demo
{
class ThreadTask : public noncopyable{

using Functor = std::function<void()>;

public:
    ThreadTask();
    ~ThreadTask();
    void start();
    void stop();
    void submit();



private:
    void process();
    mutable std::mutex m_mtx;
    std::condition_variable m_not_empty_cond;
    std::unique_ptr<std::thread> m_thread;
    std::queue<Functor> m_queue;
    std::atomic<int> m_count;
    bool m_running;


};



} // namespace demo





#endif