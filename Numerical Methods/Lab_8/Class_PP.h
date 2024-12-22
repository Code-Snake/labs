#pragma once
#include <vector>
#include <functional>
#include "Point.h"

using namespace std;

template<typename T>
class Class_PP {
public:
    enum class SchemeType {
        ERK1 = 1,
        ERK2,
        ERK4,
        Adams
    };

    vector<T> adamsPredictorCorrector(
        SchemeType schemeType,
        double startTime,
        double endTime,
        const vector<vector<double>>& initialConditions,
        const function<vector<T>(const vector<T>& U, const Point& P)>& rhsFunction
    );

    vector<T> rungeKuttaScheme(
        SchemeType schemeType,
        double startTime,
        double endTime,
        const vector<T>& initialConditions,
        const function<vector<T>(const vector<T>& U, const Point& P)>& rhsFunction
    );

    vector<vector<T>> rungeKuttaImplementation(
        double startTime,
        double stepSize,
        int stepCount,
        const vector<T>& initialConditions,
        const function<vector<T>(const vector<double>& U, const Point& P)>& rhsFunction
    );
};

// Реализация метода Рунге-Кутты
template<typename T>
vector<T> Class_PP<T>::rungeKuttaScheme(
    SchemeType schemeType,
    double startTime,
    double endTime,
    const vector<T>& initialConditions,
    const function<vector<T>(const vector<T>& U, const Point& P)>& rhsFunction)
{
    int stagesNumber = 4;
    int variableCount = initialConditions.size();
    double stepSize = endTime - startTime;

    vector<T> solution = initialConditions;

    vector<double> b, c;
    vector<vector<double>> a;

    switch (schemeType) {
    case SchemeType::ERK1: {
        stagesNumber = 1;
        a = { {0} };
        b = { 1 };
        c = { 0.5 };
        break;
    }
    case SchemeType::ERK2: {
        stagesNumber = 2;
        a = { {0.0, 0.0}, {2.0 / 3.0, 0.0} };
        b = { 1.0 / 4.0, 3.0 / 4.0 };
        c = { 0.0, 2.0 / 3.0 };
        break;
    }
    default: {
        stagesNumber = 4;
        a = {
            {0, 0, 0, 0},
            {0.5, 0, 0, 0},
            {0, 0.5, 0, 0},
            {0, 0, 1.0, 0}
        };
        b = { 1.0 / 6.0, 1.0 / 3.0, 1.0 / 3.0, 1.0 / 6.0 };
        c = { 0.0, 0.5, 0.5, 1.0 };
        break;
    }
    }

    vector<vector<T>> w(stagesNumber, vector<T>(variableCount));

    for (int k = 0; k < stagesNumber; k++) {
        vector<T> adjustment1(variableCount, 0);
        for (int l = 0; l < k; l++) {
            for (int i = 0; i < variableCount; i++) {
                adjustment1[i] += a[k][l] * w[l][i];
            }
        }

        vector<T> U(variableCount);
        for (int i = 0; i < variableCount; i++) {
            U[i] = solution[i] + stepSize * adjustment1[i];
        }
        Point P(startTime + stepSize * c[k], 0, 0);
        w[k] = rhsFunction(U, P);
    }

    vector<T> adjustment2(variableCount, 0);
    for (int k = 0; k < stagesNumber; k++) {
        for (int i = 0; i < variableCount; i++) {
            adjustment2[i] += b[k] * w[k][i];
        }
    }

    for (int i = 0; i < variableCount; i++) {
        solution[i] += stepSize * adjustment2[i];
    }

    return solution;
}

template<typename T>
vector<vector<T>> Class_PP<T>::rungeKuttaImplementation(
    double startTime,
    double stepSize,
    int stepCount,
    const vector<T>& initialConditions,
    const function<vector<T>(const vector<double>& U, const Point& P)>& rhsFunction)
{
    vector<vector<T>> steps(stepCount, initialConditions);
    vector<T> solution = initialConditions;

    for (int step = 0; step < stepCount; ++step) {
        solution = rungeKuttaScheme(
            SchemeType::ERK4,
            startTime + step * stepSize,
            startTime + (step + 1) * stepSize,
            solution,
            rhsFunction
        );
        steps[step] = solution;
    }

    return steps;
}

template<typename T>
vector<T> Class_PP<T>::adamsPredictorCorrector(
    SchemeType schemeType,
    double startTime,
    double endTime,
    const vector<vector<double>>& initialConditions,
    const function<vector<T>(const vector<T>& U, const Point& P)>& rhsFunction)
{
    int variableCount = initialConditions[0].size();
    double stepSize = endTime - startTime;
    vector<T> predicted(variableCount);
    vector<T> corrected(variableCount);
    Point P(startTime, 0, 0);

    for (int i = 0; i < variableCount; ++i) {
        predicted[i] = initialConditions[3][i] + stepSize * (
            55.0 / 24.0 * rhsFunction(initialConditions[3], P)[i] -
            59.0 / 24.0 * rhsFunction(initialConditions[2], P)[i] +
            37.0 / 24.0 * rhsFunction(initialConditions[1], P)[i] -
            9.0 / 24.0 * rhsFunction(initialConditions[0], P)[i]
            );
    }

    const int maxIterations = 10;
    const double tolerance = 1e-6;

    vector<T> previous = predicted;
    corrected = predicted;

    for (int iter = 0; iter < maxIterations; ++iter) {
        Point P_next(startTime + stepSize, 0, 0);

        for (int i = 0; i < variableCount; ++i) {
            corrected[i] = initialConditions[3][i] + stepSize / 24.0 * (
                9.0 * rhsFunction(corrected, P_next)[i] +
                19.0 * rhsFunction(initialConditions[3], P)[i] -
                5.0 * rhsFunction(initialConditions[2], P)[i] +
                rhsFunction(initialConditions[1], P)[i]
                );
        }

        double error = 0.0;
        for (int i = 0; i < variableCount; ++i) {
            error += abs(corrected[i] - previous[i]);
        }
        if (error < tolerance) {
            break;
        }

        previous = corrected;
    }

    return corrected;
}
