#include <cstdio>

int divide(const int a, const int b)
  pre(b != 0)
  post(r : r * b <= a)
{
  return a / b;
}

int main(int argc, char **) {
  std::printf("%d\n", divide(7, 2));
  contract_assert(argc >= 1);
  if (argc > 1)
    std::printf("%d\n", divide(1, 0)); // violates the precondition: terminates
}
