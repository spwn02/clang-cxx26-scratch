#include <cstdio>
#include <string>
#include <string_view>
#include <text_encoding>

extern "C" __declspec(dllimport) unsigned int __stdcall GetACP(void);

// std::text_encoding::environment() is the encoding of the environment: the ANSI code page of the system, the one that
// GetACP() returns (65001 is UTF-8), not the one of the "C" locale that a program starts with.
int main() {
  const std::text_encoding e = std::text_encoding::environment();
  const unsigned acp         = GetACP();
  const std::string expected = acp == 65001 ? "UTF-8" : "windows-" + std::to_string(acp);
  std::printf("environment = %s, GetACP = %u (expected %s)\n", e.name(), acp, expected.c_str());
  return std::string_view(e.name()) == expected ? 0 : 1;
}
