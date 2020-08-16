#define BOOST_TEST_MODULE "Main test module"
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <iostream>
#include <vector>

#include "Algorithm.hpp"
#include "DeepRange.hpp"
#include "Range.hpp"
#include "Types.hpp"

using DeepRanges::S32;

using DeepRanges::makeRange;
using DeepRanges::Range;

using DeepRanges::DeepRange;
using DeepRanges::makeDeepRange;

BOOST_AUTO_TEST_CASE(test_range_with_empty_vector) {
  const auto v = std::vector<S32>{};
  BOOST_TEST(v == toVector(makeRange(v)));
}

BOOST_AUTO_TEST_CASE(test_range_over_vector_should_use_the_same_space_as_2_pointers) {
  const auto v = std::vector<S32>{};
  const auto range = makeRange(v);
  BOOST_TEST(2 * sizeof(void *) == sizeof(range));
}

BOOST_AUTO_TEST_CASE(test_range_with_non_empty_vector) {
  const auto v = std::vector<S32>{1, 2, 3};
  BOOST_TEST(v == toVector(makeRange(v)));
}

BOOST_AUTO_TEST_CASE(test_deep_range_of_depth_1_with_empty_vector) {
  const auto v = std::vector<S32>{};
  BOOST_TEST(v == toVector(makeDeepRange<1>(v)));
}

BOOST_AUTO_TEST_CASE(test_deep_range_of_depth_1_over_vector_should_use_the_same_space_as_2_pointers) {
  const auto v = std::vector<S32>{};
  const auto range = makeDeepRange<1>(v);
  BOOST_TEST(2 * sizeof(void *) == sizeof(range));
}

BOOST_AUTO_TEST_CASE(test_deep_range_of_depth_2_with_empty_vector) {
  const auto v = std::vector<std::vector<S32>>{};
  const auto emptyVector = std::vector<S32>();
  BOOST_TEST(emptyVector == toVector(makeDeepRange<2>(v)));
}

BOOST_AUTO_TEST_CASE(test_deep_range_of_depth_2_over_vector_should_use_the_same_space_as_4_pointers) {
  const auto v = std::vector<std::vector<S32>>{};
  const auto range = makeDeepRange<2>(v);
  BOOST_TEST(4 * sizeof(void *) == sizeof(range));
}

BOOST_AUTO_TEST_CASE(test_deep_range_of_depth_2_over_non_empty_vector_of_empty_vectors) {
  const auto v = std::vector<std::vector<S32>>{{}, {}};
  const auto expectedResult = std::vector<S32>{};
  BOOST_TEST(expectedResult == toVector(makeDeepRange<2>(v)));
}

BOOST_AUTO_TEST_CASE(test_deep_range_of_depth_2_over_non_empty_vector_with_an_empty_vector_at_the_start) {
  const auto v = std::vector<std::vector<S32>>{{}, {1, 2}};
  const auto expectedResult = std::vector<S32>{1, 2};
  BOOST_TEST(expectedResult == toVector(makeDeepRange<2>(v)));
}

BOOST_AUTO_TEST_CASE(test_deep_range_of_depth_2_over_non_empty_vector_with_an_empty_vector_at_the_end) {
  const auto v = std::vector<std::vector<S32>>{{1, 2}, {}};
  const auto expectedResult = std::vector<S32>{1, 2};
  BOOST_TEST(expectedResult == toVector(makeDeepRange<2>(v)));
}

BOOST_AUTO_TEST_CASE(test_deep_range_of_depth_2_over_non_empty_vector_of_non_empty_vectors) {
  const auto v = std::vector<std::vector<S32>>{{1, 2}, {3, 4}};
  const auto expectedResult = std::vector<S32>{1, 2, 3, 4};
  BOOST_TEST(expectedResult == toVector(makeDeepRange<2>(v)));
}

BOOST_AUTO_TEST_CASE(test_deep_range_of_depth_2_over_non_empty_non_uniform_vector) {
  const auto v = std::vector<std::vector<S32>>{{1}, {2, 3}, {4, 5, 6}};
  const auto expectedResult = std::vector<S32>{1, 2, 3, 4, 5, 6};
  BOOST_TEST(expectedResult == toVector(makeDeepRange<2>(v)));
}

BOOST_AUTO_TEST_CASE(test_deep_range_of_depth_3_with_empty_vector) {
  const auto v = std::vector<std::vector<std::vector<S32>>>{};
  const auto emptyVector = std::vector<S32>();
  BOOST_TEST(emptyVector == toVector(makeDeepRange<3>(v)));
}

BOOST_AUTO_TEST_CASE(test_deep_range_of_depth_3_over_vector_should_use_the_same_space_as_6_pointers) {
  const auto v = std::vector<std::vector<std::vector<S32>>>{};
  const auto range = makeDeepRange<3>(v);
  BOOST_TEST(6 * sizeof(void *) == sizeof(range));
}

BOOST_AUTO_TEST_CASE(test_deep_range_of_depth_4_over_empty_list) {
  const auto l = std::list<std::list<std::list<std::list<S32>>>>{};
  const auto expectedResult = std::vector<S32>{};
  BOOST_TEST(expectedResult == toVector(makeDeepRange<4>(l)));
}

BOOST_AUTO_TEST_CASE(test_deep_range_of_depth_4_over_non_empty_list) {
  const auto l = std::list<std::list<std::list<std::list<S32>>>>{{{{1, 2}, {3}}, {{4}}}};
  const auto expectedResult = std::vector<S32>{1, 2, 3, 4};
  BOOST_TEST(expectedResult == toVector(makeDeepRange<4>(l)));
}

BOOST_AUTO_TEST_CASE(test_deep_range_of_depth_4_over_list_should_use_the_same_space_as_8_pointers) {
  const auto l = std::list<std::list<std::list<std::list<S32>>>>{};
  const auto range = makeDeepRange<4>(l);
  BOOST_TEST(8 * sizeof(void *) == sizeof(range));
}
