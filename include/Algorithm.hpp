#pragma once

#include <vector>

namespace DeepRanges {
template <typename T>
using IteratorType = decltype(std::declval<T>().begin());

template <typename T>
using DereferenceType = decltype(std::declval<T>().operator*());

/**
 * Converts a range into a vector.
 */
template <typename RangeLike>
std::vector<DereferenceType<RangeLike>> toVector(RangeLike &&r) {
  std::vector<DereferenceType<RangeLike>> v;
  r.apply([&v](const auto &t) {
    v.push_back(t);
  });
  return v;
}
} // namespace DeepRanges
