#include <cmath>
#include <print>
#include <stdfloat>

// constant evaluation of the transcendental functions needs MPFR in the compiler (CLANG_ENABLE_MPFR_CONSTEXPR_MATH)
constexpr double s = std::sin(0.5);
constexpr float e = std::exp(1.0f);
static_assert(s > 0.479 && s < 0.480);
static_assert(e > 2.718f && e < 2.719f);

int main() { std::println("sin(0.5) = {}, exp(1) = {}", s, e); }
