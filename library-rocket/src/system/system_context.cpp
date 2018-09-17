/**
 * @file system_context.cpp
 * @author Feo
 * @date 10/08/2018
 * @brief
 */

#include "system/system_loop.h"
#include "system/service/worker_watchdog_service.h"
#include "system/task/worker_broken_task.h"

#include "system/system_context.h"

namespace rocket {

    system_context::system_context(const groot::setting& config, task_router *router) noexcept :
            crucial(config, router, system_loop::make_unique(router->get_queue<system_context>(), this))
    {
        add_service(worker_watchdog_service::make_unique(get_config(), get_router(), this));

        RC_BIND_TASK_ROUTE(worker_broken_task, worker_watchdog_service);
    }

}