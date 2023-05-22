#include <iostream> //Подключение библиотеки ввода и вывода
#include <cmath>
using namespace std; //Пространство имён
float S = 0;
void rast(float mas1[], float mas2[]) {
	S += (sqrt((mas2[0] - mas1[0]) * (mas2[0] - mas1[0]) + (mas2[1] - mas1[1]) * (mas2[1] - mas1[1])));
}
int main()
{
	setlocale(LC_ALL, "Ru"); //Подключение кириллицы в консоли
	const int kolvo = 10;
	int i;
	float mas[kolvo][2];
	for (i = 0; i < kolvo; i++) {
		cout << "Введите 2 координаты "<<i + 1<<"-ой вершины через Enter\n";
		cin >> mas[i][0];
		cin >> mas[i][1];
	}
	for (i = 0; i < kolvo - 1; i++) {
		rast(mas[i], mas[i + 1]);
	}
	rast(mas[0], mas[kolvo - 1]);
	cout << "Периметр получившегося десятиугольника равен: " « S;
	return 0;
}
