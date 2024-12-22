import matplotlib.pyplot as plt
import pandas as pd
import sys


def plot_wavelet_data(basis, stage):
    # Считывание данных из CSV-файла
    filename = 'data.csv'  # путь к файлу с данными
    data = pd.read_csv(filename)

    # Подготовка данных для графиков
    index = data['Index']
    Z = data['Original Data']  # Исходный сигнал
    Z_rec = data['Filtered Data']  # Восстановленный сигнал
    psi = data['Psi'].fillna(0)  # Коэффициенты Psi (или Кси), заполняем пустые значения нулями
    phi = data['Fi'].fillna(0)  # Коэффициенты Fi (или Фи), заполняем пустые значения нулями

    # Создание фигуры и 4-х графиков на одном листе
    fig, axs = plt.subplots(2, 2, figsize=(12, 8))

    # График 1: Исходный сигнал
    axs[0, 0].plot(index, Z, label="Оригинальный сигнал")
    axs[0, 0].set_title(f"Оригинальный сигнал (Этап {stage})")
    axs[0, 0].set_xlabel("Индекс")
    axs[0, 0].set_ylabel("Амплитуда")
    axs[0, 0].grid(True)

    # График 2: Восстановленный сигнал
    axs[0, 1].plot(index, Z_rec, label="Восстановленный сигнал", color='orange')
    axs[0, 1].set_title(f"Восстановленный сигнал (Этап {stage})")
    axs[0, 1].set_xlabel("Индекс")
    axs[0, 1].set_ylabel("Амплитуда")
    axs[0, 1].grid(True)

    # График 3: Коэффициенты Psi
    axs[1, 0].plot(index, psi, label="Psi", color='green')
    axs[1, 0].set_title(f"P")
    axs[1, 0].set_xlabel("Индекс")
    axs[1, 0].set_ylabel("Амплитуда")
    axs[1, 0].grid(True)

    # График 4: Коэффициенты Fi (или Фи)
    axs[1, 1].plot(index, phi, label="Fi (Phi)", color='red')
    axs[1, 1].set_title(f"P0 ")
    axs[1, 1].set_xlabel("Индекс")
    axs[1, 1].set_ylabel("Амплитуда")
    axs[1, 1].grid(True)

    # Автоматическое подстраивание размещения графиков
    plt.tight_layout()

    # Отображение графиков
    plt.show()


if __name__ == "__main__":
    # Получаем аргументы из командной строки
    if len(sys.argv) < 3:
        print("Usage: python3 plot.py <basis> <stage>")
        sys.exit(1)

    basis = sys.argv[1]  # Тип вейвлет-базиса
    stage = int(sys.argv[2])  # Этап (stage)

    # Вызов функции для построения графиков
    plot_wavelet_data(basis, stage)
