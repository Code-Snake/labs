#include <iostream> //Подключение библиотеки ввода и вывода
using namespace std; //Пространство имён
int main()
{
	setlocale(LC_ALL, "Ru"); //Подключение кириллицы в консоли
	int count = 0, n=0, probel = 0, probcount=0;
	bool flag = 1;
	char mas[440];
	
	cout << "Введите последовательность слов через пробел(не более 40 слов по 10 символов). Чтобы закончить поседовательность, поставьте точку в конце. \n";
	 //Вводим первый символ
	scanf_s("%c", &mas[n]);
	while (mas[n] != '.' and flag!=0) { //Пока символ не равен точке, будем выполнять цикл
		n++;
		scanf_s("%c", &mas[n]); //Вводим новый символ
		if (mas[n] == ' ') {
			if ((n - probel) <= 10) probel = n; 
			else flag = 0;
			probcount++;
			if (probcount > 39) flag = 0;
		};
	}
	if ((n - probel) > 10) flag=0;

	for (int i = 0; i <= n and flag!=0; i++) {
		if ((mas[i] == 'a') and (mas[i+1] == 'b') and (mas[i+2]=='c')) count++;
	}
	if (flag == 0) cout << "Некорректный ввод данных.";
	else cout << "Количество вхождений группы abc в последовательность:" << count; //Выводим количество вхождений группы abc в последовательность
	return 0;
}