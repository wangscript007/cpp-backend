//
// Created by Feo on 07/12/2018.
//

#include "context/web/service/http_client_service.h"

namespace engine {

    http_client_service::http_client_service(const stl::setting& config,
                                             task_router *router,
                                             const work_service_delegate *delegate) noexcept :
            crucial(config, router, delegate),
            _session { nullptr },
            _requests {},
            _request_context_pool { stl::fixed_memory_pool::make_unique(sizeof(request_context)) }
    {
        EX_ASSIGN_TASK_HANDLER(http_client_request_task, http_client_service, handle_http_client_request_task);
    }

    // virtual
    http_client_service::~http_client_service()
    {
    }

    // virtual
    void http_client_service::setup() noexcept
    {
        setup_soup_session();
    }

    // virtual
    void http_client_service::reset() noexcept
    {
        reset_soup_session();
    }

    void http_client_service::setup_soup_session() noexcept
    {
        _session = soup_session_new_with_options(SOUP_SESSION_USER_AGENT, "bitpayments",
                                                 NULL);

//#ifndef NDEBUG
//        auto logger { soup_logger_new(SoupLoggerLogLevel::SOUP_LOGGER_LOG_NONE, -1) };
//        soup_session_add_feature(_session, SOUP_SESSION_FEATURE(logger));
//        STL_GOBJECT_RELEASE(logger);
//#endif
    }

    void http_client_service::reset_soup_session() noexcept
    {
        if (_session != nullptr) {
            soup_session_abort(_session);
            STL_GOBJECT_RELEASE(_session);
            _session = nullptr;
        }
    }

    void http_client_service::handle_http_client_request_task(basic_task *base_task) noexcept
    {
        auto task { reinterpret_cast<http_client_request_task *>(base_task) };
        auto request { task->request() };

        auto context { _request_context_pool->construct<request_context>() };
        context->service = this;
        context->callback = std::move(task->grab_callback());

        soup_session_queue_message(_session, request->message(), &http_client_service::handler_callback, context);
    }

    void http_client_service::on_handler(SoupSession *,
                                         SoupMessage *message,
                                         request_context *context) noexcept
    {
        auto response { new (std::nothrow) http_client_response(message) };
        auto task { new (std::nothrow) http_client_response_task(response, std::move(context->callback)) };

        router()->enqueue(task);

        _request_context_pool->destruct(context);
    }

    // static
    void http_client_service::handler_callback(SoupSession *session, SoupMessage *message, gpointer user_data) noexcept
    {
        if (user_data != nullptr) {
            auto context = reinterpret_cast<request_context *>(user_data);
            context->service->on_handler(session, message, context);
        }
    }

}
