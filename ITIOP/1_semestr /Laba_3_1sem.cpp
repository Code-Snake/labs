#include <iostream> //Подключение библиотеки ввода и вывода
using namespace std; //Пространство имён
int main()
{
	setlocale(LC_ALL, "Ru"); //Подключение кириллицы в консоли
	float* X, * Y, * Answ, A[100][100];
	int n, stroka, stolbec, i;

	cout << "Введите размерность матрицы: \n"; cin >> n; //Вводим размерность векторов и квадратного массива

	X = new float[n]; //Задаем размерность вектору X n
	Y = new float[n]; //Задаем размерность вектору Y n

	Answ = new float[n](); //Задаем вектору который получится в ответе размерность n и заполняем его нулями

	for (int stroka = 0; stroka < n; stroka++) {
		cout << "Введите элементы " << stroka+1 << " строки матрицы A через Enter: \n"; // Заполняем квадратную матрицу построчно
		for (int stolbec = 0; stolbec < n; stolbec++) cin >> A[stroka][stolbec];
	}

	cout << "Введите элементы вектора X через Enter: \n";
	for (i = 0; i < n; i++) cin >> X[i]; //Вводим элементы вектора X через enter

	cout << "Введите элементы вектора Y через Enter: \n";
    
	for (i = 0; i < n; i++) cin >> Y[i]; //Вводим элементы вектора Y через enter
	for (stroka = 0; stroka < n; stroka++) {
		for (stolbec = 0; stolbec < n; stolbec++) Answ[stroka] += A[stroka][stolbec] * (X[stolbec] + Y[stolbec]); // По элементно умножаем i-ую строку квадратной матрицы на i-ый элемент суммы векторов X и Y и добавляем это значение в i-ый элемент изначально нулевой матрицы.
	}
	cout << "Получившийся вектор: ";
	for (i = 0; i < n; i++) cout << Answ[i] << "; "; //Выводим элементы получившегося вектора
	return 0;
}
