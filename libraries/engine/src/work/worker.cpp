/**
 * @file execution_context.cpp
 * @author Feo
 * @date 09/08/2018
 * @brief
 */

#include "work/worker.h"

namespace engine {

    worker::worker(const stl::setting& config, work_context::uptr&& context) noexcept :
            _thread {},
            _config { config },
            _context { std::move(context) },
            _should_restart { false }
    {
    }

    void worker::start(detach_state state) noexcept
    {
        static const std::unordered_map<detach_state, int> POSIX_DETACH_STATE {
                { detach_state::JOINABLE, PTHREAD_CREATE_JOINABLE },
                { detach_state::DETACHED, PTHREAD_CREATE_DETACHED },
        };

        int status;
        pthread_attr_t attributes;

        status = pthread_attr_init(&attributes);
        logassert(status == 0, "Can't init pthread.");
        status = pthread_attr_setdetachstate(&attributes, POSIX_DETACH_STATE.at(state));
        logassert(status == 0, "Can't set detach state.");
        status = pthread_create(&_thread, &attributes, &worker::exec_callback_fn, this);
        logassert(status == 0, "Can't create pthread.");
    }

    void worker::stop() noexcept
    {
        _should_restart = false;
        _context->stop();
    }

    void worker::restart() noexcept
    {
        _should_restart = true;
        _context->stop();
    }

    void worker::join() noexcept
    {
        int status = pthread_join(_thread, nullptr);
        logassert(status == 0, "Can't join pthread.");
    }

    void worker::on_exec_callback() noexcept
    {
        do {
            _context->setup_services();
            _context->start();
            _context->reset_services();
        } while (_should_restart);
    }

    // static
    void *worker::exec_callback_fn(void *ptr) noexcept
    {
        reinterpret_cast<worker *>(ptr)->on_exec_callback();
        pthread_exit(nullptr);
    }

}
