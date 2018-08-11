/**
 * @file engine_const.h
 * @author Feo
 * @date 11/08/2018
 * @brief
 */

#ifndef ENGINE_ENGINE_CONST_H
#define ENGINE_ENGINE_CONST_H

namespace engine {

    class engine_const {
    public:
        static constexpr std::string_view SYSTEM_CONTEXT_NAME { "system" };
        static constexpr std::string_view WEB_SERVER_CONTEXT_NAME { "web-server" };

    };

}

#endif /* ENGINE_ENGINE_CONST_H */