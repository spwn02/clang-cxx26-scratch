#include <print>
#include <tuple>

int main() {
  template for (auto x : std::tuple{1, 2.5, 'c'}) {
    std::println("{}", x);
  }
  constexpr int sum = [] {
    int s = 0;
    template for (constexpr int i : {1, 2, 3}) {
      s += i;
    }
    return s;
  }();
  static_assert(sum == 6);
}
