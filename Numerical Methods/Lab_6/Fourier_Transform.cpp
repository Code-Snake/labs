#include "Fourier_Transform.h"

void FourierTransform::updateData(const std::vector<std::complex<double>>& inputData) {
    data = inputData;
    N = data.size();
}

std::vector<std::complex<double>> FourierTransform::computeDFT() const {
    std::vector<std::complex<double>> result(N);
    for (size_t k = 0; k < N; ++k) {
        std::complex<double> accumulator(0.0, 0.0);
        for (size_t n = 0; n < N; ++n) {
            double theta = -2.0 * PI * k * n / static_cast<double>(N);
            accumulator += data[n] * std::polar(1.0, theta);
        }
        result[k] = accumulator;
    }
    return result;
}

std::vector<std::complex<double>> FourierTransform::computeIDFT() const {
    std::vector<std::complex<double>> result(N);
    for (size_t n = 0; n < N; ++n) {
        std::complex<double> accumulator(0.0, 0.0);
        for (size_t k = 0; k < N; ++k) {
            double theta = 2.0 * PI * k * n / static_cast<double>(N);
            accumulator += data[k] * std::polar(1.0, theta);
        }
        result[n] = accumulator / static_cast<double>(N);
    }
    return result;
}

std::vector<std::complex<double>> FourierTransform::computeFFT() const {
    if (N <= 1) {
        return data;
    }

    size_t halfSize = N / 2;
    std::vector<std::complex<double>> evenData(halfSize), oddData(halfSize);
    for (size_t i = 0; i < halfSize; ++i) {
        evenData[i] = data[i * 2];
        oddData[i] = data[i * 2 + 1];
    }

    FourierTransform evenFT, oddFT;
    evenFT.updateData(evenData);
    oddFT.updateData(oddData);

    auto evenResult = evenFT.computeFFT();
    auto oddResult = oddFT.computeFFT();

    std::vector<std::complex<double>> result(N);
    for (size_t k = 0; k < halfSize; ++k) {
        std::complex<double> twiddleFactor = std::polar(1.0, -2.0 * PI * k / static_cast<double>(N));
        result[k] = evenResult[k] + twiddleFactor * oddResult[k];
        result[k + halfSize] = evenResult[k] - twiddleFactor * oddResult[k];
    }

    return result;
}

std::vector<std::complex<double>> FourierTransform::computeIFFT() const {
    auto fftResult = computeFFT();
    std::vector<std::complex<double>> result(N);

    for (size_t i = 0; i < N; ++i) {
        result[i] = std::conj(fftResult[i]) / static_cast<double>(N);
    }

    return result;
}
