//
// Created by Feo on 03/09/2018.
//

#include "memory/float_memory_pool.h"

#define STL_PAGE_RESERVE_COUNT   16 // @todo: move to config

namespace stl {

    // static
    void float_memory_chunk::write(float_memory_chunk *chunk, void *dst) noexcept
    {
        std::memcpy(dst, chunk, sizeof(float_memory_chunk));
    }

    float_memory_page::float_memory_page(u32 total_size) noexcept :
            _memory { nullptr },
            _total_size { total_size },
            _free_size { total_size - CHUNK_HEAD_SIZE }
    {
        _memory = memory::aligned_alloc<u8>(_total_size);

#ifdef STL_TRASHING_MEMORY_POOL
        std::memset(_memory,
                memory_pool_debug::TRASH_ON_CREATE_SIGNATURE,
                _total_size);
#endif

        // allocate first free block
#ifdef STL_BOUNDING_MEMORY_POOL
        _free_size -= memory_pool_debug::BOUNDS_CHECK_SIZE * 2;

        float_memory_chunk chunk { _total_size - CHUNK_HEAD_SIZE - memory_pool_debug::BOUNDS_CHECK_SIZE * 2 };

        float_memory_chunk::write(&chunk, _memory + memory_pool_debug::BOUNDS_CHECK_SIZE);

        std::memcpy(_memory,
                memory_pool_debug::START_BOUND,
                memory_pool_debug::BOUNDS_CHECK_SIZE);
        std::memcpy(_memory + _total_size - memory_pool_debug::BOUNDS_CHECK_SIZE,
                memory_pool_debug::END_BOUND,
                memory_pool_debug::BOUNDS_CHECK_SIZE);
#else
        float_memory_chunk chunk { _total_size - CHUNK_HEAD_SIZE };
        chunk.write(_memory);
#endif
    }

    //virtual
    float_memory_page::~float_memory_page()
    {
        std::free(_memory);
    }

    void *float_memory_page::alloc(u32 alloc_size) noexcept
    {
        auto required_size { alloc_size + CHUNK_HEAD_SIZE };
        auto memory { _memory };

#ifdef STL_BOUNDING_MEMORY_POOL
        required_size += memory_pool_debug::BOUNDS_CHECK_SIZE * 2;
        memory = _memory + memory_pool_debug::BOUNDS_CHECK_SIZE;
#endif

        // search for a chunk big enough
        auto chunk { reinterpret_cast<float_memory_chunk *>(memory) };
        while (chunk != nullptr) {
            if (chunk->is_free() && chunk->size() >= required_size) {
                break;
            }
            chunk = chunk->next();
        }

        memory = reinterpret_cast<u8 *>(chunk);

        if (chunk != nullptr) {
            // if chunk is valid, create new free chunk with what remains of the chunk memory
            auto free_size { chunk->size() - required_size };
            if (free_size > MIN_FREE_BLOCK_SIZE) {
                float_memory_chunk free_chunk { free_size };
                free_chunk.next(chunk->next());
                free_chunk.prev(chunk);

                float_memory_chunk::write(&free_chunk, memory + required_size);

                if (free_chunk.next() != nullptr) {
                    free_chunk.next()->prev(reinterpret_cast<float_memory_chunk *>(memory + required_size));
                }

#ifdef STL_BOUNDING_MEMORY_POOL
                std::memcpy(memory + required_size - memory_pool_debug::BOUNDS_CHECK_SIZE,
                            memory_pool_debug::START_BOUND,
                            memory_pool_debug::BOUNDS_CHECK_SIZE);
#endif

                chunk->next(reinterpret_cast<float_memory_chunk *>(memory + required_size));
                chunk->size(alloc_size);
            }

            // if chunk is found, update the pool size
            _free_size -= chunk->size();

            chunk->set_free(false);

#ifdef STL_BOUNDING_MEMORY_POOL
            std::memcpy(memory - memory_pool_debug::BOUNDS_CHECK_SIZE,
                        memory_pool_debug::START_BOUND,
                        memory_pool_debug::BOUNDS_CHECK_SIZE);
            std::memcpy(memory + CHUNK_HEAD_SIZE + chunk->size(),
                        memory_pool_debug::END_BOUND,
                        memory_pool_debug::BOUNDS_CHECK_SIZE);
#endif

#ifdef STL_TRASHING_MEMORY_POOL
            std::memset(memory + CHUNK_HEAD_SIZE,
                        memory_pool_debug::TRASH_ON_ALLOCATE_SIGNATURE,
                        chunk->size());
#endif

            memory += CHUNK_HEAD_SIZE;
        }

        return memory;
    }

    void float_memory_page::free(void *ptr) noexcept
    {
        logassert(ptr != nullptr, "Can't free nullptr.");
        if (ptr == nullptr) {
            return;
        }

        auto chunk { reinterpret_cast<float_memory_chunk *>(static_cast<u8 *>(ptr) - CHUNK_HEAD_SIZE) };
        logassert(!chunk->is_free(), "Can't free already freed chunk.");
        if (chunk->is_free()) {
            return;
        }

        u32 full_size = chunk->size() + CHUNK_HEAD_SIZE;
#ifdef STL_BOUNDING_MEMORY_POOL
        full_size += memory_pool_debug::BOUNDS_CHECK_SIZE * 2;
#endif

        _free_size += chunk->size();

        auto head_chunk { chunk };
        auto prev_chunk { chunk->prev() };
        auto next_chunk { chunk->next() };

        // if chunk before is free then merge it with this one
        if (chunk->prev() != nullptr && chunk->prev()->is_free()) {
            head_chunk = chunk->prev();
            prev_chunk = chunk->prev()->prev();
            next_chunk = chunk->next();

            // include previous node in the chunk size so trash it as well
            full_size += chunk->prev()->size() + CHUNK_HEAD_SIZE;
#ifdef STL_BOUNDING_MEMORY_POOL
            full_size += memory_pool_debug::BOUNDS_CHECK_SIZE * 2;
#endif

            // if there is a next one, need to update its pointer
            if (chunk->next()) {
                chunk->next()->prev(head_chunk);

                if (chunk->next()->is_free()) {
                    // point to next's next
                    next_chunk = chunk->next()->next();

                    if (chunk->next()->next()) {
                        chunk->next()->next()->prev(head_chunk);
                    }

                    full_size += chunk->next()->size() + CHUNK_HEAD_SIZE;
#ifdef STL_BOUNDING_MEMORY_POOL
                    full_size += memory_pool_debug::BOUNDS_CHECK_SIZE * 2;
#endif
                }
            }
        } else if (chunk->next() && chunk->next()->is_free()) {
            // if next chunk is free lets merge it to the current one
            head_chunk = chunk;
            prev_chunk = chunk->prev();
            next_chunk = chunk->next()->next();

            // include the next node in the block size so trash it as well
            full_size += chunk->next()->size();
#ifdef STL_BOUNDING_MEMORY_POOL
            full_size += memory_pool_debug::BOUNDS_CHECK_SIZE * 2;
#endif
        }

        auto memory { reinterpret_cast<u8 *>(head_chunk) };

#ifdef STL_TRASHING_MEMORY_POOL
        auto trash_memory { memory };
#ifdef STL_BOUNDING_MEMORY_POOL
        trash_memory -= memory_pool_debug::BOUNDS_CHECK_SIZE;
#endif
        std::memset(trash_memory, memory_pool_debug::TRASH_ON_FREE_SIGNATURE, full_size);
#endif

        auto free_size { full_size - CHUNK_HEAD_SIZE };
#ifdef STL_BOUNDING_MEMORY_POOL
        free_size -= memory_pool_debug::BOUNDS_CHECK_SIZE * 2;
#endif

        float_memory_chunk free_chunk { free_size };
        free_chunk.prev(prev_chunk);
        free_chunk.next(next_chunk);

        float_memory_chunk::write(&free_chunk, memory);

#ifdef STL_BOUNDING_MEMORY_POOL
        std::memcpy(memory - memory_pool_debug::BOUNDS_CHECK_SIZE,
                    memory_pool_debug::START_BOUND,
                    memory_pool_debug::BOUNDS_CHECK_SIZE);
        std::memcpy(memory + CHUNK_HEAD_SIZE + free_size,
                    memory_pool_debug::END_BOUND,
                    memory_pool_debug::BOUNDS_CHECK_SIZE);
#endif
    }

    float_memory_pool::float_memory_pool(size_t total_size) noexcept :
            _pages {},
            _total_size { static_cast<u32>(total_size) }
    {
        _pages.reserve(STL_PAGE_RESERVE_COUNT);
    }

    float_memory_pool::~float_memory_pool()
    {
        for (auto page: _pages) {
            delete page;
        }
    }

    void *float_memory_pool::alloc(size_t size) noexcept
    {
        logassert(0 < size && size < (_total_size / 2), "Invalid alloc size.");

        index_type index { 0 };
        u8 *result { nullptr };
        auto required_size {static_cast<u32>(size + INDEX_SIZE) };

        // Try to find free space in already created pools
        for (size_t i = 0; i < _pages.size(); ++i) {
            result = static_cast<u8 *>(_pages[i]->alloc(required_size));
            if (result != nullptr) {
                index = static_cast<index_type>(i);
                break;
            }
        }

        // If there is no free space, create new pool
        if (result == nullptr) {
            auto page { _pages.emplace_back(new float_memory_page(_total_size)) };

            result = static_cast<u8 *>(page->alloc(required_size));
            index = static_cast<index_type>(_pages.size() - 1);
        }

        // Write pool's index to the chunk
        std::memcpy(result, &index, INDEX_SIZE);

        return result + INDEX_SIZE;
    }

    void float_memory_pool::free(void *ptr) noexcept
    {
        auto memory { static_cast<u8 *>(ptr) - INDEX_SIZE };
        auto index { *reinterpret_cast<index_type *>(memory) };

        logassert(0 <= index && index < _pages.size(), "Can't free chunk with %su pool's index.", index);

        if (index < _pages.size()) {
            _pages[index]->free(memory);
        }
    }

}
