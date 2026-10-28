#include <execution>
#include <print>

namespace ex = std::execution;

int main() {
  auto work = ex::just(20)
            | ex::then([](int x) { return x + 1; })
            | ex::then([](int x) { return x * 2; });
  auto [result] = std::this_thread::sync_wait(std::move(work)).value();
  std::println("result = {}", result);
}
