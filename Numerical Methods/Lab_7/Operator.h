#pragma once
#ifndef Complex_Operators_h
#define Complex_Operators_h

#include <complex>
#include <vector>
using namespace std;

class Operators {
public:
	//�������� ������
	//k - �������� ������, Data - ������� ������, Result - ������ ��� ���������
	void Cyclic_Shift(int k, const vector<complex<double>>& Data, vector<complex<double>>& Result);

	//�������� ��������� �������
	//l - ���������� �������, Data - ��������, Result - �����
	void Downsampling_Operator(int l, const vector<complex<double>>& Data, vector<complex<double>>& Result);

	//�������� ����������� �������
	//l - ���������� �������, Data - ��������, Result - �����
	void Upsampling_Operator(int l, const vector<complex<double>>& Data, vector<complex<double>>& Result);

	//��������� ������������ ���� ��������
	complex<double> Dot_Product(const vector<complex<double>>& Vec1, const vector<complex<double>>& Vec2);
};


#endif // ! Complex_Operators_h

