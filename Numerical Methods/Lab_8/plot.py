# -*- coding: utf-8 -*-
import sys
import pandas as pd
import matplotlib.pyplot as plt

# Чтение имени файла из аргументов командной строки
datafile = sys.argv[1]

# Загрузка данных из CSV
data = pd.read_csv(datafile)

# График изменения численности жертв
plt.figure(figsize=(8, 6))
plt.plot(data['Time'], data['Prey'], label='Жертвы', color='blue')
plt.xlabel('Дни')
plt.ylabel('Популяция')
plt.title(f'Численность жертв со временем')
plt.legend()
plt.grid(True)
plt.show()

# График изменения численности хищников
plt.figure(figsize=(8, 6))
plt.plot(data['Time'], data['Predator'], label='Хищники', color='red')
plt.xlabel('Дни')
plt.ylabel('Популяция')
plt.title(f'Численность хищников со временем ')
plt.legend()
plt.grid(True)
plt.show()

# График зависимости численности хищников от численности жертв
plt.figure(figsize=(8, 6))
plt.scatter(data['Prey'], data['Predator'], label='Хищники vs Жертвы', color='green')
plt.xlabel('Численность жертв')
plt.ylabel('Численность хищников')
plt.title(f'Зависимость численности хищников от жертв ')
plt.legend()
plt.grid(True)
plt.show()
