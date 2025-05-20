#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Функция для создания матрицы в куче
int** createMatrix(int rows, int cols)
{
    int** matrix = new int* [rows];
    for (int i = 0; i < rows; i++)
    {
        matrix[i] = new int[cols];
    }
    return matrix;
}

// Функция для заполнения матрицы случайными числами
void fillMatrix(int** matrix, int rows, int cols)
{
    srand(time(0));
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix[i][j] = rand() % 100; // Генерация чисел от 0 до 99
        }
    }
}

// Функция для вывода матрицы
void printMatrix(int** matrix, int rows, int cols)
{
    setlocale(LC_ALL, "Russian");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
}

// Функция для замены четных и нечетных столбцов
void swapColumns(int** matrix, int rows, int cols)
{
    for (int j = 0; j < cols - 1; j += 2)
    {
        for (int i = 0; i < rows; i++)
        {
            swap(matrix[i][j], matrix[i][j + 1]);
        }
    }
}

// Функция для освобождения памяти
void deleteMatrix(int** matrix, int rows)
{
    for (int i = 0; i < rows; i++)
    {
        delete[] matrix[i];
    }
    delete[] matrix;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    int rows, cols;
    cout << "Введите количество строк: ";
    cin >> rows;
    cout << "Введите количество столбцов: ";
    cin >> cols;

    // Создание и обработка матрицы
    int** matrix = createMatrix(rows, cols);
    fillMatrix(matrix, rows, cols);

    cout << "Исходная матрица: " << endl;
    printMatrix(matrix, rows, cols);

    swapColumns(matrix, rows, cols);

    cout << "Матрица после замены столбцов: " << endl;
    printMatrix(matrix, rows, cols);

    // Очистка памяти
    deleteMatrix(matrix, rows);

    return 0;
}
