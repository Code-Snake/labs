using System;
using System.Collections.Generic;
using System.Globalization;

namespace NumericalIntegration
{
    class Program
    {
        const double I_exact = 0.21306131942526682; // Точное значение интеграла

        static double FunctionToIntegrate(double x)
        {
            return -Math.Exp(-x / 2) + 1;
        }

        static void BuildGridsAndCalculate(Func<double, double> f, double a, double b)
        {
            var hValues = new List<double> { 0.1, 0.05, 0.025 };
            var integrator = new Integrator(a, b, 0.1);

            // --- Результаты по методу трапеций ---
            var trapezoidalResults = new List<double>();
            Console.WriteLine("\nТрапеция результаты:\n");
            
            foreach (double h in hValues)
            {
                integrator.SetStepSize(h);
                double trapezoidalResult = integrator.TrapezoidalRule(f);
                trapezoidalResults.Add(trapezoidalResult);
                double error = trapezoidalResult - I_exact;
                Console.WriteLine($"h = {h} : {trapezoidalResult} Ошибка: {error}");

                
            }
            Console.WriteLine("\nРезультаты интегрирования (трапеция) для заполнения таблицы ");
            Console.WriteLine("{0,-15}{1,-15}{2,-15}{3,-15}{4,-15}{5,-15}{6,-15}", "h", "k", "Ошибка", "Норма ошибки", "Рунге", "Ричардсон", "Ошибка по Ричардсону");
            
            
            foreach (double h in hValues)
            {
                integrator.SetStepSize(h);
                double trapezoidalResult = integrator.TrapezoidalRule(f);
                trapezoidalResults.Add(trapezoidalResult);
                if (trapezoidalResults.Count > 1 )
                {
                    double k = Math.Abs(Math.Log2(Math.Abs(1 + (trapezoidalResults[trapezoidalResults.Count - 2] - trapezoidalResults[trapezoidalResults.Count - 1]) / (I_exact - trapezoidalResults[trapezoidalResults.Count - 2]))));
                    double err = I_exact - trapezoidalResults[trapezoidalResults.Count - 1];
                    double errNorm = (I_exact - trapezoidalResults[trapezoidalResults.Count - 1]) / (I_exact - trapezoidalResults[trapezoidalResults.Count - 2]);
                    double runge = (trapezoidalResults[trapezoidalResults.Count - 2] - trapezoidalResults[trapezoidalResults.Count - 1]) / (Math.Pow(2, k) - 1);
                    double richardson = trapezoidalResults[trapezoidalResults.Count - 2] + runge;
                    double errRichardson = I_exact - richardson;
                    if (h < 0.1)
                    {
                        Console.WriteLine("{0,-15}{1,-15}{2,-15}{3,-15}{4,-15}{5,-15}{6,-15}", h + " ", k + " ", err + " ", errNorm + " ", runge + " ", richardson + " ", errRichardson);
                    }
                   
                }
            }

                // --- Результаты по методу Гаусса-3 ---
                var gauss3Results = new List<double>();
            Console.WriteLine("\nГаусс3 результаты:\n");
            foreach (double h in hValues)
            {
                integrator.SetStepSize(h);
                double gauss3Result = integrator.GaussianQuadrature3(f);
                gauss3Results.Add(gauss3Result);
                double error = gauss3Result - I_exact;
                Console.WriteLine($"h = {h} : {gauss3Result} Error: {error}");
            }
            Console.WriteLine("\nРезультаты интегрирования (Гаусс3) для заполнения таблицы ");
            Console.WriteLine("{0,-15}{1,-15}{2,-15}{3,-15}{4,-15}{5,-15}{6,-15}", "h", "k", "Ошибка", "Норма ошибки", "Рунге", "Ричардсон", "Ошибка по Ричардсону");
            foreach (double h in hValues)
            {
                integrator.SetStepSize(h);
                double gauss3Result = integrator.GaussianQuadrature3(f);
                gauss3Results.Add(gauss3Result);
                if (gauss3Results.Count > 1)
                {
                    double k = Math.Abs(Math.Log2(Math.Abs(1 + (gauss3Results[gauss3Results.Count - 2] - gauss3Results[gauss3Results.Count - 1]) / (I_exact - gauss3Results[gauss3Results.Count - 2]))));
                    double err = I_exact - gauss3Results[gauss3Results.Count - 1];
                    double errNorm = (I_exact - gauss3Results[gauss3Results.Count - 1]) / (I_exact - gauss3Results[gauss3Results.Count - 2]);
                    double runge = (gauss3Results[gauss3Results.Count - 2] - gauss3Results[gauss3Results.Count - 1]) / (Math.Pow(2, k) - 1);
                    double richardson = gauss3Results[gauss3Results.Count - 2] + runge;
                    double errRichardson = I_exact - richardson;

                    if (h < 0.1)
                    {
                        Console.WriteLine("{0,-15}{1,-15}{2,-15}{3,-15}{4,-15}{5,-15}{6,-15}", h + " ", k + " ", err + " ", errNorm + " ", runge + " ", richardson + " ", errRichardson);
                    }
                }
            }
        }

        static void Main(string[] args)
        {
            double a = 0.0, b = 1.0;
            BuildGridsAndCalculate(FunctionToIntegrate, a, b);
        }
    }
}
