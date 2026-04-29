/// \file libcx/mem/pointer.hh
#ifndef CX_MEM_POINTER_HH
#define CX_MEM_POINTER_HH
#include <libcx/config.hh>
#include <libcx/traits.hh>
#include <libcx/concepts.hh>
#include <libcx/uti/utilities.hh>
#include <assert.h>

namespace cx::mem {

// /**
//     Prints the full address of `ptr` in hex.
//   
//     @para
//     - `ptr`: the pointer to print
// **/
// onedef proc print_full_ptr(anyptr ptr) noexce -> void
// {
//     printf("%0*" PRIXPTR "\n", int(usize_of(uptr) * 2), uptr(ptr));
// }

/// Predicate that is `true` if `Tp` requires more alignment than `std::max_align_t`.
template<typename Tp> predicate is_over_aligned =  ialign_of(Tp) > ialign_of(max_align_t);

/**
    Aligns `ptr` upward to the `aln` boundary.
  
    @para
    - `ptr`: the pointer
    - `aln`: the alignment
  
    @ret 
    - the aligned pointer
  
    @pre
    - `aln` is a power of two
**/  
finline cexpr proc align_up(anyptr ptr, isize aln) noexce -> anyptr
{
    assert(uti::is_power_of_two(aln));  // TODO: custom assert
    return anyptr((uptr(ptr) + (aln - 1)) & ~(aln - 1));
}

/**
    Aligns `ptr` upward to the alignment of `Tp`.
  
    @para
    - `Tp`: the type to align to
    - `ptr`: the pointer
  
    @ret
    - the aligned pointer
**/
template<typename Tp>
finline cexpr proc align_up(anyptr ptr) noexce -> anyptr
{
    return align_up(ptr, ialign_of(Tp));
}

/**
    Aligns `ptr` upward to the `aln` boundary.
  
    @para
    - `ptr`: the pointer
    - `aln`: the alignment
  
    @ret 
    - the aligned pointer
  
    @pre
    - `aln` is a power of two
**/  
finline cexpr proc align_up(uptr ptr, isize aln) noexce -> uptr
{
    assert(uti::is_power_of_two(aln));  // TODO: custom assert
    return (ptr + (aln - 1)) & ~(aln - 1);
}

/**
  Aligns `ptr` upward to the alignment of `Tp`.
  
  @para
  - `Tp`: the type to align to
  - `ptr`: the pointer
  
  @ret
  - the aligned pointer
**/  
template<typename Tp> finline cexpr proc align_up(uptr ptr) noexce -> uptr
{
    return align_up(ptr, ialign_of(Tp));
}

/**
  Adds `off` to `ptr` and returns the result.
  
  @para
  - `ptr`: the pointer
  - `off`: the offset
  
  @ret
  - the result pointer
**/  
finline cexpr proc ptr_add(auto* ptr, isize off) noexce -> auto*
{
    return declt(ptr)(ptru8(ptr) + off);
}

/**
    Subtracts `off` from `ptr` and returns the result.
  
    @para
    - `ptr`: the pointer
    - `off`: the offset
  
    @ret 
    - the result pointer
  
**/
finline cexpr proc ptr_sub(auto* ptr, isize off) noexce -> auto*
{
    return declt(ptr)(ptru8(ptr) - off);
}

/**
    Computes the difference between `end` and `beg`.
  
    @para
    - `beg`: the beginning pointer
    - `end`: the end pointer
  
    @ret
    - the difference in bytes
  
**/
finline cexpr proc ptr_diff(auto* beg, auto* end) noexce -> isize
{
    return isize(ptru8(end) - ptru8(beg));
}

/**
    Tags the given pointer with the given `tag`.
  
    @para
    - `ptr`: the pointer
    - `tag`: the tag
   
    @ret
    - the tagged pointer
**/  
template<CRawPointer Ptr> finline cexpr proc ptr_tag(anyptr ptr, uptr tag) noexce -> Ptr
{
    return Ptr(uptr(ptr) | tag);
}

/**
    Removes the tag from the given pointer.
  
    @para
    - `ptr`: the pointer
    - `tag`: the tag
   
    @ret
    - the untagged pointer
**/  
template<CRawPointer Ptr> finline cexpr proc ptr_untag(anyptr ptr, uptr msk) noexce -> Ptr
{
    return Ptr(uptr(ptr) & ~msk);
}

};  // namespace cx::mem

#endif
