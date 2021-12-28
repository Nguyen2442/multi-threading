/*
MY THREAD POOL

Version 1:
- Simple thread pool.
- Method "waitTaskDone" consumes CPU (due to bad synchronization).
*/



#ifndef _MY_THREAD_POOL_V1_HPP_
#define _MY_THREAD_POOL_V1_HPP_



#include <vector>
#include <queue>
#include <atomic>
#include <pthread.h>



// interface ITask
class ITask {
public:
    virtual ~ITask() = default;
    virtual void run() = 0;
};



class ThreadPoolV1 {

private:
    int numThreads = 0;
    std::vector<pthread_t> lstTh;

    std::queue<ITask*> taskPending;
    pthread_mutex_t mutTaskPending = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t condTaskPending = PTHREAD_COND_INITIALIZER;

    std::atomic_int32_t counterTaskRunning;

    volatile bool forceThreadShutdown;


public:
    ThreadPoolV1() = default;
    ThreadPoolV1(const ThreadPoolV1& other) = delete;
    ThreadPoolV1(const ThreadPoolV1&& other) = delete;
    void operator=(const ThreadPoolV1& other) = delete;
    void operator=(const ThreadPoolV1&& other) = delete;


    void init(int numThreads) {
        shutdown();

        mutTaskPending = PTHREAD_MUTEX_INITIALIZER;
        condTaskPending = PTHREAD_COND_INITIALIZER;

        this->numThreads = numThreads;
        lstTh.resize(numThreads);
        counterTaskRunning.store(0);
        forceThreadShutdown = false;

        for (auto&& th : lstTh) {
            pthread_create(&th, nullptr, threadRoutine, this);
        }
    }


    void submit(ITask* task) {
        pthread_mutex_lock(&mutTaskPending);
        taskPending.push(task);
        pthread_mutex_unlock(&mutTaskPending);

        pthread_cond_signal(&condTaskPending);
    }


    void waitTaskDone() {
        bool done = false;

        for (;;) {
            pthread_mutex_lock(&mutTaskPending);

            if (0 == taskPending.size() && 0 == counterTaskRunning.load()) {
                done = true;
            }

            pthread_mutex_unlock(&mutTaskPending);

            if (done) {
                break;
            }

            pthread_yield();
        }
    }


    void shutdown() {
        pthread_mutex_lock(&mutTaskPending);

        forceThreadShutdown = true;
        std::queue<ITask*>().swap(taskPending);

        pthread_mutex_unlock(&mutTaskPending);

        pthread_cond_broadcast(&condTaskPending);

        for (auto&& th : lstTh) {
            pthread_join(th, nullptr);
        }

        numThreads = 0;
        lstTh.clear();

        pthread_mutex_destroy(&mutTaskPending);
        pthread_cond_destroy(&condTaskPending);
    }


private:
    static void* threadRoutine(void* argVoid) {
        auto thisPtr = (ThreadPoolV1*)argVoid;

        auto&& taskPending = thisPtr->taskPending;
        auto&& mutTaskPending = thisPtr->mutTaskPending;
        auto&& condTaskPending = thisPtr->condTaskPending;

        auto&& counterTaskRunning = thisPtr->counterTaskRunning;
        auto&& forceThreadShutdown = thisPtr->forceThreadShutdown;


        for (;;) {
            // WAIT FOR AN AVAILABLE PENDING TASK
            pthread_mutex_lock(&mutTaskPending);

            while (0 == taskPending.size() and false == forceThreadShutdown) {
                pthread_cond_wait(&condTaskPending, &mutTaskPending);
            }

            if (forceThreadShutdown) {
                pthread_mutex_unlock(&mutTaskPending);
                break;
            }

            // GET THE TASK FROM THE PENDING QUEUE
            auto task = taskPending.front();
            taskPending.pop();

            ++counterTaskRunning;

            pthread_mutex_unlock(&mutTaskPending);

            // DO THE TASK
            task->run();

            --counterTaskRunning;
        }

        pthread_exit(nullptr);
        return nullptr;
    }

};



#endif // _MY_THREAD_POOL_V1_HPP_