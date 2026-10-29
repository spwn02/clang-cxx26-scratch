#include <exception>
#include <print>

constexpr int round_trip() {
  std::exception_ptr p = std::make_exception_ptr(42);
  std::exception_ptr q = p;
  try {
    std::rethrow_exception(q);
  } catch (int v) {
    return v;
  }
  return 0;
}
static_assert(round_trip() == 42);

int main() { std::println("round trip = {}", round_trip()); }
