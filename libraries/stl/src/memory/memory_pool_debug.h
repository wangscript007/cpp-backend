//
// Created by Feo on 03/09/2018.
//

#ifndef STL_MEMORY_POOL_H
#define STL_MEMORY_POOL_H

#include "memory/memory.h"
#include "scalar/numeric.h"
#include "logger/log_manager.h"

#ifndef NDEBUG
/**
 * Mark pool memory chunk on any action (create pool/alloc chunk/free chunk).
 * Should be used only for useful debugging.
 */
#define STL_TRASHING_MEMORY_POOL  1
/**
 * Wrap pool memory chunk with human readable 16 byte length guards.
 * Should be used only for useful debugging.
 */
#define STL_BOUNDING_MEMORY_POOL  1
#endif // NDEBUG

namespace stl {

    class memory_pool_debug {
    public:
        STL_DELETE_ALL_DEFAULT(memory_pool_debug)

#ifdef STL_TRASHING_MEMORY_POOL
        static constexpr u8 TRASH_ON_CREATE_SIGNATURE { 0xCE };
        static constexpr u8 TRASH_ON_ALLOCATE_SIGNATURE { 0xAE };
        static constexpr u8 TRASH_ON_FREE_SIGNATURE { 0xFE };
#endif

#ifdef STL_BOUNDING_MEMORY_POOL
        static constexpr u8 BOUNDS_CHECK_SIZE { 16 };
        static const u8 START_BOUND[BOUNDS_CHECK_SIZE];
        static const u8 END_BOUND[BOUNDS_CHECK_SIZE];
#endif

    };

}

#endif /* STL_MEMORY_POOL_H */
