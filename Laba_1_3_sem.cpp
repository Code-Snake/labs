#include <iostream>
#include <string>
#include <ctype.h>
#include <math.h>
#include <fstream>

using namespace std;

string second_part(string num) {
	string result = "0";
	for (int i = num.find("."); i < num.size(); i++) {
		result += num[i];
	}
	return result;
}

string second_part_to_base(long double num, int base) {
	string result;
	string first;
	string digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	while (num != 0) {
		num = num * base;
		for (int i = 0; i < to_string(num).find("."); i++) {
			first += to_string(num)[i];
		}
		result += digits[stoi(first)];
		num = num - stoi(first);
		first = "";
	}
	return result;
}

string convert_to_base(string num, int base) {
	bool flag = false;
	string result;
	long double second_part_num;
	int first_num;
	string result_base;
	string digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	if (num.find(".") < num.size()) {
		flag = true;
		second_part_num = stod(second_part(num));
		result_base = second_part_to_base(second_part_num, base);
	}
	first_num = stoi(num);
	while (first_num > 0) {
		result = digits[first_num % base] + result;
		first_num = first_num / base;
	}
	if (flag == true) {
		return result + "." + result_base;
	}
	else {
		return result + result_base;
	}
}

long double convert_second_part_to_decimal(string num, int base) {
	int j = -1;
	long double result = 0.0;
	for (int i = num.find(".") + 1; i < num.size(); i++) {
		int digit = num[i];
		if (isdigit(digit)) {
			result += (digit - '0') * pow(base, j);
		}
		else {
			result += (digit - 'A' + 10) * pow(base, j);
		}
		j--;
	}
	return result;
}

long double convert_to_decimal(string num, int base) {
	long double result = 0.0;
	int j = 0;
	string second_part_num;
	double result_second_part = 0.0;
	int start = num.size() - 1;
	if (num.find(".") < num.size()) {
		start = num.find(".") - 1;
		second_part_num = second_part(num);
		result_second_part = convert_second_part_to_decimal(second_part_num, base);
	}
	for (int i = start; i >= 0; i--) {
		int digit = num[j];
		if (isdigit(digit)) {
			result += (digit - '0') * pow(base, i);
		}
		else {
			result += (digit - 'A' + 10) * pow(base, i);
		}

		j++;
	}
	result += result_second_part;
	return result;
}

string base_sum(string num1, string num2, int base) {
	long double number1 = convert_to_decimal(num1, base);
	long double number2 = convert_to_decimal(num2, base);
	long double sum_result = number1 + number2;
	return convert_to_base(to_string(sum_result), base);
}
string base_multiplication(string num1, string num2, int base) {
	long double number1 = convert_to_decimal(num1, base);
	long double number2 = convert_to_decimal(num2, base);
	long double sum_result = number1 * number2;
	return convert_to_base(to_string(sum_result), base);
}

int main() {
	int checkbox;
	string num;
	string num2;
	int base;
	cout << "What do you want?" << endl;
	cout << "1 - convert to base " << endl;
	cout << "2 - convert to decimal" << endl;
	cout << "3 - base sum" << endl;
	cout << "4 - base multiplication" << endl;
	cout << "0 - leave" << endl;
	cin >> checkbox;
	while (checkbox != 0)
	{
		
		switch (checkbox) {
		case 1: {
			cout << "Enter the number: "; cin >> num;
			cout << "Enter the base: "; cin >> base;
			cout << "Answer: "<< convert_to_base(num, base) << endl;
			cout << endl;
			break;
			}
		case 2: {
			cout << "Enter the number: "; cin >> num;
			cout << "Enter the base: "; cin >> base;
			cout << "Answer: " << convert_to_decimal(num, base) << endl;
			cout << endl;
			break;
		}
		case 3: {

			cout << "Enter the first number: "; cin >> num;
			cout << "Enter the second number: "; cin >> num2;
			cout << "Enter the base: "; cin >> base;
			cout << "Answer: " << base_sum(num,num2, base) << endl;
			cout << endl;
			break;
		}
		case 4: {
			
			cout << "Enter the first number: "; cin >> num;
			cout << "Enter the second number: "; cin >> num2;
			cout << "Enter the base: "; cin >> base;
			cout << "Answer: " << base_multiplication(num, num2, base) << endl;
			cout << endl;
			break;
		}
			  
			
		}
		cout << "What next? Enter the number menu: "; cin >> checkbox;
	}
	cout << "Goodbye!";

}
