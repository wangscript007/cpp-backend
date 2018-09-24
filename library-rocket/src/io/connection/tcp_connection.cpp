/**
 * @file tcp_connection.cpp
 * @author Feo
 * @date 21/08/2018
 * @brief
 */

#include "io/connection/tcp_connection.h"

namespace rocket {

    tcp_connection::tcp_connection(groot::ip_version version,
                                   groot::connection_side side,
                                   groot::connection_kind kind) noexcept :
            _handle {},
            _version { version },
            _side { side },
            _kind { kind },
            _connect_request {},
            _shutdown_request {}
    {
    }

    void tcp_connection::init(uv_loop_t *loop, void *data) noexcept
    {
        int status = uv_tcp_init(loop, &handle()->tcp);
        logassert(status == 0, "Invalid status.");
        handle()->handle.data = data;
    }

    bool tcp_connection::bind(groot::socket_address *addr) noexcept
    {
        int status = uv_tcp_bind(&handle()->tcp, &addr->sa, 0);
        logassert(status == 0, "Invalid status.");
        return (status == 0);
    }

    bool tcp_connection::listen(u16 backlog, uv_connection_cb cb) noexcept
    {
        int status = uv_listen(&handle()->stream, backlog, cb);
        logassert(status == 0, "Invalid status.");

        return (status == 0);
    }

    bool tcp_connection::accept(tcp_connection *client_connection) noexcept
    {
        int status = uv_accept(&handle()->stream, &client_connection->handle()->stream);
        logassert(status == 0, "Invalid status: %s (%s).", uv_strerror(status), uv_err_name(status));
        return (status == 0);
    }

    bool tcp_connection::start(uv_alloc_cb alloc_cb, uv_read_cb read_cb) noexcept
    {
        int status = uv_read_start(&handle()->stream, alloc_cb, read_cb);
        logassert(status == 0, "Invalid status: %s (%s).", uv_strerror(status), uv_err_name(status));
        return (status == 0);
    }

    uv_connect_t *tcp_connection::connect(groot::socket_address *addr, uv_connect_cb cb) noexcept
    {
        int status = uv_tcp_connect(&_connect_request, &handle()->tcp, &addr->sa, cb);
        if (status != 0) {
            logerror("Failed to connect: %s (%s).", uv_strerror(status), uv_err_name(status));
        } else {
            _connect_request.data = handle()->handle.data;
        }
        return &_connect_request;
    }

    uv_shutdown_t *tcp_connection::shutdown(uv_shutdown_cb cb) noexcept
    {
        int status = uv_shutdown(&_shutdown_request, &handle()->stream, cb);
        if (status != 0) {
            logerror("Failed to shutdown: %s (%s).", uv_strerror(status), uv_err_name(status));
        } else {
            _shutdown_request.data = handle()->handle.data;
        }
        return &_shutdown_request;
    }

    void tcp_connection::set_nodelay(bool enable) noexcept
    {
        int status = uv_tcp_nodelay(&handle()->tcp, enable ? 1 : 0);
        logassert(status == 0, "Invalid status: %s (%s).", uv_strerror(status), uv_err_name(status));
    }

    void tcp_connection::set_nonblock(bool enable) noexcept
    {
        int status = uv_stream_set_blocking(&handle()->stream, enable ? 0 : 1);
        logassert(status == 0, "Invalid status: %s (%s).", uv_strerror(status), uv_err_name(status));
    }

    void tcp_connection::set_keepalive(bool enable, u32 delay) noexcept
    {
        int status = uv_tcp_keepalive(&handle()->tcp, enable ? 1 : 0, delay);
        logassert(status == 0, "Invalid status: %s (%s).", uv_strerror(status), uv_err_name(status));
    }

}
