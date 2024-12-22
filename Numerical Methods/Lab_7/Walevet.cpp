#include "Walevet.h"
using namespace std;

// Конструктор: инициализация масштабирующих и материнских вейвлетов
// signalLength - количество отсчётов в сигнале, type - тип вейвлет-базиса
WaveletTransform::WaveletTransform(int signalLength, BasisType type) {
    int N = signalLength;
    scalingWavelet.clear(); scalingWavelet.resize(N);
    motherWavelet.clear(); motherWavelet.resize(N);
    const double PI = 3.141592653589793;

    switch (type) {
    case BasisType::ComplexShannon: {
        if (N % 4 != 0) {
            throw runtime_error("\nError in Complex Shannon Basis: N % 4 != 0 ...\n");
        }
        complex<double> value;
        scalingWavelet[0] = motherWavelet[0] = 1.0 / sqrt(2.0);

        for (int i = 1; i < N; i++) {
            value.real(sqrt(2.0) / N * cos(PI * i / N) * sin(PI * i / 2.0) / sin(PI * i / N));
            value.imag(-sqrt(2.0) / N * sin(PI * i / N) * sin(PI * i / 2.0) / sin(PI * i / N));
            scalingWavelet[i] = value;
            motherWavelet[i] = pow(-1, i) * value;
        }
        break;
    }
    case BasisType::DaubechiesD6: {
        if (N % 4 != 0) {
            throw runtime_error("\nError in Daubechies Basis: N % 4 != 0 ...\n");
        }
        double coef = sqrt(2) / 32;
        double a = 1 - sqrt(10), b = 1 + sqrt(10), c = sqrt(5 + 2 * sqrt(10));

        scalingWavelet[0] = coef * (b + c);
        scalingWavelet[1] = coef * (2 * a + 3 * b + 3 * c);
        scalingWavelet[2] = coef * (6 * a + 4 * b + 2 * c);
        scalingWavelet[3] = coef * (6 * a + 4 * b - 2 * c);
        scalingWavelet[4] = coef * (2 * a + 3 * b - 3 * c);
        scalingWavelet[5] = coef * (b - c);

        motherWavelet[0] = -scalingWavelet[1];
        motherWavelet[1] = scalingWavelet[0];
        motherWavelet[motherWavelet.size() - 1] = scalingWavelet[2];
        motherWavelet[motherWavelet.size() - 2] = -scalingWavelet[3];
        motherWavelet[motherWavelet.size() - 3] = scalingWavelet[4];
        motherWavelet[motherWavelet.size() - 4] = -scalingWavelet[5];
        break;
    }
    }
}

//--------------------------------------------------------------------------------------------------------------------

// Построение системы фильтров вейвлетов
// levels - количество уровней декомпозиции
void WaveletTransform::buildFilterSystem(int levels) {
    Operators op;
    int N = scalingWavelet.size();

    vector<vector<complex<double>>> scalingFilters(levels), motherFilters(levels);

    scalingFilters[0] = scalingWavelet;
    motherFilters[0] = motherWavelet;

    for (int i = 1; i < levels; i++) {
        int filterSize = N / int(pow(2, i));
        scalingFilters[i].resize(filterSize);
        motherFilters[i].resize(filterSize);

        for (int n = 0; n < filterSize; n++) {
            int maxIndex = int(pow(2, i));
            for (int k = 0; k < maxIndex; k++) {
                scalingFilters[i][n] += scalingFilters[0][n + k * N / maxIndex];
                motherFilters[i][n] += motherFilters[0][n + k * N / maxIndex];
            }
        }
    }

    FourierTransform dft;
    vector<complex<double>> tempScaling, tempMother;

    lowPassFilters.resize(levels);
    highPassFilters.resize(levels);

    lowPassFilters[0] = motherFilters[0];
    highPassFilters[0] = scalingFilters[0];

    for (int i = 1; i < levels; i++) {
        op.Upsampling_Operator(i, scalingFilters[i], tempScaling);
        op.Upsampling_Operator(i, motherFilters[i], tempMother);
        lowPassFilters[i] = dft.Convolution(highPassFilters[i - 1], tempMother);
        highPassFilters[i] = dft.Convolution(highPassFilters[i - 1], tempScaling);
    }
}

//--------------------------------------------------------------------------------------------------------------------

// Построение вейвлет-базиса
// level - текущий уровень декомпозиции
// highFreqComponents - высокочастотные компоненты
// lowFreqComponents  - низкочастотные компоненты
void WaveletTransform::constructWaveletBasis(int level,
    vector<vector<complex<double>>>& highFreqComponents,
    vector<vector<complex<double>>>& lowFreqComponents) {
    Operators op;
    int N = scalingWavelet.size();
    int basisSize = N / int(pow(2, level));

    if (highPassFilters.size() < level) {
        buildFilterSystem(level + 1);
    }

    highFreqComponents.resize(basisSize);
    lowFreqComponents.resize(basisSize);

    for (int i = 0; i < basisSize; i++) {
        int index = int(pow(2, level)) * i;
        op.Cyclic_Shift(index, lowPassFilters[level - 1], highFreqComponents[i]);
        op.Cyclic_Shift(index, highPassFilters[level - 1], lowFreqComponents[i]);
    }
}

//--------------------------------------------------------------------------------------------------------------------

// Декомпозиция сигнала
void WaveletTransform::decompose(int level, const vector<complex<double>>& signal,
    vector<complex<double>>& highFreqCoeffs,
    vector<complex<double>>& lowFreqCoeffs) {
    Operators op;
    vector<vector<complex<double>>> psi, phi;

    constructWaveletBasis(level, psi, phi);
    int basisSize = psi.size();

    for (int i = 0; i < basisSize; i++) {
        highFreqCoeffs.push_back(op.Dot_Product(signal, psi[i]));
        lowFreqCoeffs.push_back(op.Dot_Product(signal, phi[i]));
    }
}

//--------------------------------------------------------------------------------------------------------------------

// Реконструкция сигнала
void WaveletTransform::reconstruct(int level,
    const vector<complex<double>>& highFreqCoeffs,
    const vector<complex<double>>& lowFreqCoeffs,
    vector<complex<double>>& highProjections,
    vector<complex<double>>& lowProjections,
    vector<complex<double>>& reconstructedSignal) {
    vector<vector<complex<double>>> psi, phi;
    constructWaveletBasis(level, psi, phi);
    int basisSize = psi.size();
    int N = scalingWavelet.size();

    for (int i = 0; i < N; i++) {
        complex<double> highProjection(0, 0), lowProjection(0, 0);

        for (int j = 0; j < basisSize; j++) {
            highProjection += highFreqCoeffs[j] * psi[j][i];
            lowProjection += lowFreqCoeffs[j] * phi[j][i];
        }
        highProjections.push_back(highProjection);
        lowProjections.push_back(lowProjection);
        reconstructedSignal.push_back(highProjection + lowProjection);
    }
}





