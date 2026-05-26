/** @file libcx/mem/pointer.hh **/

#ifndef CX_MEM_POINTER_HH
#define CX_MEM_POINTER_HH

// #include <assert.h>

#include "libcx/config.hh"
#include "libcx/traits.hh"
#include "libcx/concepts.hh"
#include "libcx/uti/utilities.hh"

namespace cx {
inline namespace mem {

// /**
//     Prints the full address of `ptr` in hex.
//   
//     @para
//     - `ptr`: the pointer to print
// **/
// onedef fn print_full_ptr(mutaptr ptr) noexce -> void
// {
//     printf("%0*" PRIXPTR "\n", int(size_of(uptr) * 2), uptr(ptr));
// }

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
inln cons fn align_up(mutaptr ptr, isize aln) noexce -> mutaptr
{
    // assert(uti::is_power_of_two(aln));  // TODO: custom assert
    return mutaptr((uptr(ptr) + (aln - 1)) & ~(aln - 1));
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
inln cons fn align_up(mutaptr ptr) noexce -> mutaptr
{
    return align_up(ptr, align_of(Tp));
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
inln cons fn align_up(uptr ptr, isize aln) noexce -> uptr
{
    // assert(is_power_of_two(aln));  // TODO: custom assert
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
template<typename Tp> inln cons fn align_up(uptr ptr) noexce -> uptr
{
    return align_up(ptr, align_of(Tp));
}

/**
  Adds `off` to `ptr` and returns the result.
  
  @para
  - `ptr`: the pointer
  - `off`: the offset
  
  @ret
  - the result pointer
**/  
inln cons fn ptr_add(auto* ptr, isize off) noexce -> auto*
{
    return declt(ptr)(byteptr(ptr) + off);
}

/**
    Subtracts `off` from `ptr` and returns the result.
  
    @para
    - `ptr`: the pointer
    - `off`: the offset
  
    @ret 
    - the result pointer
  
**/
inln cons fn ptr_sub(auto* ptr, isize off) noexce -> auto*
{
    return declt(ptr)(byteptr(ptr) - off);
}

/**
    Computes the difference between `end` and `beg`.
  
    @para
    - `beg`: the beginning pointer
    - `end`: the end pointer
  
    @ret
    - the difference in bytes
  
**/
inln cons fn ptr_diff(auto* beg, auto* end) noexce -> isize
{
    return isize(byteptr(end) - byteptr(beg));
}

/**
    Tags the given pointer with the given `tag`.
  
    @para
    - `ptr`: the pointer
    - `tag`: the tag
   
    @ret
    - the tagged pointer
**/  
template<CRawPointer Ptr> inln cons fn ptr_tag(mutaptr ptr, uptr tag) noexce -> Ptr
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
template<CRawPointer Ptr> inln cons fn ptr_untag(mutaptr ptr, uptr msk) noexce -> Ptr
{
    return Ptr(uptr(ptr) & ~msk);
}

}       // namespace mem
}       // namespace cx
#endif  // CX_MEM_POINTER_HH
