#ifndef FOURIER_TRANSFORM_H
#define FOURIER_TRANSFORM_H

#include <vector>
#include <complex>

const double PI = 3.141592653589793;

class FourierTransform {
private:
    std::vector<std::complex<double>> data; // �������� ������
    size_t N = 0; // ������ ������

public:
    // ���������� ������
    void updateData(const std::vector<std::complex<double>>& inputData);

    // ���������� �������������� �����
    std::vector<std::complex<double>> computeDFT() const;

    // �������� ���������� �������������� �����
    std::vector<std::complex<double>> computeIDFT() const;

    // ������� �������������� �����
    std::vector<std::complex<double>> computeFFT() const;

    // �������� ������� �������������� �����
    std::vector<std::complex<double>> computeIFFT() const;
};

#endif // FOURIER_TRANSFORM_H
