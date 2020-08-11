# Deep C++ ranges

An example implementation of deep C++ ranges.

If you want to report an issue or talk about changes, feel free to reach out to me either directly or through issues in this repository.

## Examples

```cpp
std::vector<std::vector<S32>> m = {{1, 2}, {3, 4}};
// A normal (depth 1) range over m is a range of std::vector<S32>.
// A depth 2 range over m would be a range of S32.
```

```cpp
const auto v = std::vector<std::vector<S32>>{{1}, {2, 3}, {4, 5, 6}};
toVector(makeDeepRange<2>(v)) // makes std::vector<S32>{1, 2, 3, 4, 5, 6}
```

For more code examples consider reading [test/Tests.cpp](test/Tests.cpp).

To understand how some of this was implemented, look at the headers under include.
