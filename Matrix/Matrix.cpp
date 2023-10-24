
#include <iostream>
#include "Mtrx.h"
using namespace std;

void menu(Matrix matrix) {
	int checkbox;
	cout << "Меню: " << endl;
	cout << "1 - Умножить матрицу на число" << endl;
	cout << "2 - Транспонировать матрицу" << endl;
	cout << "3 - Найти определитель матрицы" << endl;
	cout << "4 - Найти обратную матрицу" << endl;
	cout << "5 - Вывести текущую матрицу" << endl;
	cout << "0 - Завершить программу" << endl;
	cin >> checkbox;
	do {
		if (checkbox == 0) { return; }
		else {
			switch (checkbox) {
			case 1: {
				float scalar;
				cout << "Введите число: "; cin >> scalar;
				matrix.multByScalar(scalar);
				matrix.print();
				break;
			}
			case 2: {
				matrix.Transposition();
				cout << "Транспонированная матрица: " << endl;
				matrix.print();
				break;
			}
			case 3: {
				cout << "Определитель матрицы:  " << matrix.calculateDeterminant()<<endl;
				break;
			}
			case 4: {
				cout << "Обратная матрица:  " << endl;
				matrix.inverse().print();
				break;
			}
			case 5: {
				cout << "Текущая матрица: " << endl;
				matrix.print();
				break;
			}
			}
			cout << "----------------------------------------" << endl;
			cout << "Какое следующее действие? "; cin >> checkbox;
		}
	} while (checkbox != 0);
}

int main() {
    setlocale(LC_ALL, "Ru");

    Matrix matrix=matrix.loadFromFile("matrix.txt");

	menu(matrix);

    return 0;
}


