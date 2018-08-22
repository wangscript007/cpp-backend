/**
 * @file websocket_service.h
 * @author Feo
 * @date 18/08/2018
 * @brief
 */

#ifndef ROCKET_WEBSOCKET_SERVICE_H
#define ROCKET_WEBSOCKET_SERVICE_H

#include <libsoup/soup.h>

#include "work/work_service.h"

namespace rocket {

    class websocket_service final : public groot::crucial<work_service, websocket_service> {
    public:
        GR_DECLARE_SMARTPOINTERS(websocket_service)
        GR_DELETE_ALL_DEFAULT(websocket_service)

        explicit websocket_service(const groot::config_setting::sptr& config,
                                   const task_router::sptr& router,
                                   const work_context_delegate *service_provider) noexcept;
        virtual ~websocket_service();

        void setup() noexcept final;
        void reset() noexcept final;

    private:
        void handler(SoupServer *server,
                     SoupWebsocketConnection *connection,
                     const char *path,
                     SoupClientContext *client) noexcept;

        static void handler_routine(SoupServer *server,
                                    SoupWebsocketConnection *connection,
                                    const char *path,
                                    SoupClientContext *client,
                                    gpointer context) noexcept;

    };

}

#endif /* PROJECT_WEBSOCKET_SERVICE_H */
