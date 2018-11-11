/**
 * @file http_request_task.h
 * @author Feo
 * @date 19/08/2018
 * @brief
 */

#ifndef ENGINE_HTTP_REQUEST_TASK_H
#define ENGINE_HTTP_REQUEST_TASK_H

#include "task/basic_task.h"
#include "context/web/http/http_request.h"

namespace engine {

    class http_request_task : public stl::crucial<basic_task, http_request_task> {
    public:
        STL_DECLARE_SMARTPOINTERS(http_request_task)
        STL_DELETE_ALL_DEFAULT_EXCEPT_CTOR(http_request_task)

        explicit http_request_task(const http_request::sptr& request) noexcept : _request { request } {}
        virtual ~http_request_task() = default;

        http_request::sptr get_request() const noexcept { return _request; }

    private:
        http_request::sptr      _request;

    };

}

#endif /* ENGINE_HTTP_REQUEST_TASK_H */