// Compiled as C++23: the members of exception_ptr that the library used to define out of line.
#include <cstdio>
#include <exception>
#include <stdexcept>
#include <string_view>

int main() {
  std::exception_ptr empty;
  std::exception_ptr p = std::make_exception_ptr(std::runtime_error("boom"));
  std::exception_ptr q = p;
  std::exception_ptr r;
  r = q;
  r = nullptr;
  bool ok = !empty && p && q && !r && p == q && p != r;
  std::swap(r, q);
  ok = ok && r && !q;
  try {
    std::rethrow_exception(r);
  } catch (const std::runtime_error& e) {
    ok = ok && std::string_view(e.what()) == "boom";
  }
  try {
    try {
      throw 7;
    } catch (...) {
      std::exception_ptr c = std::current_exception();
      ok = ok && c;
      std::rethrow_exception(c);
    }
  } catch (int v) {
    ok = ok && v == 7;
  }
  try {
    std::throw_with_nested(std::logic_error("outer"));
  } catch (const std::logic_error& e) {
    ok = ok && std::string_view(e.what()) == "outer";
  }
  std::puts(ok ? "exc23 ok" : "exc23 FAILED");
  return ok ? 0 : 1;
}
