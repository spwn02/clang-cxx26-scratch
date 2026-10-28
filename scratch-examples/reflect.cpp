#include <meta>
#include <print>

struct Point {
  int x;
  double y;
};

int main() {
  template for (constexpr auto member :
                std::define_static_array(std::meta::nonstatic_data_members_of(
                    ^^Point, std::meta::access_context::unchecked()))) {
    std::println("{}: {}", std::meta::identifier_of(member),
                 std::meta::display_string_of(std::meta::type_of(member)));
  }
}
