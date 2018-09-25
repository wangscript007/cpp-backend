//
// Created by Feo on 16/08/2018.
//

#ifndef ROCKET_TASK_QUEUE_H
#define ROCKET_TASK_QUEUE_H

#include "task/basic_task.h"
#include "main/rocket_consts.h"

namespace rocket {

    class task_queue {
    public:
        GR_DECLARE_SMARTPOINTERS(task_queue)
        GR_DELETE_ALL_DEFAULT(task_queue)

        virtual ~task_queue() = default;

        virtual void enqueue(basic_task *task) noexcept = 0;
        virtual basic_task *dequeue() noexcept = 0;

        virtual bool empty() const noexcept = 0;

    protected:
        explicit task_queue(const std::string_view& domain) noexcept;

        void push(basic_task *task) noexcept;
        basic_task *pop() noexcept;

        bool is_empty() const noexcept;

    private:
        std::string_view            _domain;
        std::queue<basic_task *>    _queue;

    };

}

#endif /* ROCKET_TASK_QUEUE_H */
