#include <cstdint>
#include <iostream>
#include <vector>

#include "DeepRange.hpp"
#include "Range.hpp"
#include "Types.hpp"

using std::begin;
using std::end;

template <typename RangeLike, typename Functor>
void apply(RangeLike &&rangeLike, Functor &&f) {
  while (!rangeLike.isExhausted()) {
    f(*rangeLike);
    ++rangeLike;
  }
}

void printS32(const S32 value) {
  std::cout << value << '\n';
}

int main() {
  std::vector<S32> v = {-1, -2, -3};
  apply(Range(begin(v), end(v)), printS32);
  apply(makeRange(v), printS32);
  apply(DeepRange<std::vector<S32>::iterator, 1>(std::begin(v), std::end(v)), printS32);
  std::vector<std::vector<S32>> m = {{1, 2}, {3, 4}};
  apply(DeepRange<std::vector<std::vector<S32>>::iterator, 2>(std::begin(m), std::end(m)), printS32);
}
