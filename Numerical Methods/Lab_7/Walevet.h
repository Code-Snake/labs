#pragma once

#ifndef WAVELET_TRANSFORM_H
#define WAVELET_TRANSFORM_H

#include <vector>
#include <complex>
#include "Operators.h"
#include "Fourier_Transform.h"
using namespace std;

class WaveletTransform {
private:
    // Вейвлеты: масштабирующий и материнский
    vector<complex<double>> scalingWavelet, motherWavelet;
    // Векторы фильтров: порождают p-этапные вейвлет-базы
    vector<vector<complex<double>>> lowPassFilters, highPassFilters;

public:
    // Типы базисов
    enum class BasisType {
        // Комплексный базис Шеннона
        ComplexShannon = 1,
        DaubechiesD6 = 2,
    };

    // Конструктор: инициализация вейвлетов
    WaveletTransform(int signalLength, BasisType type);

private:
    // Построение системы фильтров вейвлетов
    void buildFilterSystem(int levels);

    // Создание вейвлет-базиса
    void constructWaveletBasis(int level,
        vector<vector<complex<double>>>& highFreqComponents,
        vector<vector<complex<double>>>& lowFreqComponents);

public:
    // Фаза анализа сигнала
    void decompose(int level,
        const vector<complex<double>>& signal,
        vector<complex<double>>& highFreqCoeffs,
        vector<complex<double>>& lowFreqCoeffs);

    // Фаза синтеза (восстановление сигнала)
    void reconstruct(int level,
        const vector<complex<double>>& highFreqCoeffs,
        const vector<complex<double>>& lowFreqCoeffs,
        vector<complex<double>>& highProjections,
        vector<complex<double>>& lowProjections,
        vector<complex<double>>& reconstructedSignal);

};

#endif
