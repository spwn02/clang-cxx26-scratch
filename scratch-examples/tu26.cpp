// Compiled as C++26: hands an exception_ptr to code compiled as C++23.
#include <exception>
#include <stdexcept>

std::exception_ptr make_from_cpp26() { return std::make_exception_ptr(std::out_of_range("from c++26")); }
bool rethrows_in_cpp26(std::exception_ptr p) {
  try {
    std::rethrow_exception(p);
  } catch (const std::out_of_range&) {
    return true;
  }
  return false;
}
