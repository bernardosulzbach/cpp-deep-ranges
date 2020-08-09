#pragma once

template <typename It>
class Range {
  It itBegin;
  It itEnd;

public:
  Range(It begin, It end) : itBegin(begin), itEnd(end) {
  }

  Range &operator++() {
    ++itBegin;
    return *this;
  }

  using ValueType = typename std::iterator_traits<It>::value_type;
  ValueType operator*() {
    return *itBegin;
  }

  [[nodiscard]] const bool isExhausted() const noexcept {
    return itBegin == itEnd;
  }
};

template <typename Iterable>
Range<decltype(std::declval<Iterable>().begin())> makeRange(Iterable &&i) {
  return Range<decltype(std::declval<Iterable>().begin())>(i.begin(), i.end());
}
