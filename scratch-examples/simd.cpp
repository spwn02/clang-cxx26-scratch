#include <simd>
#include <print>

namespace dp = std::simd;

int main() {
  constexpr dp::vec<int, 4> a{[](auto i) { return int(i) + 1; }};
  constexpr dp::vec<int, 4> b{10};
  constexpr auto c = a + b;
  static_assert(c[0] == 11 && c[3] == 14);
  std::println("{} {} {} {}", c[0], c[1], c[2], c[3]);
  std::println("sum = {}", dp::reduce(c));
}
