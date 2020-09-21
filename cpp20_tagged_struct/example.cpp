#include <iostream>
#include <string>

#include "tagged_struct.h"

int main() {
  tagged_struct<member<"hello", auto_, [] { return 5; }>,
                member<"world", std::string>,
                member<"test", auto_,
                       [](auto& t) {
                         return 2 * get<"hello">(t) + get<"world">(t).size();
                       }>,
                member<"last", int, [] { return 0; }>>
      ts{tag<"hello"> = 1};

  using T = decltype(ts);
  T t2{tag<"world"> = std::string("JRB")};

  auto hello = tag<"hello">;
  std::cout << get<"hello">(ts) << "\n";
  std::cout << get<"world">(ts) << "\n";
  std::cout << get<"test">(ts) << "\n";
  std::cout << get<"hello">(t2) << "\n";
  std::cout << get<"world">(t2) << "\n";
  std::cout << get<"test">(t2) << "\n";
}