#pragma once

#include <iterator>
#include <optional>

#include "Range.hpp"
#include "Types.hpp"

namespace DeepRanges {
/**
 * A DeepRange of depth N has two iterators: current position and end.
 *
 * The size of a DeepRange of an iterator It and depth N is guaranteed to be N * sizeof(It).
 *
 * For each position, it creates a DeepRange of depth N - 1, if N > 1. If N = 1, it returns its elements directly.
 *
 * Requires the pre-increment, the dereference, and the equality operators to be defined for all iterators.
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

  // Advances the deep range until it either dereferences to a value or is exhausted.
  // If the deep range already already dereferences to a value or is exhausted, nothing is done.
  void seek() noexcept {
    if (itBegin == itEnd) {
      return;
    }
    while (nestedDeepRange.isExhausted()) {
      ++itBegin;
      if (itBegin == itEnd) {
        return;
      }
      makeNestedDeepRange();
    }
  }

public:
  DeepRange(It iBegin, It iEnd) : itBegin(iBegin), itEnd(iEnd) {
    if (itBegin == itEnd) {
      return;
    }
    makeNestedDeepRange();
    seek();
  }

  [[nodiscard]] It begin() const noexcept {
    return itBegin;
  }

  [[nodiscard]] It end() const noexcept {
    return itEnd;
  }

  DeepRange &operator++() noexcept {
    if (!nestedDeepRange.isExhausted()) {
      ++nestedDeepRange;
    }
    seek();
    return *this;
  }

  [[nodiscard]] auto operator*() noexcept {
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

  [[nodiscard]] It begin() const noexcept {
    return itBegin;
  }

  [[nodiscard]] It end() const noexcept {
    return itEnd;
  }

  DeepRange &operator++() noexcept {
    ++itBegin;
    return *this;
  }

  [[nodiscard]] auto operator*() noexcept {
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
