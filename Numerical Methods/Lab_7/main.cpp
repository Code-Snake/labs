#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "Walevet.h"  // Подключаем заголовочный файл для вейвлет-преобразования
using namespace std;
#define EPS 1e-10

// Функция для сохранения данных в файл
void save_to_file(const vector<complex<double>>& Z, const vector<complex<double>>& Z_rec,
	const vector<complex<double>>& highFreqCoeffs, const vector<complex<double>>& lowFreqCoeffs, string filename) {
	ofstream file(filename);
	if (file.is_open()) {
		// Заголовок для CSV файла
		file << "Index,Original Data,Filtered Data,Psi,Fi\n";
		int highFreqSize = highFreqCoeffs.size();
		int lowFreqSize = lowFreqCoeffs.size();

		// Запись данных в файл
		for (int i = 0; i < Z.size(); ++i) {
			file << i << "," << Z[i].real() << "," << Z_rec[i].real();

			// Запись коэффициентов высоких частот
			if (i < highFreqSize) {
				file << "," << highFreqCoeffs[i].real();
			}
			else {
				file << ",";  // Пустое значение, если индекс превышает размер фильтра
			}

			// Запись коэффициентов низких частот
			if (i < lowFreqSize) {
				file << "," << lowFreqCoeffs[i].real();
			}
			else {
				file << ",";  // Пустое значение, если индекс превышает размер фильтра
			}
			file << "\n";
		}
		file.close();
		cout << "Data saved to " << filename << endl;
	}
	else {
		cout << "Unable to open file!" << endl;
	}
}


int main() {
	setlocale(LC_ALL, "Rus");  // Устанавливаем локаль для корректного отображения русского текста
	int N = 512;  // Размер сигнала
	double alpha = 1.87, beta = 2.47;  // Параметры для формирования исходного сигнала
	int Stage = 1;  // Этап разложения
	string basis = "DaubechiesD6"; // Тип базиса (Добаеши D6)

	// Сигнал и коэффициенты разложения
	vector<complex<double>> Z(N), highFreqCoeffs, lowFreqCoeffs;
	// Частичное восстановление, проекции и восстановленный сигнал
	vector<complex<double>> P, Q;
	vector<complex<double>> highProjections, lowProjections, Z_Rec;

	// Формирование исходного сигнала
	for (int n = 0; n < N; n++) {
		if (n >= 128 && n <= 255) Z[n]._Val[0] = sin(fabs(pow(n - 128, alpha)) / 128.0);
		if (n >= 384 && n <= 447) Z[n]._Val[0] = sin(fabs(pow(n - 128, beta)) / 128.0);
	}

	// Создаем объект для вейвлет-преобразования
	WaveletTransform waveletTransform(N, WaveletTransform::BasisType::DaubechiesD6);
	waveletTransform.decompose(Stage, Z, highFreqCoeffs, lowFreqCoeffs);  // Разложение сигнала
	waveletTransform.reconstruct(Stage, highFreqCoeffs, lowFreqCoeffs, P, Q, Z_Rec);  // Восстановление сигнала

	// Сохраняем данные в CSV файл
	save_to_file(Z, Z_Rec, highFreqCoeffs, lowFreqCoeffs, "data.csv");

	// Вызываем Python-скрипт для построения графиков
	string command = "python plot.py " + basis + " " + to_string(Stage);
	system(command.c_str());

	// Печать исходного сигнала и восстановленного сигнала
	int SETW = 22;
	cout << left << setw(SETW) << "Number" << setw(SETW) << "Z" << setw(SETW) << "Z_Recovery" << endl;
	for (int i = 0; i < Z.size(); i++) {
		cout << left << setw(SETW) << i
			<< setw(SETW) << Z[i].real()
			<< setw(SETW) << Z_Rec[i].real() << endl;
	}

	// Печать коэффициентов вейвлетов
	cout << endl << left << setw(SETW) << "Number" << setw(SETW) << "|HighFreq|" << setw(SETW) << "|LowFreq|" << endl;
	for (int i = 0; i < lowFreqCoeffs.size(); i++) {
		cout << left << setw(SETW) << pow(2, Stage) * i
			<< setw(SETW) << sqrt(pow(highFreqCoeffs[i].real(), 2) + pow(highFreqCoeffs[i].imag(), 2))
			<< setw(SETW) << sqrt(pow(lowFreqCoeffs[i].real(), 2) + pow(lowFreqCoeffs[i].imag(), 2)) << endl;
	}


	if (Stage == 1) {
		vector<complex<double>> P0(P.size());
		vector<complex<double>> E(P.size());
		double max_error = 0;
		for (int i = 0; i < P.size(); i++) {
			P0[i] = P[i] + Q[i];  // Полное восстановление
			E[i] = P[i] - P0[i];  // Ошибка восстановления
			max_error = max(max_error, abs(E[i]));  // Находим максимальную ошибку
		}
		save_to_file(Z, Z_Rec, P, P0, "data.csv");  // Сохраняем данные в CSV
		string command = "python plot.py " + basis + " " + to_string(Stage);  // Вызов Python-скрипта
		system(command.c_str());

		// Печать первых 10 образцов для P0 и ошибки
		cout << "P0 example first 10 samples" << endl;
		cout << endl << left << setw(SETW) << "Index" << setw(SETW) << "Z" << setw(SETW) << "P0" << setw(SETW) << "Err" << endl;
		for (int i = 0; i < 10; i++) {
			cout << left << setw(SETW) << i << setw(SETW) << Z[i] << setw(SETW) << P0[i] << setw(SETW) << E[i] << endl;
		}
		// Вывод ошибки восстановления
		cout << "Ошибка: " << max_error << endl;
	}

	return 0;
}
