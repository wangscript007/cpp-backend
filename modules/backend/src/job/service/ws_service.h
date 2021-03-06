/**
 * @file ws_service.h
 * @author Feo
 * @date 23/08/2018
 * @brief
 */

#ifndef BACKEND_WS_SERVICE_H
#define BACKEND_WS_SERVICE_H

#include <engine.h>

namespace backend {

    class ws_service final : public stl::crucial<engine::work_service, ws_service> {
    public:
        STL_DECLARE_SMARTPOINTERS(ws_service)
        STL_DELETE_ALL_DEFAULT(ws_service)

        explicit ws_service(const stl::setting& config,
                            engine::task_router *router,
                            const engine::work_service_delegate *delegate) noexcept;

        virtual ~ws_service();

        void setup() noexcept final;
        void reset() noexcept final;

        void disconnect(SoupWebsocketConnection *connection) noexcept;

    private:
        using message_processor = std::function<void(SoupWebsocketConnection *, const Json::Value&)>;

        std::unordered_map<std::string_view, message_processor>     _processors;

        void handle_ws_request_task(engine::basic_task *base_task) noexcept;
        void handle_ws_connection_status_task(engine::basic_task *base_task) noexcept;

        void process_create_invoice_message(SoupWebsocketConnection *connection, const Json::Value& json) noexcept;

    };

}

#endif /* BACKEND_WS_SERVICE_H */
