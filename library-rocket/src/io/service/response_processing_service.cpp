/**
 * @file serialization_service.cpp
 * @author Feo
 * @date 25/09/2018
 * @brief
 */

#include "io/service/response_processing_service.h"

namespace rocket {

    response_processing_service::response_processing_service(const groot::setting &config,
                                                 task_router *router,
                                                 const work_service_delegate *service_delegate) noexcept :
            crucial(config, router, service_delegate)
    {
    }

    // virtual
    response_processing_service::~response_processing_service()
    {
    }

    void response_processing_service::setup() noexcept
    {

    }

    void response_processing_service::reset() noexcept
    {

    }

}
