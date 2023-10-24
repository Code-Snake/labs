#pragma once
#include <vector>
#include <string>

class Matrix {
private:
    int rows;
    int cols;
    std::vector<std::vector<float>> data;

public:
    Matrix(int r, int c);

    void print();

    static Matrix loadFromFile(const std::string& filename);

    void multByScalar(float scalar);

    Matrix Transposition();

    float calculateDeterminant();

    Matrix inverse();
};
