/** @file libcx/uti/decay.hh **/

#ifndef CX_UTI_DECAY_HH
#define CX_UTI_DECAY_HH

#include "libcx/traits.hh"
#include "libcx/uti/sfinae.hh"

namespace cx {
inline namespace uti {

template<typename T>
struct ___decay
{
    using U = rm_ref<T>;
    using Type = 
        typename uti::condition<is_raw_array<U>, typename add_ptr<rm_extent<U>>::Type,
        typename uti::condition<is_func<U>,      typename add_ptr<U>::Type,
                                                          rm_cv<U>
        >
    >;
};

template<typename T> using decay = ___decay<T>::Type;

}      // namespace uti
}      // namespace cx
#endif // CX_UTI_DECAY_HH
