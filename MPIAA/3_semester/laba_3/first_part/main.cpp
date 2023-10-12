#define CATCH_CONFIG_RUNNER

#include "catch.hpp"

#include "lcs.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std::chrono;
using namespace std;

int main() {
    int result = Catch::Session().run();

    default_random_engine generator;

    double time1 = 0.0;
    double time2 = 0.0;
    double time_stop = 3;
    int size = 2;
    int n = 3;
    string temp;
    while (time1 < time_stop && time2 < time_stop) {
        string fixedString(n, 'F');
        uniform_int_distribution<int> distribution(65, 90);
        string randomString(n, 'F');

        for (int i = 0; i < n; ++i) {
            randomString[i] = static_cast<char>(distribution(generator));
        }

        auto t1 = steady_clock::now();
        temp = lcs(fixedString, randomString);
        auto t2 = steady_clock::now();

        time1 = duration<double>(t2 - t1).count();

        t1 = steady_clock::now();
        temp = lcs_full_search(fixedString, randomString);
        t2 = steady_clock::now();

        time2 = duration<double>(t2 - t1).count();

        cout << "Time for lcs, N = " << n << endl;
        cout << "Time:" << time1 << endl;

        cout << "Time for lcs_full_search, N = " << n << endl;
        cout << "Time:" << time2 << endl;
        cout << "-------------------------------------" << endl;
        n += size;
    }
    return result;
}
