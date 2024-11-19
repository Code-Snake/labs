using System;
using System.Collections.Generic;

namespace NumericalIntegration
{
    internal class Integrator
    {
        private double lowerBound, upperBound, stepSize;
        private List<double> partition;

        public Integrator(double lower, double upper, double step)
        {
            this.lowerBound = lower;
            this.upperBound = upper;
            this.stepSize = step;
            CreatePartition();
        }

        public double GetStepSize()
        {
            return stepSize;
        }

        public void SetStepSize(double newStep)
        {
            stepSize = newStep;
            CreatePartition();
        }

        private void CreatePartition()
        {
            double current = lowerBound + stepSize;
            partition = new List<double> { lowerBound };
            while (Math.Abs(current - upperBound) >= 1e-6)
            {
                partition.Add(current);
                current += stepSize;
            }
            if (partition[partition.Count - 1] != upperBound)
            {
                partition.Add(upperBound);
            }
        }

        // Метод интегрирования методом трапеций
        public double TrapezoidalRule(Func<double, double> function)
        {
            double totalArea = stepSize * function(lowerBound) + stepSize * function(upperBound);
            for (int i = 1; i < partition.Count - 1; i++)
            {
                double x = partition[i];
                totalArea += (stepSize + stepSize) * function(x);
            }
            return totalArea * 0.5;
        }

        // Метод интегрирования методом Гаусса с 3 точками
        public double GaussianQuadrature3(Func<double, double> function)
        {
            double[] points = { -Math.Sqrt(3.0 / 5.0), 0.0, Math.Sqrt(3.0 / 5.0) };
            double[] weights = { 5.0 / 9.0, 8.0 / 9.0, 5.0 / 9.0 };

            double totalArea = 0.0;
            for (int k = 1; k < partition.Count; k++)
            {
                double left = partition[k - 1];
                double right = partition[k];
                double halfStep = (right - left) / 2.0;

                for (int i = 0; i < 3; i++)
                {
                    double x = (left + right) / 2.0 + halfStep * points[i];
                    totalArea += weights[i] * function(x);
                }
            }

            return totalArea * (stepSize / 2.0);
        }
    }
}
