#include "point.h"
#include <vector>
#include <numeric>
#include <random>

std::pair<Point, Point> closestPair(const std::vector<Point> &points);
std::pair<Point, Point> closestPairBetween(std::vector<Point>& PLeft, std::vector<Point>& PRight, double d);
std::pair<Point, Point> divideAndConquer(const std::vector<Point> &points);
std::vector<double> shuffledSequence(int size);
