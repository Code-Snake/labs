#include <iostream>
#include <fstream>
using namespace std;

struct grades // предметы и оценки
{
    char subject[20]; // название предмета
    int mark; // оценка за него
};
struct Course 
{
    char surname[20]; //фамилия
    char name[20]; //имя
    char patronym[20]; //отчество
    char sex; //пол
    grades grades_stud[5]; 
};
struct group // структура для группы
{
    char surname[20]; 
    char name[20]; 
    char patronym[20];
};

int main()
{
    setlocale(LC_ALL, "RUS");
    int StudNow = 0, k = 20, k2 = 8, max = -1, min = k2 * 5, minId = 0, maxId = 0, lastID=0;
    int SubjGr[]={0,0,0,0,0};//для счёта оценок группы
    Course inf1[100]; 
    group inf2[100];


    ifstream fin1("F1.txt"); //открытие файлов
    ifstream fin2("F2.txt");

    if (!fin1.is_open() and k<100) cout << "Файл не открыт.Ошибка"; //ошибка
    else {
        for (int i=0; i < k; i++) {
            fin1.getline(inf1[StudNow].surname, 20, ' '); //считывание ФИО, пола
            fin1.getline(inf1[StudNow].name, 20, ' ');
            fin1.getline(inf1[StudNow].patronym, 20, ' ');
            (fin1 >> inf1[StudNow].sex).get();
            for (int j = 0; j < 5; j++)   //считывание предметов с оценками
            {
                fin1.getline(inf1[StudNow].grades_stud[j].subject, 20, ' '); //  j-ый предмет
                (fin1 >> inf1[StudNow].grades_stud[j].mark).get(); //  j-ая оценка
            }
            StudNow++;

        }

    }
    int StudNow1 = 0;

    if (!fin2.is_open() and k2 < 100) cout << "Файл не открыт.Ошибка"; //ошибка
    else {
        for (int i = 0; i < k2; i++) {
            fin2.getline(inf2[StudNow1].surname, 20, ' '); //считывание ФИО
            fin2.getline(inf2[StudNow1].name, 20, ' ');
            fin2.getline(inf2[StudNow1].patronym, 20, '\n');
            StudNow1++;
        }

    }
    for (int i = 0; i < k; i++) {
        for (int i1 = 0; i1 < k2; i1++){
            if ((strcmp(inf1[i].surname, inf2[i1].surname) == 0) && (strcmp(inf1[i].name, inf2[i1].name) == 0) &&  strcmp(inf1[i].patronym, inf2[i1].patronym) == 0) {
                for (int j = 0; j < 5; j++) {
                    SubjGr[j] = inf1[i].grades_stud[j].mark + SubjGr[j]; 
                }
                lastID = i; //номер последнего студента группы
            }
        }
    }
    for (int j = 0; j < 5; j++) {
        if (min > SubjGr[j]) { min = SubjGr[j]; minId = j; } //находим самый минимальный по результатам предмет, сохраняем его айди
        if (max < SubjGr[j]) { max = SubjGr[j]; maxId = j; }//находим самый максимальный по результатам предмет. сохраняем его айди
    }
    cout <<"Минимальный средний балл группы равен " << min / k2 << " по " << inf1[lastID].grades_stud[minId].subject<<'\n';
    cout << "Максимальный средний балл группы  равен " << max / k2 << " по " << inf1[lastID].grades_stud[maxId].subject << '\n';
    fin1.close();
    fin2.close();
}