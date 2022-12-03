#pragma once

#include <array>
#include <map>
#include <iostream>

#include <assert.h>

const constexpr int DefaultPreallocSize = 10;

// Memory pool for chunked allocator
// Allocates memory by N-bytes chunks
// does not work in multithreaded environment
template <typename T, size_t N = DefaultPreallocSize>
class MemoryPool
{
public:
    static MemoryPool& GetInstance()
    {
        if (instance)
            return *instance;

        instance = new MemoryPool();
        return *instance;
    }

    using Chunk = std::array<T, N>;

    T* Alloc(size_t n)
    {
        assert (n <= N && "invalid allocation size");
        for (auto& chunk : memoryPool)
        {
            auto& ar = *chunk.first;
            if (chunk.second + n <= N)
            {
                T* ptr = &ar[0] + chunk.second;
                chunk.second += n;
                return ptr;
            }
        }
        return AppendNewChunk(n);
    }

    void Dealloc(T* p)
    {
        auto chunk = std::prev(memoryPool.upper_bound(reinterpret_cast<Chunk*>(p)));
        if (--chunk->second == 0)
        {
            delete chunk->first;
            memoryPool.erase(chunk);
        }
    }

private:

    MemoryPool()
    {
    }

    ~MemoryPool()
    {
        for (auto& chunk : memoryPool)
        {
            delete chunk->first;
        }
    }

    MemoryPool(const MemoryPool&) = delete;
    MemoryPool(MemoryPool&&) = delete;
    MemoryPool& operator = (const MemoryPool&) = delete;
    MemoryPool& operator = (MemoryPool&&) = delete;

    T* AppendNewChunk(size_t n)
    {
        auto newChunk = new Chunk();
        memoryPool.emplace(std::make_pair(newChunk, n));
        auto& ar = *newChunk;
        auto ptr = &ar[0];
        return ptr;
    }

private:
    std::map<Chunk*, size_t> memoryPool;  // Chunk -> countIssued, for quick deallocation

    static MemoryPool* instance;
};

template <typename T, size_t N>
MemoryPool<T, N>* MemoryPool<T, N>::instance = nullptr;

