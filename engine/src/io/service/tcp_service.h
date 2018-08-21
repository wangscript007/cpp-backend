/**
 * @file tcp_service.h
 * @author Feo
 * @date 20/08/2018
 * @brief
 */

#ifndef ENGINE_TCP_SERVICE_H
#define ENGINE_TCP_SERVICE_H

#include "work/work_service.h"
#include "io/connection/connection_manager.h"

namespace engine {

    class tcp_service final : public framework::crucial<work_service, tcp_service> {
    public:
        FW_DECLARE_SMARTPOINTERS(tcp_service)
        FW_DELETE_ALL_DEFAULT(tcp_service)

        explicit tcp_service(const framework::config_setting::sptr& config,
                             const task_router::sptr& router,
                             const work_context_delegate *service_provider) noexcept;
        virtual ~tcp_service();

        void setup() noexcept final;
        void reset() noexcept final;

    private:
        uv_loop_t *                 _loop;

        connection_manager::uptr    _connections;

        void listen(const framework::endpoint::sptr& endpoint,
                    u16 backlog,
                    u32 keepalive) noexcept;
        void connect(const framework::endpoint::sptr& endpoint) noexcept;

        void setup_sockaddr(const framework::endpoint::sptr& endpoint, framework::socket_address *addr) noexcept;

        void on_connection(framework::network_handle *handle, int status) noexcept;
        void on_connect(uv_connect_t *request, int status) noexcept;
        void on_shutdown(uv_shutdown_t *request, int status) noexcept;

        static void connection_routine(uv_stream_t *stream, int status) noexcept;
        static void connect_routine(uv_connect_t *request, int status) noexcept;
        static void shutdown_routine(uv_shutdown_t *request, int status) noexcept;

    };

}

#endif /* ENGINE_TCP_SERVICE_H */