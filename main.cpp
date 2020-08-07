#include <cstdint>
#include <iostream>
#include <vector>

using S32 = int32_t;

using std::begin;
using std::end;

template <typename It>
class Range {
  It itBegin;
  It itEnd;

public:
  Range(It begin, It end) : itBegin(begin), itEnd(end) {
  }

  void operator++() {
    ++itBegin;
  }

  using ValueType = typename std::iterator_traits<It>::value_type;
  ValueType operator*() {
    return *itBegin;
  }

  bool ended() const {
    return itBegin == itEnd;
  }
};

template <typename Iterable>
Range<decltype(std::declval<Iterable>().begin())> makeRange(Iterable &&i) {
  return Range<decltype(std::declval<Iterable>().begin())>(i.begin(), i.end());
}

template <typename It, typename Functor>
void apply(Range<It> &&range, Functor &&f) {
  while (!range.ended()) {
    f(*range);
    ++range;
  }
}

void printS32(const S32 value) {
  std::cout << value << '\n';
}

int main() {
  std::vector<S32> v = {-1, -2, -3};
  apply(Range(begin(v), end(v)), printS32);
  apply(makeRange(v), printS32);
  // Maybe?
  // std::vector<std::vector<S32>> m = {{1, 2}, {3, 4}};
  // apply(makeDeepRange(m), printS32);
}
