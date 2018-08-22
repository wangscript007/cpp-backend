/**
 * @file webserver_service.cpp
 * @author Feo
 * @date 17/08/2018
 * @brief
 */

#include "web/service/webserver_service.h"

namespace rocket {

    webserver_service::webserver_service(const groot::config_setting::sptr& config,
                                         const task_router::sptr& router,
                                         const work_context_delegate *service_provider) noexcept :
            crucial(config, router, service_provider),
            _server { nullptr }
    {
    }

    // virtual
    webserver_service::~webserver_service()
    {
    }

    void webserver_service::setup() noexcept
    {
        GError *error { nullptr };

        const char *header = consts::WEB_SERVER_HEADER.data();
        if (!get_config()->lookup_string("header", &header)) {
            logwarn("Used default web server header: '%s'.", header);
        }

        s32 port = static_cast<s32>(consts::WEB_SERVER_PORT);
        if (!get_config()->lookup_s32("port", &port)) {
            logwarn("Used default web server port: '%d'.", port);
        }

        _server = soup_server_new(SOUP_SERVER_SERVER_HEADER, header, nullptr);
        if (_server != nullptr) {
            gboolean result { FALSE };
            auto listen = (*get_config())[consts::CONFIG_KEY_LISTEN]->to_string_view();
            if (listen == consts::CONFIG_WEBSERVER_LISTEN_ALL) {
                result = soup_server_listen_all(_server,
                                                static_cast<guint>(port),
                                                static_cast<SoupServerListenOptions>(0),
                                                &error);
            } else if (listen == consts::CONFIG_WEBSERVER_LISTEN_LOCAL) {
                result = soup_server_listen_local(_server,
                                                  static_cast<guint>(port),
                                                  static_cast<SoupServerListenOptions>(0),
                                                  &error);
            } else {
                // @todo Implement parse custom address and listen it.
            }

            if (result == TRUE) {
                print_server_info();
                loginfo("Web server started...");
            } else {
                if (error != nullptr) {
                    logerror("%s", error->message);
                }
                logcrit("Failed to start web server.");

                soup_server_disconnect(_server);
                _server = nullptr;
            }
        } else {
            logerror("Failed to create web server.");
        }
    }

    void webserver_service::reset() noexcept
    {
        soup_server_disconnect(_server);
        GR_GOBJECT_UNREF(_server);
        _server = nullptr;
    }

    void webserver_service::print_server_info() const noexcept {
        GSList *uris { soup_server_get_uris(_server) };
        for (GSList *u { uris }; u != nullptr; u = u->next) {
            auto suri { reinterpret_cast<SoupURI *>(u->data) };
            char *str { soup_uri_to_string(suri, FALSE) };
            loginfo("Listening on %s", str);
            g_free(str);
            soup_uri_free(suri);
        }
        g_slist_free(uris);
    }

}
