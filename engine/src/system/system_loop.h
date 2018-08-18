/**
 * @file system_loop.h
 * @author Feo
 * @date 11/08/2018
 * @brief
 */

#ifndef ENGINE_SYSTEM_LOOP_H
#define ENGINE_SYSTEM_LOOP_H

#include "work/work_loop.h"

namespace engine {

    class system_loop : public work_loop {
    public:
        FW_DECLARE_SMARTPOINTERS(system_loop)
        FW_DELETE_ALL_DEFAULT(system_loop)

        explicit system_loop(const task_queue::sptr& queue, task_handler *handler) noexcept;
        virtual ~system_loop();

        void start() noexcept final;
        void stop() noexcept final;

    private:
        std::atomic_bool        _should_work;

    };

}

#endif /* ENGINE_SYSTEM_LOOP_H */
