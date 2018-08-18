/**
 * @file job_event_queue.cpp
 * @author Feo
 * @date 12/08/2018
 * @brief
 */

#include "job/job_queue.h"

namespace engine {

    job_queue::job_queue() :
            task_queue(engine_const::DOMAIN_TASK_QUEUE_JOB),
            _cv {},
            _mutex {}
    {
    }

    // virtual
    void job_queue::enqueue(const task::sptr& task) noexcept
    {
        {
            FW_ULOCK(lock, _mutex);
            push(task);
        }
        _cv.notify_one();
    }

    // virtual
    task::sptr job_queue::dequeue() noexcept
    {
        FW_ULOCK(lock, _mutex);

        _cv.wait(lock, [this]() {
            return !is_empty();
        });

        return pop();
    }

    // virtual
    bool job_queue::empty() const noexcept
    {
        FW_ULOCK(lock, _mutex);
        return is_empty();
    }

}
