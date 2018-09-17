/**
 * @file system_loop.h
 * @author Feo
 * @date 11/08/2018
 * @brief
 */

#ifndef ROCKET_SYSTEM_LOOP_H
#define ROCKET_SYSTEM_LOOP_H

#include "work/work_loop.h"

namespace rocket {

    class system_loop : public work_loop {
    public:
        GR_DECLARE_SMARTPOINTERS(system_loop)
        GR_DELETE_ALL_DEFAULT(system_loop)

        explicit system_loop(task_queue *queue, const task_handler *handler) noexcept;
        virtual ~system_loop();

        void start() noexcept final;
        void stop() noexcept final;

    private:
        std::atomic_bool        _should_work;

    };

}

#endif /* ROCKET_SYSTEM_LOOP_H */