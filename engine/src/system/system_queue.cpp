/**
 * @file system_event_queue.cpp
 * @author Feo
 * @date 12/08/2018
 * @brief
 */

#include "system/system_queue.h"

namespace engine {

    system_queue::system_queue() :
            task_queue(engine_const::DOMAIN_TASK_QUEUE_SYSTEM),
            _cv {},
            _mutex {}
    {
    }

    // virtual
    void system_queue::enqueue(const task::sptr& task) noexcept
    {
        {
            FW_ULOCK(lock, _mutex);
            push(task);
        }
        _cv.notify_one();
    }

    // virtual
    task::sptr system_queue::dequeue() noexcept
    {
        FW_ULOCK(lock, _mutex);

        _cv.wait(lock, [this]() {
            return !is_empty();
        });

        return pop();
    }

    // virtual
    bool system_queue::empty() const noexcept
    {
        FW_ULOCK(lock, _mutex);
        return is_empty();
    }

}
