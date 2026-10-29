// Compiled as C++23: receives an exception_ptr from C++26 code and gives one back.
#include <cstdio>
#include <exception>
#include <stdexcept>

std::exception_ptr make_from_cpp26();
bool rethrows_in_cpp26(std::exception_ptr);

int main() {
  std::exception_ptr p = make_from_cpp26();
  std::exception_ptr copy = p;
  bool ok = p && copy == p;
  try {
    std::rethrow_exception(copy);
  } catch (const std::out_of_range&) {
    ok = ok && true;
  }
  ok = ok && rethrows_in_cpp26(std::make_exception_ptr(std::out_of_range("from c++23")));
  std::puts(ok ? "mixed ok" : "mixed FAILED");
  return ok ? 0 : 1;
}
