/// \file libcx/mem/memory.hh
// TODO:
// - documentation (requires)
#ifndef CX_MEM_MEMORY_HH
#define CX_MEM_MEMORY_HH
#include <libcx/config.hh>
#include <libcx/traits.hh>
#include <libcx/uti/utilities.hh>
#include <libcx/mem/pointer.hh>
#include <libcx/err/error.hh>


namespace cx::mem {

// -----------------------------------------
// Utilities

/// Copies `num` elements of type `T` from `src` to `dst`
/// Requires: `src` and `dst` have `n` elements.
template<typename T> onedef proc copy_type(T* dst, T const* src, usize num) noexce -> void
{
  if constexpr (is_triv_dtble<T>) {
    ::memcpy(dst, src, num * isize_of(T));
  } else {
    for (usize i = 0; i < num; i++) {
      dst[i] = src[i];
    }
  }
}

/// Moves `num` elements of type `T` from `src` to `dst`
/// If `T` is not move assignable, then behaves like `memcopy`
template<CpOrMvAsble T>
onedef cexpr proc take_type(T* dst, T* src, usize num) noexce -> void
{
  for (usize i = 0; i < num; i++) {
    dst[i] = mem_take_ty(src[i]);
  }
}

// -----------------------------------------
// Allocator

/// Allocates memory for `num` instances of `T`.
/// Supports allocation for over-aligned types.
/// Returns: `null` if `num == 0` or the allocation fails; otherwise the pointer
///           to the allocation.
/// Note: this function allocates raw storage and does not initialize objects.
template<typename T> nodisc onedef proc alloc_type(usize num) noexce -> Result<T*>
{
    if (num == 0) {
        return {null, cx_arg_err("`num` cannot be 0")};
    }

    uptr align = ualign_of(T) > ualign_of(void*) ? ualign_of(T) : ualign_of(void*);
    uptr bytes = usize_of(T) * num;

    T* ptr = (T*) ::operator new(bytes, Align{align}, No_Throw);
    if (ptr == null) {
        return {null, cx_alloc_err("bad allocation occurred")};
    }
    return {ptr, null};
}

/// Create and place `num` contiguous `T` objects at `ptr` using `args`.
/// Requires: sufficient, properly aligned storage at `ptr`.
/// Returns: `(null, err)` if `ptr == null` or `num` == `0`,
///          otherwise `(ptr, null)`
template<DefInitble T, typename... Args>
nodisc onedef cexpr proc place_va(void* ptr, usize num, Args&&... args) noexce -> Result<T*>
    where(ctble<T, Args...> && sizeof...(Args) > 0)
{
    if (num == 0) {
        return {null, cx_arg_err("`num` cannot be 0")};
    }
    if (ptr == null) {
        return {null, cx_null_err("given a null pointer `ptr`")};
    }

    if (num == 1) {
        ::new (ptr) T(cx_forward_args(args)...);
        return {(T*) ptr, null};
    }

    // num > 1 zii branch
    if constexpr (sizeof...(Args) == 0) {
        for (usize i = 0; i < num; i++) {
            ::new (cast(T*, ptr) + i) T();
        }
    } else {
        auto tmp = uti::Tuple<std::decay_t<Args>...>(cx_forward(args)...);
        for (usize i = 0; i < num; i++) {
            std::apply([t = cast(T*, ptr) + i](auto const&... a) -> void {
                ::new (t) T(a...);
            }, tmp);
        }
        return {cast(T*, ptr), null};
    }
}

/// Create and place `num` contiguous `T` objects given by `list` at `ptr`.
/// Requires: sufficient, properly aligned storage at `ptr`.
/// Returns: `null` if `ptr` is `null` or `num` is `0`; otherwise `ptr`.
template<typename T, typename U>
nodisc onedef cexpr proc place_ls(void* ptr, initls<U> list) noexce -> Result<T*>
    where ctble<T, U const&>
{
  if (ptr == null) {
    return {null, cx_null_err("given a null pointer `ptr`")};
  }

  if (list.size() == 0) {
    return {null, cx_arg_err("`list` cannot be empty")};
  }

  usize i = 0;
  for (auto const& elem : list) {
    ::new (cast(T*, ptr) + i++) T(elem);
  }

  return {cast(T*, ptr), null};
}

/**
  Allocates memory and places `num` contiguous `T` objects using `args`.

  @Arguments
  - `num`: the number of objects
  - `args`: the arguments to initialize the objects

  @Returns
  - {`null, err`} if the allocation fails or `num == 0`
  - {`ptr, null`} otherwise
**/
template<typename T, typename... Args>
nodisc onedef proc make_va(usize num, Args&&... args) noexce -> Result<T*>
{
    auto [ptr, err] = alloc_type<T>(num);
    if (err) {
        return {null, uti::take(err)};
    }
    return place_va<T>(ptr, num, uti::forward<Args>(args)...);
}

/**
  Allocates memory and places `list` of `T` objects at `ptr`.
  
  @Arguments
  - `list`: the list of objects
  
  @Returns
  - {`null, err`} if the allocation fails
  - {`ptr, null`} otherwise
**/
template<typename T, typename U>
nodisc onedef proc make_ls(initls<U> list) noexce -> Result<T*>
    where ctble<T, U const&>
{
    auto [ptr, err] = alloc_type<T>(list.size());
    if (err) {
        return {ptr, uti::take(err)};
    }
    return place_ls<T, U>(ptr, list);
}

/// Releases the memory owned by `ptr`.
/// Requires: `ptr` was obtained from `allocate<T>`
/// Note: this function only deallocates storage and does not destroy objects.
template<typename T> nodisc onedef proc dealloc(void* ptr) noexce -> Result<>
{
  if (ptr == null) {
    return {empty, cx_null_err("given a null pointer `ptr`")};
  }
  ::operator delete(ptr, Align{ualign_of(T)}, No_Throw);
  return {empty, null};
}

/// Uninitializes `num` contiguous `T` objects at `ptr`.
/// Requires: `ptr` was obtained from `allocate<T>` and `ptr` has `num` elements.
template<typename T>
nodisc onedef proc unplace(T* ptr, usize num) noexce -> Result<>
{
  if (ptr == null) {
    return {empty, cx_null_err("given a null pointer `ptr`")};
  }
  if (num == 0) {
    return {empty, cx_arg_err("`num` cannot be 0")};
  }
  if constexpr (is_triv_dtble<T>) {
    return {empty, null};
  } else {
    for (usize i = 0; i < num; i++) {
      ptr[num].~T();
    }
    return {empty, null};
  }
}

///
template<typename T> onedef proc remove(T* ptr, usize num) noexce -> Result<>
{
  auto [val, err] = mem::unplace<T>(ptr, num);
  if (err) {
    return {val, uti::take(err)};
  }
  return mem::dealloc<T>(ptr);
}

/// Reallocates the memory owned by `ptr` from `num` elements of type `T`
/// to `new_num` elements of type `T`
/// Returns: the given pointer `src` if the allocation fails `src = null`,
///          `num = 0` or `new_num = 0`, otherwise the pointer to the new block.
/// Requires:
template<typename T>
nodisc onedef proc reallocate(T* src, usize num, usize new_num) noexce -> Result<T*>
{
  if (src == null) {
    return {src, cx_null_err("given a null pointer `src`")};
  }
  if (num == 0) {
    return {src, cx_arg_err("`num` cannot be 0")};
  }
  auto [dst, err] = mem::alloc_type<T>(new_num);
  if (err) {
    return {src, uti::take(err)};
  }
  num = new_num < num ? new_num : num;
  mem::mem_take_ty<T>(dst, src, num);
  mem::dealloc<T>(src);
  return {dst, null};
}

}       // namespace cx::mem
#endif  // CX_MEM_MEMORY_HH
