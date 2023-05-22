#include <iostream>
using namespace std;

int main()
{
    setlocale(LC_ALL, "Ru");
    double x, result=1.0; int i, xx;
    cin >> x;
    xx = x;
    if (xx < 64 and xx % 2 == 1 and xx==x) cout << "Невозможно вычислить ответ, так как знаменатель равен 0";
    else {
        for (i = 2; i <= 64; i *= 2) {
            result *= (x - i) / (x - i + 1);
            cout << "числитель умножается на: " << (x-i) << " знаменатель умножается на: " << (x-i+1) << "\n";
        }
        cout << result;
    }
}