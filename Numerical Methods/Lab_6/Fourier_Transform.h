#ifndef FOURIER_TRANSFORM_H
#define FOURIER_TRANSFORM_H

#include <vector>
#include <complex>

const double PI = 3.141592653589793;

class FourierTransform {
private:
    std::vector<std::complex<double>> data; // Исходные данные
    size_t N = 0; // Размер данных

public:
    // Обновление данных
    void updateData(const std::vector<std::complex<double>>& inputData);

    // Дискретное преобразование Фурье
    std::vector<std::complex<double>> computeDFT() const;

    // Обратное дискретное преобразование Фурье
    std::vector<std::complex<double>> computeIDFT() const;

    // Быстрое преобразование Фурье
    std::vector<std::complex<double>> computeFFT() const;

    // Обратное быстрое преобразование Фурье
    std::vector<std::complex<double>> computeIFFT() const;
};

#endif // FOURIER_TRANSFORM_H
