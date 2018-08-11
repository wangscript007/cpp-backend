/**
 * @file web_server_loop.h
 * @author Feo
 * @date 11/08/2018
 * @brief
 */

#ifndef ENGINE_WEB_SERVER_LOOP_H
#define ENGINE_WEB_SERVER_LOOP_H

#include <glib/gstdio.h>

#include "core/execution_loop.h"

namespace engine {

    class web_server_loop : public execution_loop {
    public:
        FW_DECLARE_SMARTPOINTERS(web_server_loop)
        FW_DELETE_ALL_DEFAULT_EXCEPT_CTOR(web_server_loop)

        web_server_loop();
        virtual ~web_server_loop();

        void run() noexcept final;
        void stop() noexcept final;

        bool stopped() const noexcept final;

    private:
        GMainLoop *         _loop;

    };

}

#endif /* ENGINE_WEB_SERVER_LOOP_H */