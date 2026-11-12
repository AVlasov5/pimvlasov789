#define _CRT_SECURE_NO_WARNINGS // Отключение предупреждений безопасности MSVC
#include <stdio.h>              // Библиотека для ввода-вывода
#include <stdlib.h>             // Библиотека для работы с памятью и генерации чисел (malloc, free, rand, srand)
#include <math.h>               // Библиотека для математических функций (sqrt)
#include <omp.h>                // Библиотека функций OpenMP для параллельных вычислений
#include <time.h>               // Библиотека для инициализации генератора случайных чисел временем

// Вспомогательная функция для безопасного чтения размера массива из input.txt
int read_size_from_input() {
    FILE* file_in = fopen("input.txt", "r");
    if (!file_in) {
        printf("Error: Could not open input.txt!\n");
        printf("Please create input.txt and write there the number of elements (e.g., 50).\n\n");
        return -1;
    }

    int n;
    if (fscanf(file_in, "%d", &n) != 1 || n <= 0) {
        printf("Error: Could not read a valid array size from input.txt!\n\n");
        fclose(file_in);
        return -1;
    }
    fclose(file_in);
    return n;
}

// --- ФУНКЦИЯ ДЛЯ ПЕРВОГО ПУНКТА: ГЕНЕРАЦИЯ, СОРТИРОВКА И ЗАПИСЬ ДО/ПОСЛЕ В OUTPUT ---
void run_bubble_sort() {
    printf("\n=== ARRAY SORTING (SIZE FROM INPUT.TXT) ===\n");

    int n = read_size_from_input();
    if (n == -1) return; // Если ошибка чтения файла — выходим

    printf("Successfully read array size from input.txt: %d elements.\n", n);

    // Выделяем память под массив целых чисел
    int* arr = (int*)malloc(n * sizeof(int));
    if (!arr) {
        printf("Error: Memory allocation failed!\n\n");
        return;
    }

    // Инициализируем генератор случайных чисел и заполняем массив числами от 0 до 999
    srand((unsigned int)time(NULL));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
    }
    printf("Array generated automatically.\n");

    // Открываем output.txt для записи результатов
    FILE* file_out = fopen("output.txt", "w");
    if (!file_out) {
        printf("Error: Could not write to output.txt\n\n");
        free(arr);
        return;
    }

    // 1. ЗАПИСЫВАЕМ СТАРЫЙ (ИСХОДНЫЙ) МАССИВ В OUTPUT.TXT
    fprintf(file_out, "=== INITIAL GENERATED ARRAY ===\n");
    for (int i = 0; i < n; i++) {
        fprintf(file_out, "%d ", arr[i]);
        if ((i + 1) % 10 == 0) fprintf(file_out, "\n"); // Перенос строки каждые 10 элементов
    }
    fprintf(file_out, "\n\n");

    // Выполняем алгоритм сортировки пузырьком
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    printf("Sorting completed.\n");

    // 2. ЗАПИСЫВАЕМ НОВЫЙ (ОТСОРТИРОВАННЫЙ) МАССИВ В OUTPUT.TXT
    fprintf(file_out, "=== NEW SORTED ARRAY ===\n");
    for (int i = 0; i < n; i++) {
        fprintf(file_out, "%d ", arr[i]);
        if ((i + 1) % 10 == 0) fprintf(file_out, "\n");
    }
    fprintf(file_out, "\n");

    fclose(file_out);
    printf("Initial and Sorted arrays successfully written to output.txt\n\n");
    free(arr); // Освобождаем память
}

// --- ФУНКЦИЯ ДЛЯ ВТОРОГО ПУНКТА: ГЕНЕРАЦИЯ, ВЫЧИСЛЕНИЯ С OPENMP И ЗАПИСЬ ВСЕГО В OUTPUT ---
void run_omp_calculations() {
    printf("\n=== OPENMP CALCULATIONS (SIZE FROM INPUT.TXT) ===\n");

    int n = read_size_from_input();
    if (n == -1) return;

    printf("Successfully read array size from input.txt: %d elements.\n", n);

    // Выделяем память под массив вещественных чисел
    double* arr = (double*)malloc(n * sizeof(double));
    if (!arr) {
        printf("Error: Memory allocation failed!\n\n");
        return;
    }

    // Инициализируем генератор случайных чисел и заполняем массив числами от 0.0 до 100.0
    srand((unsigned int)time(NULL));
    for (int i = 0; i < n; i++) {
        arr[i] = ((double)rand() / RAND_MAX) * 100.0;
    }
    printf("Array generated automatically.\n");

    // Конфигурация потоков OpenMP и замеры
    int thread_counts[] = { 1, 2, 4 };
    double mean = 0.0;
    double std_dev = 0.0;
    double times[3];

    for (int t = 0; t < 3; t++) {
        int num_threads = thread_counts[t];
        omp_set_num_threads(num_threads);

        double start_time = omp_get_wtime();

        // Параллельный расчет среднего
        double sum = 0.0;
#pragma omp parallel for reduction(+:sum)
        for (int i = 0; i < n; i++) {
            sum += arr[i];
        }
        mean = sum / n;

        // Параллельный расчет СКО
        double sq_sum = 0.0;
#pragma omp parallel for reduction(+:sq_sum)
        for (int i = 0; i < n; i++) {
            double diff = arr[i] - mean;
            sq_sum += diff * diff;
        }
        std_dev = sqrt(sq_sum / n);

        double end_time = omp_get_wtime();
        times[t] = end_time - start_time;

        printf("Threads: %d | Execution time: %.6f sec\n", num_threads, times[t]);
    }

    // Открываем output.txt для записи результатов
    FILE* file_out = fopen("output.txt", "w");
    if (file_out) {
        // 1. ЗАПИСЫВАЕМ СТАРЫЙ (ИСХОДНЫЙ) МАССИВ ЧИСЕЛ
        fprintf(file_out, "=== INITIAL GENERATED ARRAY ===\n");
        for (int i = 0; i < n; i++) {
            fprintf(file_out, "%.4f ", arr[i]);
            if ((i + 1) % 10 == 0) fprintf(file_out, "\n");
        }
        fprintf(file_out, "\n\n");

        // 2. НИЖЕ ДОПИСЫВАЕМ НОВЫЕ ВЫЧИСЛЕННЫЕ ДАННЫЕ И БЕНЧМАРК ВРЕМЕНИ
        fprintf(file_out, "=== CALCULATION RESULTS ===\n");
        fprintf(file_out, "Arithmetic Mean: %.4f\n", mean);
        fprintf(file_out, "Standard Deviation: %.4f\n\n", std_dev);

        fprintf(file_out, "=== BENCHMARK (TIME) ===\n");
        fprintf(file_out, "1 Thread:  %.6f sec\n", times[0]);
        fprintf(file_out, "2 Threads: %.6f sec\n", times[1]);
        fprintf(file_out, "4 Threads: %.6f sec\n", times[2]);

        fclose(file_out);
        printf("Initial array, calculations and benchmarks successfully written to output.txt\n\n");
    }
    else {
        printf("Error: Could not write to output.txt\n\n");
    }

    free(arr); // Освобождаем память
}

// --- ГЛАВНАЯ ФУНКЦИЯ ПРОГРАММЫ ---
int main() {
    int choice;

    while (1) {
        printf("Select an action:\n");
        printf("1 - Run array sorting (Task 1: Read size -> Generate -> Bubble Sort -> Save to output.txt)\n");
        printf("2 - Process Task 2 (Task 2: Read size -> Generate -> OMP -> Save to output.txt)\n");
        printf("3 - Exit program\n");
        printf("Your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Input error! Please enter a number.\n\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1:
            run_bubble_sort();
            break;
        case 2:
            run_omp_calculations();
            break;
        case 3:
            printf("Exiting program.\n");
            return 0;
        default:
            printf("Invalid menu option. Please try again.\n\n");
        }
    }

    return 0;
}
