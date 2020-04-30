// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>

#include "../simple_type_name/simple_type_name.h"
#include "tagged_tuple.h"

int main() {
  using skydown::tagged_tuple;
  using skydown::tag;
  using skydown::make_member;
  using skydown::has_tag;
  using skydown::remove_tag;

  auto t = tagged_tuple{tag<class A> = 1, tag<class B> = 2.0};

  std::cout << "B = " << tag<B>(t) << "\n";

  for_each(t, [](auto& t) { std::cout << t.value << "\n"; });

  auto nt = append(t, make_member<class C>(3));
  for_each(nt, [](auto& t) {
    std::cout << "\n" << t.tag_name << ": " << t.value;
  });

  static_assert(tuple_size(nt) == 3);

  static_assert(has_tag<C, decltype(nt)>);

  {
    auto t = tagged_tuple{tag<class A> = 5,
                          make_member<class V>(std::string("value"))};
    auto t2 = tagged_tuple{make_member<class Z>(std::string("zvalue")),
                           make_member<class A>(6)};
    auto t3 = merge(t, t2);

    std::cout << "\n" << t3 << "\n";
    auto t4 = tagged_tuple{make_member<class X>(t)};
    auto t5 = tagged_tuple{make_member<class X>(t2)};
    auto t6 = merge(t4, t5);
    std::cout << "\n" << t6 << "\n";
    std::cout << merge(t6, tagged_tuple{make_member<X>(
                               tagged_tuple{remove_tag<Z>})})
              << "\n";
  }
}
