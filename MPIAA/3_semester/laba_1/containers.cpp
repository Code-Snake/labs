#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <random>
#include <list>
#include <chrono>
#include <set>
#include <unordered_set>

using namespace std;
using namespace std::chrono;

// vector<int> shuffled_sequence(int size);
// template <typename Container>
// auto measure_time_list_and_vector(Container &container, int size);

vector<int> shuffled_sequence(int size) {
  int start = 0;
  vector<int> result(size);
  iota(result.begin(), result.end(), start);
  random_device rd;
  mt19937 rng(rd());
  shuffle(result.begin(), result.end(), rng);
  return result;
}

template <typename Container>
auto measure_time_list_and_vector(Container &container, int size) {
  // Perform push_back into container
  auto t1 = steady_clock::now();
  const auto elems_to_add = shuffled_sequence(size);
  for (const auto &elem : elems_to_add) {
    container.push_back(elem);
  }
  auto t2 = steady_clock::now();
  auto time1 = duration<double>(t2 - t1).count();
  cout << "Push_back: " << time1 << " seconds" << endl;

  // Perform instertion into container
  container.clear();
  t1 = steady_clock::now();
  const auto elems_to_insert = shuffled_sequence(size);
  for (const auto &elem : elems_to_insert) {
    container.insert(container.begin(), elem);
  }
  t2 = steady_clock::now();
  auto time2 = duration<double>(t2 - t1).count();
  cout << "Insert: " << time2 << " seconds" << endl;

  // Perform search into container.
  int hits = 0;
  const auto elems_to_search = shuffled_sequence(size / 2);
  t1 = steady_clock::now();
  for (const auto &elem : elems_to_search) {
    auto it = find(container.begin(), container.end(), elem);
    if (it != container.end()) {
      hits++;
    }
  }
  t2 = steady_clock::now();
  auto time3 = duration<double>(t2 - t1).count();
  cout << "Find: " << time3 << " seconds" << endl;
  auto fulltime = time1 + time2 + time3;
  cout << "Fulltime: " << fulltime << " seconds" << endl;
  return fulltime;
}

template <typename Container>
auto measure_time_set_and_unordered_set(Container &container, int size) {
  // Perform push_back into container
  auto t1 = steady_clock::now();
  const auto elems_to_add = shuffled_sequence(size);
  for (const auto &elem : elems_to_add) {
    container.insert(elem);
  }
  auto t2 = steady_clock::now();
  auto time1 = duration<double>(t2 - t1).count();
  cout << "Insert: " << time1 << " seconds" << endl;

  // Perform search into container.
  int hits = 0;
  const auto elems_to_search = shuffled_sequence(size/2);
  t1 = steady_clock::now();
  for (const auto &elem : elems_to_search) {
    auto it = container.find(elem);
    if (it != container.end()) {
      hits++;
    }
  }
  t2 = steady_clock::now();
  auto time2 = duration<double>(t2 - t1).count();
  cout << "Find: " << time2 << " seconds" << endl;

  auto fulltime = time1 + time2;
  cout << "Fulltime: " << fulltime << " seconds" << endl;
  return fulltime;
}

int main() {
  // Containers to use.
  vector<int> container_vector;
  list<int> container_list;
  set<int> container_set;
  unordered_set<int> container_unordered_set;

  double total_time = 0.0;
  double time_stop = 0.4;
  int size_multiplicator = 10;
  int n = 10;

  cout << "For vector" << endl;
  while (total_time < time_stop) {
    cout << "Time for vector N = " << n << endl;
    total_time = measure_time_list_and_vector(container_vector, n);
    cout << "---------------------------------------------" << endl;
    n *= size_multiplicator;
  }

  total_time = 0.0;
  n = 10;
  cout << "For list" << endl;
  while (total_time < time_stop) {
    cout << "Time for list N = " << n << endl;
    total_time = measure_time_list_and_vector(container_list, n);
    cout << "---------------------------------------------" << endl;
    n *= size_multiplicator;
  }
  total_time = 0.0;
  n = 10;
  cout << "For set" << endl;
  while (total_time < time_stop) {
    cout << "Time for set N = " << n << endl;
    total_time = measure_time_set_and_unordered_set(container_set, n);
    cout << "---------------------------------------------" << endl;
    n *= size_multiplicator;
  }

  total_time = 0.0;
  n = 10;
  cout << "For unordered_set" << endl;
  while (total_time < time_stop) {
    cout << "Time for unordered_set N = " << n << endl;
    total_time = measure_time_set_and_unordered_set(container_unordered_set, n);
    cout << "---------------------------------------------" << endl;
    n *= size_multiplicator;
  }
  return 0;
}
