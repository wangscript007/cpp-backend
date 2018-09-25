/**
 * @file task_handler.h
 * @author Feo
 * @date 19/08/2018
 * @brief
 */

#ifndef ROCKET_TASK_HANDLER_H
#define ROCKET_TASK_HANDLER_H

#include "task/basic_task.h"

namespace rocket {

    /**
     * Interface which designed to encapsulate task handling logic for `work_context`.
     */
    class task_handler {
    public:
        GR_DELETE_ALL_DEFAULT_EXCEPT_CTOR(task_handler)

        virtual ~task_handler() = default;

        virtual void handle_task(basic_task *task) const noexcept = 0;

    protected:
        task_handler() = default;

    };

}

#endif /* ROCKET_TASK_HANDLER_H */
