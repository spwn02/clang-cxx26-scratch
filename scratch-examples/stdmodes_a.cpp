// Linked with stdmodes_b.cpp, each in its own standard mode: the functions that are inline on the MSVC ABI
// must link and agree in every mode.
#include <memory>
#include <string>
#include <cstring>
#include <cstdio>
#if __cplusplus >= 201703L
#  include <optional>
#  include <variant>
#endif
std::string second();
int main() {
  bool ok = std::to_string(-12) == "-12" && std::to_string(4294967295u) == "4294967295" && second() == "42x";
#ifndef _LIBCPP_HAS_NO_WIDE_CHARACTERS
  ok = ok && std::to_wstring(7L) == L"7";
#endif
  ok = ok && std::strcmp(std::bad_weak_ptr().what(), "bad_weak_ptr") == 0;
#if __cplusplus >= 201703L
  ok = ok && std::strcmp(std::bad_optional_access().what(), "bad_optional_access") == 0;
  ok = ok && std::strcmp(std::bad_variant_access().what(), "bad_variant_access") == 0;
#endif
  std::puts(ok ? "stdmodes ok" : "stdmodes WRONG");
  return ok ? 0 : 1;
}
