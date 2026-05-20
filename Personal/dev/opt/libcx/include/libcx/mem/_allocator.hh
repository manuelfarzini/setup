/** @file mem/allocator.hh **/

#ifndef CX_MEM_ALLOCATOR_HH
#define CX_MEM_ALLOCATOR_HH

#include <assert.h>

#include "libcx/conf/type.hh"
#include "libcx/conf/prelude.hh"
#include "libcx/traits/types.hh"
#include "libcx/mem/common.hh"
#include "libcx/mem/pointer.hh"

// =========================================
// Prelude
// =========================================

namespace cx {
inline namespace mem {

// onedef cons i32 Operation_Fail = 1;
// onedef cons i32 Invalid_Ptr    = 2;
// onedef cons i32 Invalid_Arg    = 3;
// onedef cons i32 Bad_Alloc      = 4;
// onedef cons i32 Invalid_Mode   = 5;

onedef cons u32 AllocFlags_None    = 0;
onedef cons u32 AllocFlags_Zero    = BIT<0>;
onedef cons u32 AllocFlags_Default = AllocFlags_Zero;

// =========================================
// Allocator contract
// =========================================

template<typename Alc>
concept SomeAllocator = requires(
    Alc&      alc,
    ptrany    old_ptr,
    isize     old_size,
    isize     new_size,
    isize     old_align,
    isize     new_align,
    u32       flags
) {
    { aligned_alloc(alc, new_size, new_align, flags) }
    noexce -> SameAs<Res<ptrany, ErrorCode>>;

    { aligned_resize(alc, old_ptr, new_size, old_size, new_align, old_align, flags) }
    noexce -> SameAs<Res<ptrany, ErrorCode>>;

    { aligned_free(alc, old_ptr) } noexce -> SameAs<ErrorCode>;
};

// =========================================
// Allocator public API
// =========================================

// Static allocator interface.

#define ALIGNED_ALLOC(name, Alc)               \
    cons fn name(                              \
        Alc&     alc,                          \
        isize    size,                         \
        isize    align  =  DEF_ALIGN,          \
        u32      flags  =  AllocFlags_Default  \
    ) noexce -> Res<ptrany, ErrorCode>

#define ALIGNED_RESIZE(name, Alc)                   \
    cons fn name(                                   \
        Alc&      alc,                              \
        ptrany    old_ptr,                          \
        isize     new_size,                         \
        isize     old_size,                         \
        isize     new_align  =  DEF_ALIGN,          \
        isize     old_align  =  DEF_ALIGN,          \
        u32       flags      =  AllocFlags_Default  \
    ) noexce -> Res<ptrany, ErrorCode>

#define ALIGNED_FREE(name, Alc)  \
    cons fn name(                \
        Alc& alc, ptrany ptr     \
    ) noexce -> ErrorCode

// Runtime allocator interface.

#define ALIGNED_ALLOC_VIEW(name)                \
    fn name(                                    \
        ptrany    alc,                          \
        isize     size,                         \
        isize     align  =  DEF_ALIGN,          \
        u32       flags  =  AllocFlags_Default  \
    ) noexce -> Res<ptrany, ErrorCode>

#define ALIGNED_RESIZE_VIEW(name)                   \
    fn name(                                        \
        ptrany    alc,                              \
        ptrany    old_ptr,                          \
        isize     old_size,                         \
        isize     new_size,                         \
        isize     old_align  =  DEF_ALIGN,          \
        isize     new_align  =  DEF_ALIGN,          \
        u32       flags      =  AllocFlags_Default  \
    ) noexce -> Res<ptrany, ErrorCode>

#define ALIGNED_FREE_VIEW(name)  \
    fn name(                     \
        ptrany alc, ptrany ptr   \
    ) noexce -> ErrorCode

// =========================================
// Allocator view runtime adapter
// =========================================

// Allocator view definition.

using AlignedFreeView = func(
    ptrany data, ptrany ptr
) noexce -> ErrorCode;

using AlignedAllocView = func( 
    ptrany    data,
    isize     size,
    isize     align,
    u32       flags
) noexce -> Res<ptrany, ErrorCode>;

using AlignedResizeView = func(
    ptrany    data, 
    ptrany    old_ptr,
    isize     new_size,
    isize     old_size,
    isize     new_align,
    isize     old_align,
    u32       flags
) noexce -> Res<ptrany, ErrorCode>;

struct AllocatorView
{
    ptrany              data;
    AlignedAllocView*   alloc;
    AlignedResizeView*  resize;
    AlignedFreeView*    free;
};

template<SomeAllocator Alc>
ALIGNED_ALLOC_VIEW(aligned_alloc_view)
{
    return aligned_alloc(cast(Alc*, alc)->data, size, align, flags);
}

template<SomeAllocator Alc>
ALIGNED_RESIZE_VIEW(aligned_resize_view)
{
    return aligned_resize(cast(Alc*, alc)->data, old_ptr, new_size, old_size, new_align, old_align, flags);
}

template<SomeAllocator Alc>
ALIGNED_FREE_VIEW(aligned_free_view)
{
    return aligned_free(cast(Alc*, alc)->data, ptr);
}

// Allocator view dispatcher.

ALIGNED_ALLOC(aligned_alloc, AllocatorView)
{
    return alc.alloc(alc.data, size, align, flags);
}

ALIGNED_RESIZE(aligned_resize, AllocatorView)
{
    return alc.resize(alc.data, old_ptr, new_size, old_size, new_align, old_align, flags);
}

ALIGNED_FREE(aligned_free, AllocatorView)
{
    return alc.free(alc.data, ptr);
}

template<SomeAllocator Alc>
nodisc fn allocator_view(Alc& alc) noexce -> AllocatorView
{
    return AllocatorView{
        ptrany(&alc),
        aligned_alloc_view<Alc>,
        aligned_resize_view<Alc>,
        aligned_free_view<Alc>
    };
}

// =========================================
// Heap allocator definition
// =========================================

cons fn heap_alloc(isize size, b32 zero_mem = true) noexce -> ptrany
{
    if (size <= 0) {
        return null;
    }

    if (zero_mem) {
        return ::calloc(1, size);
    } else {
        return ::malloc(size);
    }
}

cons fn heap_resize(ptrany ptr, isize new_size) noexce -> ptrany
{
    return ::realloc(ptr, new_size);
}

cons fn heap_free(ptrany ptr) noexce -> void
{
    return ::free(ptr);
}

struct HeapAllocator {};

ALIGNED_FREE(aligned_free, HeapAllocator)
{
    cx_unused(alc);
    if (ptr != null) {
        ::free(diptrany(ptr)[-1]);
    }
    return Error_None;
}

ALIGNED_ALLOC(aligned_alloc, HeapAllocator) {
    // NOTE(manu)
    // should I impose bytes % align == 0 like std::aligned_alloc?

    cx_unused(alc);
    // TODO:(what if size < 0, can I implement some logic?)
    if (size <= 0) {
        return {null, Invalid_Arg};
    }
    if (align < DEF_ALIGN) {
        align = DEF_ALIGN;
    }
    assert(is_power_of_two(align) && "`align` must be a power of 2");

    isize space = PTR_SIZE + (align - 1) + size;
    ptrany alloced_mem = heap_alloc(space, flags);
    if (alloced_mem == null) {
        return {null, Bad_Alloc};
    }

    ptrany aligned_mem = align_up(ptr_add(alloced_mem, PTR_SIZE), align);
    (diptrany(aligned_mem))[-1] = alloced_mem;

    if (flags & AllocFlags_Zero) {
        mem_zero(aligned_mem, size);
    }

    return {aligned_mem, none};
}


/**
    @ret
    - `[new_ptr, none]` on success
    - otherwise `panic`
**/
ALIGNED_RESIZE(aligned_resize, HeapAllocator) {
    if (new_size <= 0) {
        aligned_free(alc, old_ptr);
        return {null, null};
    }

    if (old_ptr == null) {
        return aligned_alloc(alc, new_size, new_align, flags);
    }

    if (new_align < DEF_ALIGN) {
        new_align = DEF_ALIGN;
    }
    assert(is_power_of_two(new_align) && "`new_align` must be a power of 2");

    ptrany alloced_mem = null;
    ptrany aligned_mem = null;
    b32 force_copy = old_align > DEF_ALIGN || new_align > DEF_ALIGN;
    isize space = PTR_SIZE + (new_align - 1) + new_size;

    if (force_copy) {
        alloced_mem = heap_alloc(space, AllocFlags_None);
        // TODO:(manu) Panic or propagate error (mod base procedures accordingly)?
        assert(alloced_mem);
        aligned_mem = align_up(ptr_add(alloced_mem, PTR_SIZE), new_align);
        (diptrany(aligned_mem))[-1] = alloced_mem;
        mem_copy(aligned_mem, old_ptr, cx_min2(old_size, new_size));
        aligned_free(alc, old_ptr);

    } else {
        ptrany base_ptr = diptrany(old_ptr)[-1];
        alloced_mem = heap_resize(base_ptr, space);
        assert(alloced_mem);
        aligned_mem = align_up(ptr_add(alloced_mem, PTR_SIZE), new_align);
        (diptrany(aligned_mem))[-1] = alloced_mem;
    }

    if ((flags & AllocFlags_Zero) && (new_size > old_size)) {
        // actually using std malloc and realloc, not os specific primitives, so
        // the new_region should be zeroed in windows too
        ptrany new_region = ptr_add(aligned_mem, old_size);
        // if not dealing with vm paging plain mem_zero is faster than conditional
        mem_zero(new_region, new_size - old_size);
    }

    return {aligned_mem, none};
}


glob HeapAllocator HEAP_ALLOCATOR{};
comp fn heap_allocator() noexce -> HeapAllocator& { return HEAP_ALLOCATOR; }

// =========================================
// Arena allocator definition
// =========================================

struct ArenaAllocator
{
    ptru8    data      {};
    isize    data_cap  {};
    isize    curr_off  {};
    isize    prev_off  {};
};

fn arena_init(ArenaAllocator& alc, isize cap) noexce -> void
{
    alc = ArenaAllocator{ptru8(heap_alloc(cap)), cap, 0, 0};
}

ALIGNED_FREE(aligned_free, ArenaAllocator)
{
    cx_unused(alc);
    cx_unused(ptr);
    return Invalid_Mode;
}

// NOTE(manu) Actually super basic version, should implement virtual mem allocator too.
ALIGNED_ALLOC(aligned_alloc, ArenaAllocator)
{
    ptru8 old_ptr = ptr_add(alc.data, alc.curr_off);
    isize off = align_up(isize(old_ptr), align) - isize(alc.data);
    if (off + size <= alc.data_cap) {
        ptrany ptr = ptr_add(alc.data, off);
        alc.prev_off = off;
        alc.curr_off = off + size;
        if (flags & AllocFlags_Zero) {
            mem_zero(ptr, size);
        }
        return {ptr, none};
    }
    return {null, Bad_Alloc};
}

static_assert(SomeAllocator<HeapAllocator>);
// static_assert(SomeAllocator<ArenaAllocator>);
// static_assert(SomeAllocator<AllocatorView>);

}       // namespace mem
}       // namespace cx
#endif  // CX_MEM_ALLOCATOR_HH
