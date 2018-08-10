/**
 * @file application.cpp
 * @author Feo
 * @date 09/08/2018
 * @brief
 */

#include <framework.h>

#include "main/application.h"

namespace engine {

    using namespace framework;

    application::application() :
            _config { config::make_shared() }
    {
        log_manager::setup();
    }

    application::~application()
    {
    }

    void application::load_config(const std::string_view& filename) noexcept
    {
        loginfo("Loading config: %s", filename.data());
        _config->read(filename);
    }

}
