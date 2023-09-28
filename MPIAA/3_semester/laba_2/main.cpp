#include "closest_pair.h"
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include <chrono>

using namespace std;
using namespace std::chrono;

int main() {
  int result = Catch::Session().run();

  double time1 = 0.0;
  double time2 = 0.0;
  double time_stop = 0.3;
  int size_multiplicator = 10;
  int n = 10;
  while (time1 < time_stop && time2 < time_stop) {
    vector<double> seq = shuffledSequence(n);
    vector<Point> Points;
    int size = seq.size();
    for (int i = 0; i < size; i += 2) {
      Points.push_back(Point(seq[i], seq[i + 1]));
    }
    cout << "Time for closestPair, N = " << n << endl;
    auto t1 = steady_clock::now();
    closestPair(Points);
    auto t2 = steady_clock::now();
    time1 = duration<double>(t2 - t1).count();

    cout << "Time: " << time1 << endl;
    cout << "---------------------------------------------" << endl;

    cout << "Time for divideAndConquer, N = " << n << endl;
    t1 = steady_clock::now();
    divideAndConquer(Points);
    t2 = steady_clock::now();
    time2 = duration<double>(t2 - t1).count();

    cout << "Time: " << time2 << endl;
    cout << "---------------------------------------------" << endl;

    n *= size_multiplicator;
  }
  return result;
}
