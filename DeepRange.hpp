#pragma once

#include <iterator>
#include <optional>

#include "Range.hpp"
#include "Types.hpp"

/**
 * A DeepRange of depth N has two iterators: current position and end.
 *
 * For each position, it creates a DeepRange of depth N - 1, if N > 1.
 *
 * If N = 1, it returns its elements directly.
 */
template <typename It, U32 Depth>
class DeepRange {
  static_assert(Depth > 1);

  using ValueType = typename std::iterator_traits<It>::value_type;
  using IteratorOfValueType = typename ValueType::iterator;

  It itBegin;
  It itEnd;

  std::optional<DeepRange<IteratorOfValueType, Depth - 1>> nestedDeepRange;

  void makeNestedDeepRange() {
    nestedDeepRange = DeepRange<IteratorOfValueType, Depth - 1>(std::begin(*itBegin), std::end(*itBegin));
  }

public:
  DeepRange(It iBegin, It iEnd) : itBegin(iBegin), itEnd(iEnd) {
    if (itBegin != itEnd) {
      makeNestedDeepRange();
    }
  }

  void operator++() {
    if (!nestedDeepRange.value().isExhausted()) {
      ++(*nestedDeepRange);
    }
    if (nestedDeepRange.value().isExhausted()) {
      itBegin = std::next(itBegin);
      if (itBegin == itEnd) {
        return;
      }
      makeNestedDeepRange();
    }
  }

  auto operator*() {
    return *(nestedDeepRange.value());
  }

  [[nodiscard]] const bool isExhausted() const noexcept {
    return itBegin == itEnd;
  }
};

template <typename It>
class DeepRange<It, 1> {
  using ValueType = typename std::iterator_traits<It>::value_type;

  It itBegin;
  It itEnd;

public:
  DeepRange(It iBegin, It iEnd) : itBegin(iBegin), itEnd(iEnd) {
  }

  void operator++() {
    itBegin = std::next(itBegin);
  }

  auto operator*() {
    return *itBegin;
  }

  [[nodiscard]] const bool isExhausted() const noexcept {
    return itBegin == itEnd;
  }
};

template <typename Iterable, U32 Depth>
DeepRange<decltype(std::declval<Iterable>().begin()), Depth> makeDeepRange(Iterable &&i) {
  return DeepRange<decltype(std::declval<Iterable>().begin()), Depth>(i.begin(), i.end());
}
