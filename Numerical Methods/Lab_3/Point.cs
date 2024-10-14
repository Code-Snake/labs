using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Smoothing_spline
{
    // Класс для представления точки в трехмерном пространстве
    public class Point
    {
        // Свойства для доступа к координатам
        public double X { get; private set; } // Координата X
        public double Y { get; private set; } // Координата Y
        public double Z { get; private set; } // Координата Z

        // Конструктор по умолчанию (с указанными значениями координат)
        public Point(double x = 0, double y = 0, double z = 0)
        {
            X = x;
            Y = y;
            Z = z;
        }

        // Методы для получения координат
        public double x() => X; // Возвращает координату X
        public double y() => Y; // Возвращает координату Y
        public double z() => Z; // Возвращает координату Z
    }

}
