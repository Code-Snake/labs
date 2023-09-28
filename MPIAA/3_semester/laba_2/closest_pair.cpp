#include "closest_pair.h"
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <limits>

using namespace std;

double long dist(const Point& p1, const Point& p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

pair<Point, Point> closestPair(const std::vector<Point>& points) {
    // Return the closest pair of points from given points.
    // Order of points in the result doesn't matter.
    // If there are fewer than 2 points, throw an exception.

    if (points.size() < 2) {
        throw invalid_argument("Not enough points");
    }

    double long min_distance = std::numeric_limits<double long>::max();
    // at the beginning!!!
    auto result = make_pair(Point(), Point());
    for (size_t i = 0; i < points.size(); ++i) {
        for (size_t j = i + 1; j < points.size(); ++j) {
            double distance = dist(points[i], points[j]);

            if (distance < min_distance) {
                min_distance = distance;
                result = make_pair(points[i], points[j]);
            }
        }
    }
    // Your implementation here.
    return result;
}

pair<Point, Point> closestPairBetween(vector<Point>& PLeft,
    vector<Point>& PRight, double d) {
    vector<Point> PStripe;
    double Xm = (PLeft.back().x + PRight.front().x) / 2.0;

    for (const Point& p : PRight) {
        if (abs(p.x - Xm) < d) {
            PStripe.push_back(p);
        }
    }

    for (const Point& p : PLeft) {
        if (abs(p.x - Xm) < d) {
            PStripe.push_back(p);
        }
    }

    sort(PStripe.begin(), PStripe.end(),
        [](Point& a, Point& b) { return a.y < b.y; });

    pair<Point, Point> closest_pair;

    closest_pair = closestPair(PStripe);
    return closest_pair;
}

pair<Point, Point> divideAndConquer(const vector<Point>& points) {
    if (points.size() < 2) {
        throw invalid_argument("Not enough points");
    }

    if (points.size() <= 3) {
        return closestPair(points);
    }

    vector<Point> sorted_points = points;
    sort(sorted_points.begin(), sorted_points.end(),
        [](const Point& a, const Point& b) { return a.x < b.x; });

    size_t middle = points.size() / 2;
    vector<Point> PLeft(sorted_points.begin(), sorted_points.begin() + middle);
    vector<Point> PRight(sorted_points.begin() + middle, sorted_points.end());

    pair<Point, Point> pl = divideAndConquer(PLeft);
    pair<Point, Point> pr = divideAndConquer(PRight);

    double d = min(dist(pl.first, pl.second), dist(pr.first, pr.second));

    pair<Point, Point> pb = closestPairBetween(PLeft, PRight, d);

    if (dist(pl.first, pl.second) < dist(pr.first, pr.second) &&
        dist(pl.first, pl.second) < dist(pb.first, pb.second)) {
        return pl;
    }
    else if (dist(pr.first, pr.second) < dist(pl.first, pl.second) &&
        dist(pr.first, pr.second) < dist(pb.first, pb.second)) {
        return pr;
    }
    else {
        return pb;
    }
}

vector<double> shuffledSequence(int size) {
    vector<double> result(size);
    iota(result.begin(), result.end(), 0);
    random_device rd;
    mt19937 rng(rd());
    shuffle(result.begin(), result.end(), rng);
    return result;
}
