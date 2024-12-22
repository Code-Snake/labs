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
    // ��������: �������������� � �����������
    vector<complex<double>> scalingWavelet, motherWavelet;
    // ������� ��������: ��������� p-������� �������-����
    vector<vector<complex<double>>> lowPassFilters, highPassFilters;

public:
    // ���� �������
    enum class BasisType {
        // ����������� ����� �������
        ComplexShannon = 1,
        DaubechiesD6 = 2,
    };

    // �����������: ������������� ���������
    WaveletTransform(int signalLength, BasisType type);

private:
    // ���������� ������� �������� ���������
    void buildFilterSystem(int levels);

    // �������� �������-������
    void constructWaveletBasis(int level,
        vector<vector<complex<double>>>& highFreqComponents,
        vector<vector<complex<double>>>& lowFreqComponents);

public:
    // ���� ������� �������
    void decompose(int level,
        const vector<complex<double>>& signal,
        vector<complex<double>>& highFreqCoeffs,
        vector<complex<double>>& lowFreqCoeffs);

    // ���� ������� (�������������� �������)
    void reconstruct(int level,
        const vector<complex<double>>& highFreqCoeffs,
        const vector<complex<double>>& lowFreqCoeffs,
        vector<complex<double>>& highProjections,
        vector<complex<double>>& lowProjections,
        vector<complex<double>>& reconstructedSignal);

};

#endif
