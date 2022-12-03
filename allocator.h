#include <cstddef>
#include "pool.h"

template <typename T, size_t N = DefaultPreallocSize>
struct ChunkedAllocator
{
	using value_type = T;
	using pointer = T *;
	using const_pointer = const T *;
	using reference = T &;
	using const_reference = const T &;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    template <typename U>
	struct rebind {
		using other = ChunkedAllocator<U, N>;
	};

    ChunkedAllocator() = default;
    ChunkedAllocator(const ChunkedAllocator&) = default;
    template <typename U, size_t S>
    ChunkedAllocator(const ChunkedAllocator<U, S>&) {}
    ~ChunkedAllocator() {}

    ChunkedAllocator& operator = (const ChunkedAllocator& other) = default;
    T * allocate(size_type n, const void* /*hint*/ = 0)
    {
        return MemoryPool<T, N>::GetInstance().Alloc(n);
    }
    void deallocate(pointer p, std::size_t)
    {
        MemoryPool<T, N>::GetInstance().Dealloc(p);
    }

    template <typename U, typename... Args>
    void construct(U* p, Args&&... args)
    {
        new (p) U(std::forward<Args>(args)...);
    }

    template <typename U>
	void destroy(U *p) {
		p->~U();
	}
};

template<size_t N>
struct ChunkedAllocator<void, N>
{
    using pointer = void*;
    using const_pointer = const void*;
    using value_type = void;
    template<typename U>
	struct rebind {
		using other = ChunkedAllocator<U, N>;
	};
};

template<typename T, size_t N, typename U, size_t S>
inline bool operator == (const ChunkedAllocator<T, N>&, const ChunkedAllocator<U, S>&)
{
    return true;
}

template<typename T, size_t N, typename U, size_t S>
inline bool operator != (const ChunkedAllocator<T, N>&, const ChunkedAllocator<U, S>&)
{
    return false;
}

