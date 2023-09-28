#include <iostream>
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include <chrono>
#include <vector>
#include <set>
#include <unordered_set>
#include <numeric>
#include <random>

using namespace std;
using namespace std::chrono;

bool has_duplicates(const vector<int> &V) {
  for (size_t i = 0; i < V.size(); ++i) {
    for (size_t j = i + 1; j < V.size(); ++j) {
      if (V[i] == V[j]) {
        return true;
      }
    }
  }
  return false;
}

vector<int> get_duplicates(const vector<int> &data) {
  std::vector<int> duplicates;
  for (size_t i = 0; i < data.size(); ++i) {
    for (size_t j = i + 1; j < data.size(); ++j) {
      if (data[i] == data[j]) {
        auto it = find(duplicates.begin(), duplicates.end(), data[i]);
        if (it == duplicates.end()) {
          duplicates.push_back(data[i]);
        }
      }
    }
  }
  return duplicates;
}

bool fast_has_duplicates(const std::vector<int> &data) {
  std::unordered_set<int> seen;
  for (const auto &element : data) {
    if (seen.count(element) > 0) {
      return true;
    }
    seen.insert(element);
  }
  return false;
}

vector<int> fast_get_duplicates(const vector<int> &data) {
  std::unordered_set<int> seen;
  std::vector<int> duplicates;

  for (const auto &element : data) {
    if (seen.count(element) > 0) {
      auto it = find(duplicates.begin(), duplicates.end(), element);
      if (it == duplicates.end()) {
        duplicates.push_back(element);
      }
    } else {
      seen.insert(element);
    }
  }
  return duplicates;
}

vector<int> shuffled_sequence(int size) {
  vector<int> result(size);
  iota(result.begin(), result.end(), 0);
  random_device rd;
  mt19937 rng(rd());
  shuffle(result.begin(), result.end(), rng);
  return result;
}

template <class T> std::vector<T> sorted(const typename std::vector<T> &data) {
  typename std::vector<T> sorted_data = data;
  std::sort(sorted_data.begin(), sorted_data.end());
  return sorted_data;
}

TEST_CASE("Empty array") {
  const std::vector<int> data = {};
  CHECK_FALSE(fast_has_duplicates(data));
  CHECK(sorted(fast_get_duplicates(data)) == std::vector<int>{});
  CHECK_FALSE(has_duplicates(data));
  CHECK(sorted(get_duplicates(data)) == std::vector<int>{});
}

TEST_CASE("Single element") {
  const std::vector<int> data = {1};
  CHECK_FALSE(fast_has_duplicates(data));
  CHECK(sorted(fast_get_duplicates(data)) == std::vector<int>{});
  CHECK_FALSE(has_duplicates(data));
  CHECK(sorted(get_duplicates(data)) == std::vector<int>{});
}

TEST_CASE("Many elements") {
  const std::vector<int> data = {2, 1, -4, 7};
  CHECK_FALSE(fast_has_duplicates(data));
  CHECK(sorted(fast_get_duplicates(data)) == std::vector<int>{});
  CHECK_FALSE(has_duplicates(data));
  CHECK(sorted(get_duplicates(data)) == std::vector<int>{});
}

TEST_CASE("Many elements, one duplicate") {
  const std::vector<int> data = {2, -3, 0, 2, 7, 1};
  CHECK(fast_has_duplicates(data));
  CHECK(sorted(fast_get_duplicates(data)) == std::vector<int>{2});
  CHECK(has_duplicates(data));
  CHECK(sorted(get_duplicates(data)) == std::vector<int>{2});
}

TEST_CASE("Many elements, many duplicates") {
  const std::vector<int> data = {2, -3, 0, 2, 1, -3, 4, 1, -1, 2};
  CHECK(fast_has_duplicates(data));
  CHECK(sorted(fast_get_duplicates(data)) == std::vector<int>{-3, 1, 2});
  CHECK(has_duplicates(data));
  CHECK(sorted(get_duplicates(data)) == std::vector<int>{-3, 1, 2});
}

TEST_CASE("Single duplicated elem") {
  const std::vector<int> data = {4, 4, 4, 4};
  CHECK(fast_has_duplicates(data));
  CHECK(sorted(fast_get_duplicates(data)) == std::vector<int>{4});
  CHECK(has_duplicates(data));
  CHECK(sorted(get_duplicates(data)) == std::vector<int>{4});
}

int main() {
  int result = Catch::Session().run();

  double time = 0.0;
  double time_stop = 0.2;
  int size_multiplicator = 10;
  int size = 10;
  while (time < time_stop) {
    cout << "Time for has_duplicates, N = " << size << endl;
    const auto data = shuffled_sequence(size);

    auto t1 = steady_clock::now();
    bool isdup = has_duplicates(data);
    auto t2 = steady_clock::now();
    time = duration<double>(t2 - t1).count();
    cout << "isdup? " << isdup << endl;
    cout << "Time: " << time << endl;
    cout << "---------------------------------------------" << endl;
    size *= size_multiplicator;
  }

  time = 0.0;
  size = 10;

  while (time < time_stop) {
    cout << "Time for get_duplicates, N = " << size << endl;
    const auto data = shuffled_sequence(size);

    auto t1 = steady_clock::now();
    vector<int> dups = get_duplicates(data);
    auto t2 = steady_clock::now();
    time = duration<double>(t2 - t1).count();

    cout << "Time: " << time << endl;
    cout << "---------------------------------------------" << endl;
    size *= size_multiplicator;
  }

  time = 0.0;
  size = 10;

  while (time < time_stop) {
    cout << "Time for fast_has_duplicates, N = " << size << endl;
    const auto data = shuffled_sequence(size);

    auto t1 = steady_clock::now();
    bool isdup = fast_has_duplicates(data);
    auto t2 = steady_clock::now();
    time = duration<double>(t2 - t1).count();
    cout << "isdup? " << isdup << endl;
    cout << "Time: " << time << endl;
    cout << "---------------------------------------------" << endl;
    size *= size_multiplicator;
  }

  time = 0.0;
  size = 10;

  while (time < time_stop) {
    cout << "Time for fast_get_duplicates, N = " << size << endl;
    const auto data = shuffled_sequence(size);

    auto t1 = steady_clock::now();
    vector<int> dups = fast_get_duplicates(data);
    auto t2 = steady_clock::now();
    time = duration<double>(t2 - t1).count();

    cout << "Time: " << time << endl;
    cout << "---------------------------------------------" << endl;
    size *= size_multiplicator;
  }

  return result;
}
