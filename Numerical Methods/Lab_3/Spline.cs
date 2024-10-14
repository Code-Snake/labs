using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Smoothing_spline
{
    public class SmoothingSpline
    {
        private double SMOOTH;
        private List<Point> points = new List<Point>();
        private List<double> alpha;

        public SmoothingSpline(double smooth) //конструктор с параметром сглаживания
        {
            this.SMOOTH = smooth;
        }

        private void FoundKsi(int Segment_Number,  double x, out double ksi)
        {
            ksi = 2.0 * (x - points[Segment_Number].X) / (points[Segment_Number + 1].X - points[Segment_Number].X) - 1.0;
        }
        
        //Находим базисные функции на [-1,1]
        private double BasisFunction(int N, double ksi){
            switch (N)
            {
                case 1:
                    {
                        return 0.5 *(1- ksi);
                        break;
                    }
                case 2:
                    {
                        return 0.5 * (1 + ksi);
                        break;
                    }
                default:
                    {
                        throw new Exception("Ошибка в номере базисной функции.");
                        break; 
                    }
            }
         }

        //Находим производные базисных функций на [-1,1]
        private double DerivativeBasis(int N, double ksi)
        {
            switch (N)
            {
                case 1:
                    {
                        return -0.5;
                        break;
                    }
                case 2:
                    {
                        return 0.5;
                        break;
                    }
                default:
                    {
                        throw new Exception("Ошибка в номере производной базисной функции.");
                        break;
                    }
            }
        }
    
        public void UpdateSpline(List<Point> points, List<double> fValue)
        {
            this.points.Clear();
            this.points.AddRange(points); //обновили точки 
            int numSegments = points.Count - 1; //кол-во сегментов
            alpha = new List<double>(new double[numSegments + 1]);

            
            List<double> a = new List<double>(new double[numSegments + 1]);
            List<double> b = new List<double>(new double[numSegments + 1]);
            List<double> c = new List<double>(new double[numSegments + 1]);


            //ассемблирование
            Action<int, Point, double, double> assembling = null;
            assembling = (i, p, fVal, w) =>
            {
                double x = p.X, ksi;
                FoundKsi(i, x, out ksi);
                double f1 = BasisFunction(1, ksi);
                double f2 = BasisFunction(2, ksi);

                //левая часть СЛАУ 
                b[i] += (1.0 - SMOOTH) * w * f1 * f1;
                b[i + 1] += (1.0 - SMOOTH) * w * f2 * f2;
                a[i + 1] += (1.0 - SMOOTH) * w * f1 * f2;
                c[i] += (1.0 - SMOOTH) * w * f2 * f1;

                //правая часть СЛАУ
                alpha[i] += (1.0 - SMOOTH) * w * f1 * fVal;
                alpha[i + 1] += (1.0 - SMOOTH) * w * f2 * fVal;
            };

            for (int i = 0; i < numSegments; i++)
            {
                //w - вес
                double w = 0.1;
                //вызов процедуры ассемблирования
                assembling(i, this.points[i], fValue[i], w);
                assembling(i, this.points[i + 1], fValue[i + 1], w);

                //вклад по первой производной
                double h = points[i + 1].X - points[i].X;
                b[i] += 1.0 / h * SMOOTH;
                b[i + 1] += 1.0 / h * SMOOTH;
                a[i + 1] -= 1.0 / h * SMOOTH;
                c[i] -= 1.0 / h * SMOOTH;
            }

            //ПРОГОНКА - forward
            for (int j = 1; j < numSegments + 1; j++)
            {
                b[j] -= a[j] / b[j - 1] * c[j - 1];
                alpha[j] -= a[j] / b[j - 1] * alpha[j - 1]; //правая часть
            }

            //ПРОГОНКА - backward
            alpha[numSegments] /= b[numSegments];
            for (int j = numSegments - 1; j >= 0; j--)
            {
                alpha[j] = (alpha[j] - alpha[j + 1] * c[j]) / b[j];//правая часть
            }
        }

        public List<double> SplineInPoint(Point p)
        {
            int numSegments = points.Count - 1;
            double x = p.X;

            // Инициализация списка res с тремя значениями
            List<double> res = new List<double>(new double[3]);

            for (int i = 0; i < numSegments; i++)
            {
                if ((x > points[i].X && x < points[i + 1].X) ||
                    Math.Abs(x - points[i].X) < CONST.EPS ||
                    Math.Abs(x - points[i + 1].X) < CONST.EPS)
                {
                    //шаг h - длина сегмента
                    double h = points[i + 1].X - points[i].X;
                    double ksi;
                    //поиск кси для перехода на [-1, 1]
                    FoundKsi(i, x, out ksi);

                    //Сплайн и производные по базисным функциям
                    res[0] = alpha[i] * BasisFunction(1, ksi) +
                             alpha[i + 1] * BasisFunction(2, ksi);
                    res[1] = (alpha[i] * DerivativeBasis(1, ksi) +
                               alpha[i + 1] * DerivativeBasis(2, ksi)) * 2.0 / h;
                    res[2] = 0.0; 

                    return res; 
                }
            }
            throw new Exception("Точка не найдена в сегменте");
        }


    }
}
