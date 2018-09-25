/**
 * @file job_loop.cpp
 * @author Feo
 * @date 11/08/2018
 * @brief
 */

#include "job/job_loop.h"

namespace rocket {

    job_loop::job_loop(task_queue *queue, const task_handler *handler) noexcept :
            work_loop(queue, handler),
            _should_work { false }
    {
    }

    // virtual
    job_loop::~job_loop()
    {
    }

    // virtual
    void job_loop::start() noexcept
    {
        _should_work.store(true, std::memory_order_relaxed);
        while (_should_work.load(std::memory_order_relaxed)) {
            auto task = queue()->dequeue();
            handler()->handle_task(task);
        }
    }

    // virtual
    void job_loop::stop() noexcept
    {
        bool expected = true;
        do {} while (!_should_work.compare_exchange_weak(expected, false));
    }

}
