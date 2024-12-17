import pandas as pd
import matplotlib.pyplot as plt

# Чтение данных из CSV-файла
filename = "data.csv"
data = pd.read_csv(filename)

# Построение графика
plt.figure(figsize=(10, 6))

# График исходных данных
plt.plot(data["Idx"], data["Data"], label="Исходные данные", linestyle='-', color='b')

# График отфильтрованных данных
plt.plot(data["Idx"], data["F_Data"], label="Фильтрованные данные", linestyle='-', color='r')

# Настройка осей и заголовков
plt.title("График исходных и фильтрованных данных")
plt.xlabel("Индекс")
plt.ylabel("Значение")
plt.legend()
plt.grid(True)

# Показ графика
plt.show()
