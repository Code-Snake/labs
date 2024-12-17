#include <iostream>
#include <iomanip>
#include <vector>
#include <complex>
#include <cmath>
#include <chrono>
#include <fstream>
#include "Fourier_Transform.h"

using namespace std;
using namespace chrono;

// Глобальная переменная для проверки близости чисел к нулю
double epsilon = 1e-7;

// Функция для сохранения данных в файл
void saveToFile(const vector<complex<double>>& originalData, const vector<complex<double>>& filteredData, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        file << "Idx,Data,F_Data\n";
        for (size_t i = 0; i < originalData.size(); i++) {
            file << i << "," << originalData[i].real() << "," << filteredData[i].real() << "\n";
        }
        file.close();
        cout << "Данные сохранены в файл: " << filename << endl;
    }
    else {
        cout << "Не удалось открыть файл!" << endl;
    }
}

// Функция форматирования значений (обнуление чисел, близких к нулю)
double formatValue(double value) {
    return (fabs(value) < epsilon) ? 0.0 : value;
}

// Функция для анализа спектра сигнала
void analyzeSignal(const vector<complex<double>>& spectrum) {
    int N = spectrum.size();
    int significantFrequencies = 0; // Количество значимых частот
    double threshold = 1e-3;        // Порог значимости

    cout << "Частота\t\tАмплитуда" << endl;
    for (int i = 0; i < N; i++) {
        double amplitude = abs(spectrum[i]);
        if (amplitude > threshold) {
            significantFrequencies++;
            cout << i << "\t\t" << amplitude << endl;
        }
    }

    cout << "\nОбщее количество значимых частот: " << significantFrequencies << endl;

    // Определение, является ли сигнал низкочастотным
    if (significantFrequencies < N / 4) {
        cout << "Сигнал низкочастотный." << endl;
    }
    else {
        cout << "Сигнал высокочастотный." << endl;
    }
}

// Главная функция
int main() {
    setlocale(LC_ALL, "Rus");
    const int N = 512;                 // Размер данных
    const double a = 87, b = -0.53;    // Параметры сигнала
    const double omega = 491;          // Частота
    const double phi = PI / 4;         // Фаза

    // Создание исходных данных (сигнал с шумом)
    vector<complex<double>> data(N);
    for (int i = 0; i < N; i++) {
        data[i] = a + b * cos((2 * PI * omega * i) / N + phi);
    }

    // Создание объекта преобразования Фурье
    FourierTransform transformer;
    transformer.updateData(data);

    // Выполнение ДПФ (дискретного преобразования Фурье)
    auto startDFT = high_resolution_clock::now();
    auto DFTResult = transformer.computeDFT();
    auto endDFT = high_resolution_clock::now();
    auto durationDFT = duration_cast<microseconds>(endDFT - startDFT);

    // Выполнение БПФ (быстрого преобразования Фурье)
    auto startFFT = high_resolution_clock::now();
    auto FFTResult = transformer.computeFFT();
    auto endFFT = high_resolution_clock::now();
    auto durationFFT = duration_cast<microseconds>(endFFT - startFFT);

    // Вывод результатов ДФТ
    const int SETW = 15;
    cout << left << setw(SETW) << "Номер"
        << left << setw(SETW) << "Re(data)"
        << left << setw(SETW) << "Re(DFT)"
        << left << setw(SETW) << "Im(DFT)"
        << left << setw(SETW) << "Амплитуда"
        << left << setw(SETW) << "Фаза (рад)" << endl;

    for (int i = 0; i < N; i++) {
        double reData = formatValue(data[i].real());
        double reDFT = formatValue(DFTResult[i].real());
        double imDFT = formatValue(DFTResult[i].imag());
        double amplitude = formatValue(abs(DFTResult[i]));
        double phase = formatValue(arg(DFTResult[i]));

        // Проверяем, есть ли ненулевые значения
        if ( reDFT != 0.0 || imDFT != 0.0 || amplitude != 0.0 ) {
            cout << left << setw(SETW) << i
                << left << setw(SETW) << reData
                << left << setw(SETW) << reDFT
                << left << setw(SETW) << imDFT
                << left << setw(SETW) << amplitude
                << left << setw(SETW) << phase << endl;
        }
    }

    // Вывод результатов FFT
    cout << endl;
    cout << left << setw(SETW) << "Номер"
        << left << setw(SETW) << "Re(data)"
        << left << setw(SETW) << "Re(FFT)"
        << left << setw(SETW) << "Im(FFT)"
        << left << setw(SETW) << "Амплитуда"
        << left << setw(SETW) << "Фаза (рад)" << endl;

    for (int i = 0; i < N; i++) {
        double reData = formatValue(data[i].real());
        double reFFT = formatValue(FFTResult[i].real());
        double imFFT = formatValue(FFTResult[i].imag());
        double amplitude = formatValue(abs(FFTResult[i]));
        double phase = formatValue(arg(FFTResult[i]));

        // Проверяем, есть ли ненулевые значения
        if (reFFT != 0.0 || imFFT != 0.0 || amplitude != 0.0) {
            cout << left << setw(SETW) << i
                << left << setw(SETW) << reData
                << left << setw(SETW) << reFFT
                << left << setw(SETW) << imFFT
                << left << setw(SETW) << amplitude
                << left << setw(SETW) << phase << endl;
        }
    }
    // Анализ спектра
    cout << "\nАнализ спектра DFT..." << endl;
    analyzeSignal(DFTResult);

    cout << "\nАнализ спектра FFT..." << endl;
    analyzeSignal(FFTResult);

    // Вывод времени выполнения
    cout << "\nВремя выполнения DFT: " << durationDFT.count() << " мкс" << endl;
    cout << "Время выполнения FFT: " << durationFFT.count() << " мкс" << endl;
    for (int i = 0; i < N; i++) {
        data[i]._Val[0] = cos((2 * PI * i) / N) + 0.01 * cos((2 * PI * omega * i) / N);
        data[i]._Val[1] = 0;
    }
    transformer.updateData(data);
    DFTResult = transformer.computeDFT();
    cout << endl;
    analyzeSignal(DFTResult);
    // Фильтрация сигнала (обнуление частотных компонентов)
    DFTResult[21] = { 0, 0 };
    DFTResult[491] = { 0, 0 };
    transformer.updateData(DFTResult);
    auto IDFTResult = transformer.computeIDFT();

    // Сравнение исходных и отфильтрованных данных
    cout << "\nСравнение исходных данных и IDFT от модифицированного результата DFT (первые 15 значений):" << endl;
    cout << setw(SETW) << "Индекс" << setw(SETW) << "Исходные данные" << setw(SETW) << "Фильтрованные данные" << endl;
    for (int i = 0; i < 15; i++) {
        cout << setw(SETW) << i
            << setw(SETW) << formatValue(data[i].real())
            << setw(SETW) << formatValue(IDFTResult[i].real()) << endl;
    }

    // Сохранение результатов в файл
    saveToFile(data, IDFTResult, "data.csv");

    return 0;
}
