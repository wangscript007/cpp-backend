/**
 * @file execution_service.h
 * @author Feo
 * @date 11/08/2018
 * @brief
 */

#ifndef ENGINE_EXECUTION_SERVICE_H
#define ENGINE_EXECUTION_SERVICE_H

#include <framework.h>

namespace engine {

    class execution_service {
    public:
        FW_DECLARE_SMARTPOINTERS(execution_service)
        FW_DELETE_ALL_DEFAULT_EXCEPT_CTOR(execution_service)
        FW_CRUCIAL_BASE_DEFINITION()

        virtual ~execution_service() = default;

    protected:
        execution_service() = default;

    };



}

#endif /* ENGINE_EXECUTION_SERVICE_H */
