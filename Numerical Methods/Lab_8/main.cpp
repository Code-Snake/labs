#include <iostream>
#include <fstream>
#include <iomanip>
#include <functional>
#include <vector>
#include "Class_PP.h"

using namespace std;

int main() {
    // Инициализация задачи Коши
    Class_PP<double> class_PP;

    // Число переменных системы (жертвы и хищники)
    const int numVariables = 2;

    // Открытие файлов для сохранения результатClass_PPов
    ofstream erkDataFile("erk_results.csv");
    ofstream adamsDataFile("adams_results.csv");

    // Заголовки для CSV файлов
    erkDataFile << "Time,Prey,Predator\n";
    adamsDataFile << "Time,Prey,Predator\n";

    // Параметры модели популяций
    const double birthRatePrey = 0.3;        // Рождаемость жертв
    const double predationRate = 0.000207;  // Успешные атаки хищников
    const double naturalDeathRate = 0.1449; // Естественная смертность хищников
    const double reproductionRate = 0.0001; // Размножение хищников при наличии пищи

    // Функция, задающая правую часть системы дифференциальных уравнений
    auto modelEquations = [&](const vector<double>& state, const Point& timePoint) {
        vector<double> derivatives(numVariables);
        derivatives[0] = (birthRatePrey - predationRate * state[1]) * state[0]; // Жертвы
        derivatives[1] = (-naturalDeathRate + reproductionRate * state[0]) * state[1]; // Хищники
        return derivatives;
    };

    // Временные параметры симуляции
    const double startTime = 0.0, endTime = 365.0; // Временной интервал
    const int numStepsERK = 365;                  // Количество шагов для ERK
    const double stepSizeERK = (endTime - startTime) / (numStepsERK); // Размер шага для ERK

    const double adamsStepSize = 0.5; // Шаг в полдня для метода Адамса
    const int numStepsAdams = static_cast<int>((endTime - startTime) / adamsStepSize); // Количество шагов для Адамса

    // Начальные условия для жертв и хищников
    vector<double> initialConditionsERK = { 3309.0, 778.0 };  // Метод Рунге-Кутты
    vector<double> initialConditionsAdams = { 3309.0, 778.0 }; // Метод Адамса

    // Генерация стартовых значений для метода Адамса
    vector<vector<double>> adamsInitialConditions = class_PP.rungeKuttaImplementation(
        startTime, adamsStepSize, 3, initialConditionsAdams, modelEquations
    );
    vector<vector<double>> adamsStateHistory = { initialConditionsAdams };
    adamsStateHistory.insert(adamsStateHistory.end(), adamsInitialConditions.begin(), adamsInitialConditions.end());

    // Основной цикл решения
    for (int stepERK = 1; stepERK <= numStepsERK; ++stepERK) {
        // Определение текущего временного интервала для ERK
        Point startPointERK((stepERK - 1) * stepSizeERK, 0, 0);
        Point endPointERK(stepERK * stepSizeERK, 0, 0);

        // Метод Рунге-Кутты 4-го порядка
        auto erkSolution = class_PP.rungeKuttaScheme(
            Class_PP<double>::SchemeType::ERK4,
            startPointERK.x(), endPointERK.x(), initialConditionsERK, modelEquations
        );
        erkDataFile << endPointERK.x() << "," << erkSolution[0] << "," << erkSolution[1] << "\n";
        initialConditionsERK = erkSolution;
        // Вывод текущих результатов для метода Рунге-Кутты
        //cout << fixed << setprecision(6);
        //cout << "\n\nTime (ERK): " << endPointAdams.x()
        //    << "\nPrey (ERK): " << adamsSolution[0]
        //    << "\nPredator (ERK): " << adamsSolution[1];
    }

    for (int stepAdams = 1; stepAdams <= numStepsAdams; ++stepAdams) {
        // Определение текущего временного интервала для Адамса
        Point startPointAdams((stepAdams - 1) * adamsStepSize, 0, 0);
        Point endPointAdams(stepAdams * adamsStepSize, 0, 0);

        // Метод Адамса
        auto adamsSolution = class_PP.adamsPredictorCorrector(
            Class_PP<double>::SchemeType::Adams,
            startPointAdams.x(),
            endPointAdams.x(),
            adamsStateHistory,
            modelEquations
        );
        adamsDataFile << endPointAdams.x() << "," << adamsSolution[0] << "," << adamsSolution[1] << "\n";
        adamsStateHistory.erase(adamsStateHistory.begin());
        adamsStateHistory.push_back(adamsSolution);

        // Вывод текущих результатов для метода Адамса
        //cout << fixed << setprecision(6);
        //cout << "\n\nTime (Adams): " << endPointAdams.x()
        //    << "\nPrey (Adams): " << adamsSolution[0]
        //    << "\nPredator (Adams): " << adamsSolution[1];
    }


    // Закрытие файлов
    erkDataFile.close();
    adamsDataFile.close();
    system("python plot.py erk_results.csv");
    system("python plot.py adams_results.csv");
    return 0;
}
