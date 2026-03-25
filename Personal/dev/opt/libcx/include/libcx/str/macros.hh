/// \file libcx/str/macro.hh
#ifndef CX_STR_MACRO_HH
#define CX_STR_MACRO_HH
#include <libcx/config.hh>

#define CX_ZS(...) cx::str::zs_create(__VA_ARGS__)
#define CX_ZS_FILE CX_ZS(CX_FILE)
#define CX_ZS_SHORT_FILE cx::str::zs_short_fpath<CX_ZS_FILE>()
#define CX_ZS_LINE CX_ZS(STR_(CX_LINE))
#define CX_ZS_FUNCTION CX_ZS(CX_FUNCTION)

#endif  // CX_STR_MACRO_HH
