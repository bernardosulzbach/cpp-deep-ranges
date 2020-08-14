#pragma once

#include <iterator>

#include "Algorithm.hpp"

namespace DeepRanges {
/**
 * A plain and simple Range.
 *
 * Needs begin(It), end(It), and next(It).
 */
template <typename It>
class Range {
  It itBegin;
  It itEnd;

public:
  Range(It begin, It end) : itBegin(begin), itEnd(end) {
  }

  [[nodiscard]] It begin() const {
    return itBegin;
  }

  [[nodiscard]] It end() const {
    return itEnd;
  }

  Range &operator++() {
    ++itBegin;
    return *this;
  }

  auto operator*() {
    return *itBegin;
  }

  [[nodiscard]] bool isExhausted() const noexcept {
    return itBegin == itEnd;
  }

  template <typename Callable>
  void apply(Callable &&c) {
    while (!isExhausted()) {
      c(*(*this));
      ++(*this);
    }
  }
};

template <typename Iterable>
Range<IteratorType<Iterable>> makeRange(Iterable &&i) {
  return Range(begin(i), end(i));
}
} // namespace DeepRanges
