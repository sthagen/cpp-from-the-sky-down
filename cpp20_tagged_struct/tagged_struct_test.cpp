
#include "tagged_struct.h"

#include <gtest/gtest.h>

namespace ftsd {
namespace {

TEST(TaggedStruct, Construction) {
  tagged_struct<member<"hello", auto_, [] { return 5; }>,
                member<"world", std::string, [] { return "world"; }>,
                member<"test", auto_,
                       [](auto& t) {
                         return 2 * get<"hello">(t) + get<"world">(t).size();
                       }>,
                member<"last", int>>
      ts{tag<"hello"> = 1};

  EXPECT_EQ(get<"hello">(ts), 1);
  EXPECT_EQ(get<"world">(ts), "world");
  EXPECT_EQ(get<"test">(ts), 7);
  EXPECT_EQ(get<"last">(ts), 0);
}

TEST(TaggedStruct, ConstructionUdl) {
  using namespace ftsd::literals;

  tagged_struct<member<"hello", auto_, [] { return 5; }>,
                member<"world", std::string,
                       [](auto& self) { return get<"hello">(self); }>,
                member<"test", auto_,
                       [](auto& t) {
                         return 2 * get<"hello">(t) + get<"world">(t).size();
                       }>,
                member<"last", int>>
      ts{"world"_tag = "Universe", "hello"_tag = 1};

  EXPECT_EQ(ts["hello"_tag], 1);
  EXPECT_EQ(ts["world"_tag], "Universe");
}

TEST(TaggedStruct, Ctad) {
  using namespace literals;
  tagged_struct ctad{tag<"a"> = 15, "b"_tag = std::string("Hello ctad")};
  EXPECT_EQ(get<"a">(ctad), 15);
  EXPECT_EQ(get<"b">(ctad), "Hello ctad");
}

TEST(TaggedStruct, ConstAccess) {
  using namespace literals;
  const tagged_struct ctad{tag<"a"> = 15, "b"_tag = std::string("Hello ctad")};
  EXPECT_EQ(get<"a">(ctad), 15);
  EXPECT_EQ(get<"b">(ctad), "Hello ctad");
}

TEST(TaggedStruct, MutableAccess) {
  using namespace literals;
  tagged_struct ctad{tag<"a"> = 15, "b"_tag = std::string("Hello ctad")};
  EXPECT_EQ(get<"a">(ctad), 15);
  EXPECT_EQ(get<"b">(ctad), "Hello ctad");
  get<"a">(ctad) = 10;
  EXPECT_EQ(get<"a">(ctad), 10);
  EXPECT_EQ(get<"b">(ctad), "Hello ctad");
}

TEST(TaggedStruct, NamedArguments) {
  using namespace literals;
  using test_arguments =
      tagged_struct<member<"a", int, [] {}>,  // Required argument.
                    member<"b", auto_, [](auto& t) { return get<"a">(t) + 2; }>,
                    member<"c", auto_, [](auto& t) { return get<"b">(t) + 2; }>,
                    member<"d", auto_, []() { return 5; }>>;

  auto func = [](test_arguments args) {
    EXPECT_EQ(get<"a">(args), 5);
    EXPECT_EQ(get<"b">(args), 7);
    EXPECT_EQ(get<"c">(args), 9);
    EXPECT_EQ("d"_tag(args), 1);
  };

  func({"d"_tag = 1, "a"_tag = 5});
}

}  // namespace
}  // namespace ftsd