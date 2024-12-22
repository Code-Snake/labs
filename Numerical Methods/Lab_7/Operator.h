#pragma once
#ifndef Complex_Operators_h
#define Complex_Operators_h

#include <complex>
#include <vector>
using namespace std;

class Operators {
public:
	//оператор сдвига
	//k - величина сдвига, Data - входные данные, Result - массив под результат
	void Cyclic_Shift(int k, const vector<complex<double>>& Data, vector<complex<double>>& Result);

	//оператор сгущающей выборки
	//l - показатель степени, Data - прообраз, Result - образ
	void Downsampling_Operator(int l, const vector<complex<double>>& Data, vector<complex<double>>& Result);

	//оператор разрежающей выборки
	//l - показатель степени, Data - прообраз, Result - образ
	void Upsampling_Operator(int l, const vector<complex<double>>& Data, vector<complex<double>>& Result);

	//скалярное произведение двух векторов
	complex<double> Dot_Product(const vector<complex<double>>& Vec1, const vector<complex<double>>& Vec2);
};


#endif // ! Complex_Operators_h

