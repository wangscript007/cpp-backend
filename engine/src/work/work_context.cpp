//
// Created by Feo on 16/08/2018.
//

#include "work/work_context.h"

namespace engine {

    FW_CRUCIAL_BASE_DECLARATION(work_context)

    work_context::work_context(const framework::config_setting::sptr& config,
                               const task_router::sptr& router,
                               work_loop::uptr&& loop) noexcept :
            _config { config },
            _router { router },
            _loop { std::move(loop) }
    {
    }

    void work_context::start() noexcept
    {
    }

    void work_context::stop() noexcept
    {
    }

}
