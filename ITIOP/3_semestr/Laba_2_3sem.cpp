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

string bin_to_hex(string bin_num, int type) {
	string result;
	string cuarto;
	string dec_num,hex_num;
	for (int i = 0; i < type*8; i+=4) {
		cuarto = bin_num.substr(i, 4);
		dec_num = to_string(convert_to_decimal(cuarto,2));
		hex_num = convert_to_base(dec_num, 16);
		if (hex_num == ".") {
			result += "0";
		}
		else {
			hex_num.erase(hex_num.find("."), 1);
			result += hex_num;
		}
	}
	return result;
}

string convert_to_base(string num, int base) {
	bool flag = false, flag_minus = false;
	string result;
	long double second_part_num;
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
	unsigned long long int first_num= strtoull(num.c_str(), &pEnd, 10);

	while (first_num > 0) {
		
		result = digits[first_num % base] + result;
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

Number normalize_number(string num, int byte_types) {
	Number result;
	num = to_string(abs(stod(num)));
	if (num.find(".") < num.size()) {
		string bin_num = convert_to_base(num, 2);
		if (byte_types == 1) {
			if (bin_num.size() > 54) {
				bin_num.erase(54, (bin_num.size() - 54));
			}
		}
		else {
			if (bin_num.size() > 25) {
				bin_num.erase(25, (bin_num.size() - 25));
			}
		}
		
		if (num.find("0") == 0) {
			string exponent = to_string(bin_num.find("1"));
			bin_num.erase(1, bin_num.find("1")-1);
			result.exponent = "-"+exponent;
		}
		else {
			string exponent = to_string(bin_num.find(".") - 1);
			result.exponent = exponent;
		}
		int dotpos = bin_num.find(".");
	
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

bytes_number decimal_to_byte_four(string num) {
	string bin_num;
	bytes_number result;
	string  fourbyte, eightbyte;
	string base_num = convert_to_base(to_string(abs(stod(num))), 2);
	string normalize_num = normalize_number(num, 0).num;
	string exponent = normalize_number(num, 0).exponent;
	string offset = convert_to_base(to_string(stoi(exponent) + 127), 2);
	
	if (num.find("-") < num.size()) {
		bin_num = "1" + offset + normalize_num.erase(0, 2);
	}
	else {
		bin_num = "0" + offset + normalize_num.erase(0, 2);
	}
	if (num.find("0") == 0) {
		bin_num = "0" + bin_num;
	}
	while (bin_num.size() != 32) {
		bin_num += "0";
	}
	result.hex_num = bin_to_hex(bin_num, 4);
	fourbyte = bin_num;
	result.fourbyte = fourbyte;

	return result;
}
bytes_number decimal_to_byte_eight(string num) {
	string bin_num;
	bytes_number result;
	string  fourbyte,eightbyte, hex_num;
	string base_num = convert_to_base(to_string(abs(stod(num))), 2);
	string normalize_num = normalize_number(num, 1).num;
	string exponent = normalize_number(num, 1).exponent;
	string offset = convert_to_base(to_string(stoi(exponent) + 1023), 2);
	
	if (num.find("-") < num.size()) {
		bin_num = "1" + offset + normalize_num.erase(0, 2);
	}
	else {
		bin_num = "0" + offset + normalize_num.erase(0, 2);
	}
	if (num.find("0") == 0) {
		bin_num = "0" + bin_num;
	}
	while (bin_num.size() != 64) {
		bin_num += "0";
	}

	result.hex_num = bin_to_hex(bin_num, 8);
	eightbyte = bin_num;
	result.eightbyte = eightbyte;

	return result;
}



long double byte_to_decimal(string num) {
	string binary_string, sing_bit, exponent_bits, offset_bits;
	long double result;
	long double offset;
	int exponent;

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
long double byte_to_decimal_four(string num) {
	string binary_string, sing_bit, exponent_bits, offset_bits;
	long double result;
	long double offset;
	int exponent;

	binary_string = hex_to_binary(num);

	while (binary_string.size() != 32) {
		binary_string = '0' + binary_string;
	}
	sing_bit = binary_string[0];
	exponent_bits = binary_string.substr(1, 8);
	offset_bits = binary_string.erase(0, 9);
	exponent = convert_to_decimal(exponent_bits, 2) - 127;
	offset = 1.0;
	for (int i = 0; i < 23; i++) {
		offset += stod(string(1, offset_bits[i])) * pow(2, -(i + 1));

	}

	result = pow(-1, stod(sing_bit)) * pow(2, exponent) * convert_to_decimal("1." + offset_bits, 2);
	return result;
}
int main() {
	string num1 = "0.05"; 
	string num2 = "-193.789";
	string num3 = "91.4844";
	string num4 = "115.617";
	cout << "|...............................TO 8 bytes......................................|" << endl;
	bytes_number result = decimal_to_byte_eight(num1);
	cout << result.hex_num << " " << result.eightbyte << endl;
	result = decimal_to_byte_eight(num2);
	cout << result.hex_num << " " << result.eightbyte << endl;
	result = decimal_to_byte_eight(num3);
	cout << result.hex_num << " " << result.eightbyte << endl;
	result = decimal_to_byte_eight(num4);
	cout << result.hex_num << " " << result.eightbyte << endl;
	cout << "|...............................TO 4 bytes......................................|" << endl;
	result = decimal_to_byte_four(num1);
	cout << result.hex_num << " " << result.fourbyte << endl;
	result = decimal_to_byte_four(num2);
	cout << result.hex_num << " " << result.fourbyte << endl;
	result = decimal_to_byte_four(num3);
	cout << result.hex_num << " " << result.fourbyte << endl;
	result = decimal_to_byte_four(num4);
	cout << result.hex_num << " " << result.fourbyte << endl;
	cout << "|...............................FROM 4 bytes......................................|" << endl;
	string hex_num1 = "C39E8800";
	string hex_num2 = "43F37000";
	cout << byte_to_decimal_four(hex_num1) << endl;
	cout << byte_to_decimal_four(hex_num2) << endl;
	cout << "|...............................FROM 8 bytes......................................|" << endl;
	string hex_num3 = "405AC90000000000";
	string hex_num4 = "4070B88000000000";

	cout << byte_to_decimal(hex_num3) << endl;
	cout << byte_to_decimal(hex_num4) << endl;
	return 0;
}
