using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Collections.Generic;
using System.Numerics;

namespace Smoothing_spline
{
    class CONST
    {
        public static double EPS = 1e-15;
    }

    public class NormalDistributionGenerator
    {
        private readonly Random _random;
        private readonly double _mean; // Математическое ожидание (M)
        private readonly double _standardDeviation; // Стандартное отклонение (σ)

        public NormalDistributionGenerator(double mean, double variance)
        {
            _random = new Random();
            _mean = mean;
            _standardDeviation = Math.Sqrt(variance);
        }

        public double NextDouble()
        {
            // Используем метод Бокса-Мюллера
            double u1 = 1.0 - _random.NextDouble(); // Генерируем случайное число [0, 1)
            double u2 = 1.0 - _random.NextDouble(); // Генерируем случайное число [0, 1)

            double z0 = Math.Sqrt(-2.0 * Math.Log(u1)) * Math.Cos(2.0 * Math.PI * u2); // Стандартное нормальное распределение
            return z0 * _standardDeviation + _mean; // Преобразуем в нормальное распределение с параметрами M и D
        }
    }
    class Program
    {
        const int N_observations = 1718;
        // Метод для сохранения данных в файл
        static void SaveToFile(List<double> valuesY,
                                List<double> smoothed0,
                                List<double> smoothed3,
                                List<double> smoothed6,
                                List<double> smoothed99,
                                string filename)
        {
            using (StreamWriter file = new StreamWriter(filename))
            {
                for (int i = 0; i < N_observations; i++)
                {
                    file.WriteLine($"{valuesY[i]} {smoothed0[i]} {smoothed3[i]} {smoothed6[i]} {smoothed99[i]}");
                }
            }
        }


        static void Main(string[] args)
        {
            // Инициализация вспомогательных классов интерполяции и сглаживания
 
            var smoothing_spline_0 = new SmoothingSpline(0);
            var smoothing_spline_3 = new SmoothingSpline(0.3);
            var smoothing_spline_6 = new SmoothingSpline(0.6);
            var smoothing_spline_99 = new SmoothingSpline(0.99);

            // Среднее значение и стандартное отклонение для нормального распределения
            double M = 1.02, D = 3.61;
            List<double> valuesY = new List<double>();
            List<double> smoothed_0 = new List<double>();
            List<double> smoothed_3 = new List<double>();
            List<double> smoothed_6 = new List<double>();
            List<double> smoothed_99 = new List<double>();
            List<double> res_smoothed_0 = new List<double>();
            List<double> res_smoothed_3 = new List<double>();
            List<double> res_smoothed_6 = new List<double>();
            List<double> res_smoothed_99 = new List<double>();
            List<Point> valuesX = new List<Point>();

            // Генерация случайных данных

            NormalDistributionGenerator generator = new NormalDistributionGenerator(M, D);
            for (int i = 0; i < N_observations; i++)
            {
                double x = i; // Равномерное распределение по оси X
                double y = generator.NextDouble(); // Нормальное распределение по оси Y
                valuesY.Add(y);
                valuesX.Add(new Point(x, 0, 0));
            }

            // Обновление сплайнов

            smoothing_spline_0.UpdateSpline(valuesX, valuesY);
            smoothing_spline_3.UpdateSpline(valuesX, valuesY);
            smoothing_spline_6.UpdateSpline(valuesX, valuesY);
            smoothing_spline_99.UpdateSpline(valuesX, valuesY);

            // Получение значений интерполяции и сглаживания
            for (int i = 0; i < N_observations; i++)
            {

                res_smoothed_0 = smoothing_spline_0.SplineInPoint(valuesX[i]);
                res_smoothed_3 = smoothing_spline_3.SplineInPoint(valuesX[i]);
                res_smoothed_6 = smoothing_spline_6.SplineInPoint(valuesX[i]);
                res_smoothed_99 = smoothing_spline_99.SplineInPoint(valuesX[i]);

                smoothed_0.Add(res_smoothed_0[0]);
                smoothed_3.Add(res_smoothed_3[0]);
                smoothed_6.Add(res_smoothed_6[0]);
                smoothed_99.Add(res_smoothed_99[0]);

                Console.WriteLine($"Случайная величина: {valuesY[i]} Сглаженное p = 0: {res_smoothed_0[0]} Сглаженное p = 0.3: {res_smoothed_3[0]} Сглаженное p = 0.6: {res_smoothed_6[0]} Сглаженное p = 0.99: {res_smoothed_99[0]}");
            }

            // Сохранение данных в файл
            string filename = "spline_data.txt";
            SaveToFile(valuesY, smoothed_0, smoothed_3, smoothed_6, smoothed_99, filename);

        }
    }
}

