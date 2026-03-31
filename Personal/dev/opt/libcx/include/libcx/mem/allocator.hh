#ifndef CX_MEM_ALLOCATOR_HH
#define CX_MEM_ALLOCATOR_HH
#include <libcx/mem/common.hh>
#include <libcx/mem/heap.hh>

namespace cx::mem {

//------------------------------------------
// Common

enum AllocatorMode : u8 {
    Mode_Alloc,
    Mode_Free,
    Mode_Resize,
    Mode_FreeAll,
    // Mode_AllocTyped,
    // Mode_ResizeTyped,
    // Mode_FreeTyped,
};

typedef proc AllocatorProc(
    anyptr           alc_data,
    AllocatorMode    mode,
    isize            size,
    isize            align,
    anyptr           old_ptr,
    isize            old_size,
    b32              zero_mem
) noexce -> Res<anyptr, AllocatorError>;

#define CX_ALLOCATOR_PROC(name)                   \
    onedef proc name(                             \
        anyptr           alc_data,                \
        AllocatorMode    mode,                    \
        isize            size,                    \
        isize            align       = DEF_ALIGN, \
        anyptr           old_ptr     = null,      \
        isize            old_size    = 0,         \
        b32              zero_mem    = true       \
    ) noexce -> Res<anyptr, AllocatorError>

struct Allocator {
    AllocatorProc*    call;
    anyptr            data;
};

onedef proc cx_mem_alloc(
    Allocator alctor, isize size, isize align = DEF_ALIGN, b32 zero_mem = true
) noexce -> Res<anyptr, AllocatorError> {
    if (size == 0) {
        return {null, Invalid_Arg};
    }
    return alctor.call(alctor.data, Mode_Alloc, size, align, null, 0, zero_mem);
}

finline onedef proc cx_mem_free(Allocator alctor, anyptr ptr) -> Res<anyptr, AllocatorError>
{
    return alctor.call(alctor.data, Mode_Free, 0, 0, ptr, 0, true);
}

finline onedef proc cx_mem_resize(
    Allocator    alctor, 
    anyptr       ptr,
    isize        old_size,
    isize        new_size,
    isize        align        = DEF_ALIGN 
) noexce -> Res<anyptr, AllocatorError> {
    if (new_size == 0) {
        if (ptr != null) {
            return alctor.call(alctor.data, Mode_Free, 0, 0, ptr, old_size, true);
        }
        return {null, Invalid_Arg};
    } else if (ptr == null) {
        return alctor.call(alctor.data, Mode_Alloc, new_size, align, null, 0, true);
    } else if (old_size == new_size && uptr(ptr) % uptr(align) == 0) {
        return alctor.call(alctor.data, Mode_Alloc, new_size, align, null, 0, false);
    }
    return alctor.call(alctor.data, Mode_Resize, new_size, align, ptr, old_size, true);
}

finline onedef proc cx_mem_free_all(Allocator alctor) -> Res<anyptr, AllocatorError>
{
    return alctor.call(alctor.data, Mode_FreeAll, 0, 0, null, 0, true);
}

/**
    @par
    - `size`: the size of the memory to be freed [bytes]
**/
onedef proc cx_mem_free_size(
    Allocator alctor, anyptr ptr, isize size
) noexce -> Res<anyptr, AllocatorError> {
    // NOTE: is this proc needed?
    return alctor.call(alctor.data, Mode_Free, 0, 0, ptr, size, true);
}

//------------------------------------------
// Heap

CX_ALLOCATOR_PROC(heap_allocator_proc)
{
    cx_unused(alc_data);

    switch (mode) {
        case Mode_Alloc:
            return heap_aligned_alloc(size, align, old_ptr, old_size, zero_mem);

        case Mode_Free: 
            heap_aligned_free(old_ptr);
            return {null, none};

        case Mode_Resize:
            return heap_aligned_resize(old_ptr, old_size, size, align, zero_mem);

        case Mode_FreeAll:
            return {null, Invalid_Mode};
 
        // case Mode_AllocTyped:
        //     return heap_aligned_alloc(size, align, old_ptr, old_size, false);

        // case Mode_FreeTyped:
        //     heap_aligned_free(old_ptr);
        //     return {null, none};

        // case Mode_ResizeTyped:
        //     return heap_aligned_resize(old_ptr, old_size, size, align, false);

        default:
            // TODO:(manu) Should I use something like cx_panic()?
            cx_unreachable();
    }
}

finline intern cexpr proc heap_allocator() -> Allocator
{
    return Allocator{heap_allocator_proc, null};
}

/*----------------------------------------++
|   Make                                   |
++----------------------------------------*/

/**
    TODO: write the correct concept
**/
template<typename T>
nodisc onedef proc make_array(
    isize num, Allocator alctor = heap_allocator(), isize align = isize_of(T)
) noexce -> Res<T*, ErrorCode> {
    // if constexpr (is_zeroable<T>) {
    //     auto [ptr, err] = cx_alloc(alctor, num * isize_of(T), align);
    //     if (err) {
    //         return {null, err};
    //     }
    //     return {cast(T*, ptr), err};
    //} else {
        auto [ptr, err] = cx_mem_alloc(alctor, num * isize_of(T), align, false);
        if (err) {
            return {null, err};
        }
        init_type<T>(ptr, num);
        return {cast(T*, ptr), none};
    // }
}

template<typename T>
nodisc onedef proc delete_array(
    T* ptr, isize num, Allocator alctor = heap_allocator()
) noexce -> Res<anyptr, ErrorCode> {
    auto [_, err] = deinit_type<T>(ptr, num);
    if (err) {
        return {_, err};
    }
    return cx_mem_free_size(alctor, ptr, num);
}

// #undef proc
// #define proc(name, ...) [[nodiscard]] inline constexpr auto name(__VA_ARGS__) noexcept
// #define meta template
// #define tpar typename
//
// meta<tpar T> proc(dummy, int a, int b) -> int
// {
//     return a + b;
// }
//
// meta<tpar T>
// proc(delete_arr, T* ptr, isize num, Allocator alctor) -> Res<anyptr, ErrorCode>
// {
//     return cx_mem_free_size(alctor, ptr, num);
// }

}       // namespace cx::mem
#endif  // CX_MEM_ALLOCATOR_HH


////////////////////////////////////////////////////////////////
//
//
//
// Unused
//
//

// NOTE:(manu) - old
// We have also the typed version of the allocator modes in
// to value init non primitive types within the `make_type` proc.
// This can be further extended with the different init_type procs.
// Is this is a good idea? Should the non strictly zero init be done
// explicitly caller side? Is more code actually useful?
// Consider that typed versions are useful with containers (arrays, maps, ...)


// /**
//   Allocates memory and places `num` contiguous `T` objects using `args`.
//
//   @Arguments
//   - `num`: the number of objects
//
//   @Returns
//   - `null` if the allocation fails or `num == 0`
//   - `ptr` pointer otherwise
// **/
// template<typename T> nodisc onedef proc heap_make_type(isize num) noexce -> Res<T*, ErrorCode> {
//     auto [ptr, err] = heap_aligned_alloc_type<T>(num);
//     if (err) {
//         return {null, err};
//     }
//     return init_ls<T>(ptr, num);
// }

// /**
//   Deinitializes `num` contiguous `T` objects at `ptr` and frees the memory.
//
//   @Parameters
//   - `ptr`: the pointer to the first element
//   - `num`: the number of elements to deinitialize
//
//   @Returns
//   - `{null, err}` if `ptr` is `null` or `num` is `0`;
//   - `{ptr, none}` otherwise
//
//   @Requires
//   - `ptr` is properly aligned
//   - `ptr` has `num` elements
// **/
// template<typename T>
// onedef proc heap_remove_type(T* ptr, isize num) noexce -> Res<Empty, AllocatorError> {
//     auto [_, err] = deinit_type<T>(ptr, num);
//     if (err) {
//         return {_, uti::take(err)};
//     }
//     return heap_aligned_free(ptr);
// }

// finline onedef proc invoke_allocator(
//     Allocator        alctor, 
//     AllocatorMode    mode,
//     isize            size,
//     isize            align       = Def_Align,
//     anyptr           old_ptr     = null,
//     isize            old_size    = 0,
//     b32              zero_mem    = true
// ) noexce -> Res<anyptr, AllocatorError> {
//     return alctor.invk(mode, alctor.data, size, align, old_ptr, old_size, zero_mem);
// }

