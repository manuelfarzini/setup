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
namespace mem {

using AllocatorError = i32;
onedef cons i32 Operation_Fail = 1;
onedef cons i32 Invalid_Ptr = 2;
onedef cons i32 Invalid_Arg = 3;
onedef cons i32 Bad_Alloc = 4;
onedef cons i32 Invalid_Mode = 5;


onedef cons u32 ALCTOR_FLAG__ZERO    = BIT<0>;
onedef cons u32 ALCTOR_FLAG__DEFAULT = ALCTOR_FLAG__ZERO;

// =========================================
// Allocator API
// =========================================

#define ALIGNED_ALLOC(name, Alc)                                                     \
    nodisc cons fn name(                                                             \
        Alc&   alc,                      isize   size,                               \
        isize  align     = DEF_ALIGN,    ptrany  old_ptr  = null,                    \
        isize  old_size  = 0,            u32     flags    = ALCTOR_FLAG__DEFAULT     \
    ) noexce -> Res<ptrany, AllocatorError>

#define ALIGNED_RESIZE(name, Alc)                                                    \
    nodisc cons fn name(                                                             \
        Alc&   alc,                                                                  \
        ptrany ptr,       isize  old_size,    isize  new_size,                       \
        isize new_align = DEF_ALIGN,          u32    flags = ALCTOR_FLAG__DEFAULT    \
    ) noexce -> Res<ptrany, AllocatorError>

#define ALIGNED_FREE(name, Alc)                                                      \
    cons fn name(Alc&  alc,    ptrany  ptr) noexce -> void

#define ALIGNED_ALLOC_VIEW(name)                                                     \
    nodisc fn name(                                                                  \
        ptrany data,      isize  size,        isize  align,                          \
        ptrany old_ptr,   isize  old_size,    u32    flags                           \
    ) noexce -> Res<ptrany, AllocatorError>

#define ALIGNED_RESIZE_VIEW(name)                                                    \
    nodisc fn name(                                                                  \
        ptrany data,       ptrany ptr,        isize  old_size,                       \
        isize  new_size,   isize new_align,   u32    flags                           \
    ) noexce -> Res<ptrany, AllocatorError>

#define ALIGNED_FREE_VIEW(name)                                                      \
    fn name(ptrany data,    ptrany ptr) noexce -> void

template<typename Alc>
concept SomeAllocator = requires(
    Alc&    alc,         isize   new_size,        
    isize   align,       ptrany  ptr,
    ptrany  old_ptr,     isize   new_align,
    isize   old_size,    u32     flags
) {
    { aligned_alloc(alc, new_size, align, old_ptr, old_size, flags) }
    noexce -> SameAs<Res<ptrany, AllocatorError>>;

    { aligned_resize(alc, ptr, old_size, new_size, new_align, flags) }
    noexce -> SameAs<Res<ptrany, AllocatorError>>;

    { aligned_free(alc, ptr) } noexce -> SameAs<void>;
};

using AlignedAllocProc = clos( 
    ptrany  data,        isize   size,
    isize   align,       ptrany  old_ptr,
    isize   old_size,    u32     flags
) noexce -> Res<ptrany, AllocatorError>;

using AlignedResizeProc = clos(
    ptrany  data,         ptrany  ptr,
    isize   old_size,     isize   new_size,
    isize   new_align,    u32     flags
) noexce -> Res<ptrany, AllocatorError>;

using AlignedFreeProc = clos(
    ptrany data, ptrany ptr
) noexce -> void;

struct AllocatorView
{
    ptrany              data;
    AlignedAllocProc*   alloc;
    AlignedResizeProc*  resize;
    AlignedFreeProc*    free;
};

ALIGNED_ALLOC(aligned_alloc, AllocatorView)
{
    return alc.alloc(alc.data, size, align, old_ptr, old_size, flags);
}

ALIGNED_RESIZE(aligned_resize, AllocatorView)
{
    return alc.resize(alc.data, ptr, old_size, new_size, new_align, flags);
}

ALIGNED_FREE(aligned_free, AllocatorView)
{
    return alc.free(alc.data, ptr);
}

// Allocator view adapter implementation.

template<SomeAllocator Alc>
ALIGNED_ALLOC_VIEW(aligned_alloc_view)
{
    return aligned_alloc(*cast(Alc*, data), size, align, old_ptr, old_size, flags);
}

template<SomeAllocator Alc>
ALIGNED_RESIZE_VIEW(aligned_resize_vew)
{
    return aligned_resize(*cast(Alc*, data), ptr, old_size, new_size, new_align, flags);
}

template<SomeAllocator Alc>
ALIGNED_FREE_VIEW(aligned_free_view)
{
    return aligned_free(*cast(Alc*, data), ptr);
}

template<SomeAllocator Alc>
nodisc fn allocator_view(Alc& alc) noexce -> AllocatorView
{
    return AllocatorView{
        ptrany(&alc),
        aligned_alloc_view<Alc>,
        aligned_resize_vew<Alc>,
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
}

ALIGNED_ALLOC(aligned_alloc, HeapAllocator) {
    //  NOTE(manu)
    //  should I impose bytes % align == 0 like std::aligned_alloc?

    if (size <= 0) {
        return {null, Invalid_Arg};
    }
    for (; align < DEF_ALIGN ;) {
        align = align << 1;
    }
    assert(is_power_of_two(align) && "`align` must be a power of 2");

    isize space = align - 1 + size + PTR_SIZE;
    b32 force_copy = (old_ptr != null) && (align > PTR_ALIGN);

    ptrany alloced_mem = null;
    if (old_ptr != null && !force_copy) {
        ptrany origin_ptr = diptrany(old_ptr)[-1];
        alloced_mem = heap_resize(origin_ptr, space);
    } else {
        alloced_mem = heap_alloc(space, flags);
    }

    if (alloced_mem == null) {
        aligned_free(alc, old_ptr);
        aligned_free(alc, alloced_mem);
        return {null, Bad_Alloc};
    }

    ptrany aligned_mem = ptr_add(alloced_mem, PTR_SIZE);
    uptr aligned_ptr = (uptr(aligned_mem) + align - 1) & ~(align - 1);
    aligned_mem = ptrany(aligned_ptr);
    (diptrany(aligned_mem))[-1] = alloced_mem;

    if (force_copy) {
        mem_copy(aligned_mem, old_ptr, cx_min2(size, old_size));
        aligned_free(alc, old_ptr);
    }

    return {aligned_mem, none};
}

ALIGNED_RESIZE(aligned_resize, HeapAllocator) {
    if (ptr == null) {
        return aligned_alloc(alc, new_size, new_align, null, old_size, flags);
    }
    auto [new_ptr, err] = aligned_alloc(alc, new_size, new_align, ptr, old_size, flags);
    if (err) {
        return {ptr, err};
    }

    if (flags && new_size > old_size) {
        //  actually using std malloc and realloc, not os specific primitives, so
        //  the new_region should be zeroed in windows too

        ptrany new_region = ptr_add(new_ptr, old_size);

        //  if not dealing with vm paging plain mem_zero is faster than conditional
        mem_zero(new_region, new_size - old_size);
        //  condit_mem_zero(new_region, new_size - old_size);
    }

    return {new_ptr, none};
}


fn heap_allocator() noexce -> HeapAllocator { return HeapAllocator{}; }

// =========================================
// Arena allocator definition
// =========================================

struct ArenaAllocator
{
    u8*      data;
    isize    size;
    isize    used;
};

ALIGNED_FREE(aligned_free, ArenaAllocator)
{
    cx_unreachable();
}

ALIGNED_ALLOC(aligned_alloc, ArenaAllocator)
{
    cx_unreachable();
}
ALIGNED_RESIZE(aligned_resize, ArenaAllocator)
{
    cx_unreachable();
}

static_assert(SomeAllocator<HeapAllocator>);
static_assert(SomeAllocator<ArenaAllocator>);
static_assert(SomeAllocator<AllocatorView>);

}       // namespace mem
}       // namespace cx
#endif  // CX_MEM_ALLOCATOR_HH
