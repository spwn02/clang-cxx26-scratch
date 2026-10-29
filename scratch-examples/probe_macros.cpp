#include <cstdio>
#include <exception>
#define S2(x) #x
#define S(x) S2(x)
int main() {
#if __has_builtin(__builtin_constexpr_exception_capture)
  std::puts("probe: builtin capture yes");
#else
  std::puts("probe: builtin capture NO");
#endif
#if __has_builtin(__builtin_constexpr_exception_retain) && __has_builtin(__builtin_constexpr_exception_release) && __has_builtin(__builtin_constexpr_exception_rethrow)
  std::puts("probe: other builtins yes");
#else
  std::puts("probe: other builtins NO");
#endif
  std::printf("probe: _LIBCPP_STD_VER=%s _LIBCPP_HAS_EXCEPTIONS=%s __cplusplus=%ld\n", S(_LIBCPP_STD_VER), S(_LIBCPP_HAS_EXCEPTIONS), (long)__cplusplus);
#ifdef _LIBCPP_BUILDING_LIBRARY
  std::puts("probe: BUILDING_LIBRARY defined");
#endif
#ifdef _LIBCPP_ABI_MICROSOFT
  std::puts("probe: ABI_MICROSOFT");
#endif
}
