#include <iostream>
#include <string>
#include <ctype.h>
#include <math.h>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

string second_part(string num);
string second_part_to_base(long double num, int base);
string convert_to_base(string num, int base);
long double convert_second_part_to_decimal(string num, int base);
long double convert_to_decimal(string num, int base);
long double byte_to_decimal(string num);

struct Number {
	string num;
	string exponent;
};

struct bytes_number {
	string fourbyte;
	string eightbyte;
	string hex_num;
};

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
		result += digits[stod(first)];
		num = num - stod(first);
		first = "";
	}
	return result;
}

string hex_to_binary(string hex) {
	string binary = "";
	for (char c : hex) {
		int num;
		if (c >= '0' && c <= '9') {
			num = c - '0';
		}
		else if (c >= 'A' && c <= 'F') {
			num = c - 'A' + 10;
		}
		else if (c >= 'a' && c <= 'f') {
			num = c - 'a' + 10;
		}
		else {
			return "";
		}
		string binaryDigit = "";
		while (num > 0) {
			binaryDigit = to_string(num % 2) + binaryDigit;
			num /= 2;
		}
		while (binaryDigit.length() < 4) {
			binaryDigit = "0" + binaryDigit;
		}
		binary += binaryDigit;
	}
	return binary;
}

string convert_to_base(string num, int base) {
	bool flag = false, flag_minus = false;
	string result;
	long double second_part_num;
	long int first_num;
	string result_base;
	string digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	if (num.find("-") < num.size()) {
		flag_minus = true;
		num.erase(0, 1);
	}
	if (num.find(".") < num.size()) {
		flag = true;
		second_part_num = stod(second_part(num));
		result_base = second_part_to_base(second_part_num, base);
	}
	char* pEnd;
	first_num = strtol(num.c_str(),&pEnd,10);
	//cout << "first " << first_num << endl;
	while (first_num > 0) {
		
		result = digits[first_num % base] + result;
		//cout <<result<<" "<< first_num % base << endl;
		first_num = first_num / base;
		
	}
	if (flag == true) {
		if (flag_minus == true) {
			return "-" + result + "." + result_base;
		}
		return result + "." + result_base;
	}
	else {
		if (flag_minus == true) {
			return "-" + result + result_base;
		}
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
	bool flag_minus = false;
	long double result = 0.0;
	int j = 0;
	string second_part_num;
	double result_second_part = 0.0;
	if (num.find("-") < num.size()) {

		flag_minus = true;
		num.erase(0, 1);
	}
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
	if (flag_minus) {
		result = -result;
	}
	return result;
}

Number normalize_number(string num) {
	Number result;
	num = to_string(abs(stod(num)));
	
	if (num.find(".") < num.size()) {
		string bin_num = convert_to_base(num, 2);
		
		if (bin_num.size() > 54) {
			bin_num.erase(54, (bin_num.size() - 54));
		}
		int dotpos = bin_num.find(".");
		string exponent = to_string(bin_num.find(".") - 1);
		result.exponent = exponent;
		bin_num.erase(dotpos, 1);
		bin_num.insert(1, ".");
		result.num = bin_num;
		return result;
	}
	else {
		result.num = num;
		result.exponent = "0";
		return result;
	}
}

bytes_number decimal_to_byte(string num) {
	string bin_num;
	bytes_number result;
	string fourbyte, eightbyte, decimal_num, hex_num;
	string base_num = convert_to_base(to_string(abs(stod(num))), 2);
	string normalize_num = normalize_number(num).num;
	
	string exponent = normalize_number(num).exponent;
	string offset = convert_to_base(to_string(stoi(exponent) + 1023), 2);

	if (num.find("-") < num.size()) {
		bin_num = "1" + offset + normalize_num.erase(0, 2);
	}
	else {
		bin_num = "0" + offset + normalize_num.erase(0, 2);
	}
	while (bin_num.size() % 4 != 0) {
		bin_num += "0";
	}
	
	
	decimal_num = to_string(convert_to_decimal(bin_num, 2));
	stringstream ss;
	char* pEnd;
	unsigned long long int long_decimal_num = strtoull(decimal_num.c_str(), &pEnd, 10);
	ss << hex << long_decimal_num;
	hex_num = ss.str();
	
	
	transform(hex_num.begin(), hex_num.end(), hex_num.begin(), [](unsigned char c) { return toupper(c); });
	if (hex_num.find(".") < hex_num.size()) {
		hex_num.erase(hex_num.find("."), 1);
	}
	
	if (bin_num.size() < 64) {
		while (bin_num.size() != 64) {
			bin_num += "0";
		}
	}
	
	while (hex_num.size() != 16) {
		hex_num += "0";
	}

	result.hex_num = hex_num;
	eightbyte = bin_num;
	//result.fourbyte = fourbyte;
	result.eightbyte = eightbyte;
	return result;
}

long double byte_to_decimal(string num) {
	string binary_string, sing_bit, exponent_bits, offset_bits;
	long double result;
	long double offset;
	int exponent;
	while (num.size() != 16) {
		num += "0";
	}
	binary_string = hex_to_binary(num);
	while (binary_string.size() != 64) {
		binary_string = '0' + binary_string;
	}
	sing_bit = binary_string[0];
	exponent_bits = binary_string.substr(1, 11);
	offset_bits = binary_string.erase(0, 12);
	exponent = convert_to_decimal(exponent_bits, 2) - 1023;
	offset = 1.0;
	for (int i = 0; i < 52; i++) {
		offset += stod(string(1, offset_bits[i])) * pow(2, -(i + 1));
	}
	result = pow(-1, stod(sing_bit)) * pow(2, exponent) * convert_to_decimal("1." + offset_bits, 2);
	return result;
}

int main() {
	string num1 = "-116.375";
	string num2 = "379.938";
	string num3 = "-276.438";
	string num4 = "115.617";
	bytes_number result = decimal_to_byte(num1);
	cout << result.hex_num << " " << result.eightbyte << endl;
	result =  decimal_to_byte(num2);
	cout << result.hex_num << " " << result.eightbyte << endl;
	result =  decimal_to_byte(num3);
	cout << result.hex_num << " " << result.eightbyte << endl;
	result =  decimal_to_byte(num4);
	cout << result.hex_num << " " << result.eightbyte << endl;
	cout << "|.....................................................................|" << endl;
	string hex_num1 = "C30BF000";
	string hex_num2 = "43F37000";
	string hex_num3 = "C06F558000000000";
	string hex_num4 = "4070B88000000000";
	cout << byte_to_decimal(hex_num1) << endl;
	cout << byte_to_decimal(hex_num2) << endl;
	cout << byte_to_decimal(hex_num3) << endl;
	cout << byte_to_decimal(hex_num4) << endl;
	return 0;
}
