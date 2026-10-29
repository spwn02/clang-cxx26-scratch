#include <cstdio>
#include <string>
#include <string_view>
#include <cstring>
#include <text_encoding>

extern "C" __declspec(dllimport) unsigned int __stdcall GetACP(void);

// std::text_encoding::environment() is the encoding of the environment: the ANSI code page of the system, the one that
// GetACP() returns (65001 is UTF-8), not the one of the "C" locale that a program starts with.
int main() {
  const std::text_encoding e = std::text_encoding::environment();
  const unsigned acp         = GetACP();
  char expected[32];
  if (acp == 65001)
    std::strcpy(expected, "UTF-8");
  else
    std::snprintf(expected, sizeof(expected), "windows-%u", acp);
  std::printf("environment = %s, GetACP = %u (expected %s)\n", e.name(), acp, expected);
  return std::string_view(e.name()) == std::string_view(expected) ? 0 : 1;
}
