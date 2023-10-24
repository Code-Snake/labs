#include "Mtrx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;


Matrix::Matrix(int r, int c) : rows(r), cols(c), data(r, vector<float>(c, 0)) {}


void Matrix::print() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cout<< data[i][j]<<" ";
		}
		cout << endl;
	}
}

Matrix Matrix::loadFromFile(const string& filename) {
	ifstream file(filename);
	if (!file.is_open()) {
		cout << "Error: file not open" << endl;
	}
	else {
		Matrix M(0, 0);
		string line;
		int r = 0; int c = 0;
		while (getline(file, line)) {
			vector<float> row;
			istringstream iss(line);
			int value;
			r += 1;
			while (iss >> value) {
				row.push_back(value);
				c += 1;
			}
			M.data.push_back(row);
		}
		M.rows = r;
		M.cols = c / r;
		return M;
	}
}

void Matrix::multByScalar(float scalar) {
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			data[r][c] = data[r][c] * scalar;
		}
	}
}

Matrix Matrix::Transposition() {
	Matrix result(cols, rows);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			result.data[j][i] = data[i][j];
		}
	}
	return result;
}

float Matrix::calculateDeterminant() {
	if (rows != cols) {
		cout << "Error. Not square matrix" << endl;
		return 0;
	}
	int n = rows;

	if (n == 1) {
		return data[0][0]; // Определитель матрицы 1x1
	}

	if (n == 2) {
		// Определитель матрицы 2x2
		return data[0][0] * data[1][1] - data[0][1] * data[1][0];
	}

	float det = 0;
	for (int i = 0; i < n; i++) {   // Разложение по первой строке (можно выбрать любую строку или столбец)

		Matrix submat(n - 1, n - 1);

		int submat_r = 0;
		for (int j = 1; j < n; j++) {
			int submat_c = 0;
			for (int k = 0; k < n; k++) {
				if (k != i) {
					submat.data[submat_r][submat_c] = data[j][k];
					submat_c++;
				}
			}
			submat_r++;
		}


		float subdet = submat.calculateDeterminant(); // Рекурсивно вычисляем определитель подматрицы


		if (i % 2 == 0) {
			det += data[0][i] * subdet; // Знак определителя меняется при каждой следующей итерации (чередование "+-" или "-+")
		}
		else {
			det -= data[0][i] * subdet;
		}
	}

	return det;
}

Matrix Matrix::inverse() {
	float det = calculateDeterminant();
	if (det == 0) {
		cout << "Error. Determinant = 0";
		return *this;  // Возвращаем текущую матрицу, так как обратной не существует
	}

	int n = rows;
	Matrix result(n, n);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			
			Matrix submat(n - 1, n - 1);

			int submat_r = 0;
			for (int k = 0; k < n; k++) {
				if (k != i) {
					int submat_c = 0;
					for (int m = 0; m < n; m++) {
						if (m != j) {
							submat.data[submat_r][submat_c] = data[k][m];
							submat_c++;
						}
					}
					submat_r++;
				}
			}

			float sub_det = submat.calculateDeterminant();

			result.data[i][j] = pow(-1, i + j) * sub_det;

		}
	}

	result = result.Transposition();  // Транспонируем матрицу
	result.multByScalar(1 / det); // Делим на определитель


	return result;
}

