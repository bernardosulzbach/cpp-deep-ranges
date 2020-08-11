#pragma once

#include <iterator>
#include <optional>

#include "Range.hpp"
#include "Types.hpp"

namespace DeepRanges {
/**
 * A DeepRange of depth N has two iterators: current position and end.
 *
 * For each position, it creates a DeepRange of depth N - 1, if N > 1.
 *
 * If N = 1, it returns its elements directly.
 */
template <typename It, U32 Depth>
class DeepRange {
  static_assert(Depth > 1U);

  It itBegin;
  It itEnd;

  using IteratorOfDereferenceType = IteratorType<DereferenceType<It>>;

  // Use an anonymous union to prevent wasting memory with std::optional or the like.
  union {
    DeepRange<IteratorOfDereferenceType, Depth - 1U> nestedDeepRange;
  };

  void makeNestedDeepRange() {
    nestedDeepRange = DeepRange<IteratorOfDereferenceType, Depth - 1U>(std::begin(*itBegin), std::end(*itBegin));
  }

public:
  DeepRange(It iBegin, It iEnd) : itBegin(iBegin), itEnd(iEnd) {
    if (itBegin != itEnd) {
      makeNestedDeepRange();
    }
  }

  [[nodiscard]] It begin() const {
    return itBegin;
  }

  [[nodiscard]] It end() const {
    return itEnd;
  }

  DeepRange &operator++() {
    if (!nestedDeepRange.isExhausted()) {
      ++nestedDeepRange;
    }
    if (nestedDeepRange.isExhausted()) {
      ++itBegin;
      if (itBegin == itEnd) {
        return (*this);
      }
      makeNestedDeepRange();
    }
    return (*this);
  }

  auto operator*() {
    return *(nestedDeepRange);
  }

  template <typename Callable>
  void apply(Callable &&c) {
    while (!isExhausted()) {
      c(*(*this));
      ++(*this);
    }
  }

  [[nodiscard]] bool isExhausted() const noexcept {
    return itBegin == itEnd;
  }
};

template <typename It>
class DeepRange<It, 1> {
  It itBegin;
  It itEnd;

public:
  DeepRange(It iBegin, It iEnd) : itBegin(iBegin), itEnd(iEnd) {
  }

  void operator++() {
    ++itBegin;
  }

  auto operator*() {
    return *itBegin;
  }

  template <typename Callable>
  void apply(Callable &&c) {
    while (!isExhausted()) {
      c(*(*this));
      ++(*this);
    }
  }

  [[nodiscard]] bool isExhausted() const noexcept {
    return itBegin == itEnd;
  }
};

template <U32 Depth, typename Iterable>
DeepRange<IteratorType<Iterable>, Depth> makeDeepRange(Iterable &&i) {
  return DeepRange<IteratorType<Iterable>, Depth>(std::begin(i), std::end(i));
}
} // namespace DeepRanges
