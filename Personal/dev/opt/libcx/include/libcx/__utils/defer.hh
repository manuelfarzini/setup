/// \file libcx/uti/defer.hh
#ifndef CX_UTI_DEFER_HH
#define CX_UTI_DEFER_HH
#include <libcx/config.hh>
#include <libcx/__utils/ownership.hh>

namespace cx {
inline namespace uti {

template<typename F> struct DeferWrapper {
  F f;
  finline cexpr DeferWrapper(F&& f) : f(uti::forward<F>(f)) {}
  finline cexpr ~DeferWrapper() { f(); };
};

template<typename F>
finline cexpr proc defer_creator(F&& f) noexce -> DeferWrapper<F>
{
  return DeferWrapper<F>(uti::forward<F>(f));
}

#ifndef defer
  #define defer(_BODY_) \
    proc CX_UNIQ(_defer_) = cx::defer_creator([&]() finline_clos { _BODY_; })
#endif


#ifndef cx_defer_loop
#define cx_defer_loop(_cx_dl_init_, _cx_dl_clean_)           \
  for (u8 _cx_dl_stop_ = 0, _cx_dl_once_ = 1;                \
       _cx_dl_once_;                                         \
       CX_NOTHING)                                           \
    for (_cx_dl_init_;                                       \
         !_cx_dl_stop_;                                      \
         _cx_dl_stop_ ? (_cx_dl_clean_, _cx_dl_once_ = 0, 1) \
                        : (_cx_dl_once_ = 0, 0))
#endif

#ifndef cx_defer_loop_stop
#define cx_defer_loop_stop() _o_dl_stop_ = 1
#endif

}       // namespace cx
}       // namespace uti
#endif  // CX_UTI_DEFER_HH

/*---------------------------------------------------
  Example
 
proc test() -> void
{
    cx_defer_loop (auto x = 0, printf("cleanup\n")) {
        x += 1;
        printf("inside");
        if (x == 3) {
            cx_defer_loop_stop();
        }
    }
}

proc test() -> void
{
    cx_defer_loop (FILE* f = fopen("test.txt", "r"), fclose(f)) {
        auto c = fgetc(f);
        printf("inside");
        if (c == '#') {
            cx_defer_loop_stop();
        }
    }
}

*/
