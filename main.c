#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 10000  

typedef struct {
    int* array;
    int size;
    char* name;
    int sort_id;  
} SortData;

double get_time_in_ms(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0;
}

void print_sort_start(int sort_id, const char* name) {
    printf("Iniciando ordenação #######%d %s...\n", sort_id, name);
}

void* bubble_sort(void* arg) {
    SortData* data = (SortData*)arg;
    print_sort_start(data->sort_id, data->name); 

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int* arr = data->array;
    int n = data->size;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("%s demorou: %.2f ms\n", data->name, get_time_in_ms(start, end));
    pthread_exit(NULL);
}

void* selection_sort(void* arg) {
    SortData* data = (SortData*)arg;
    print_sort_start(data->sort_id, data->name); 

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int* arr = data->array;
    int n = data->size;
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        int temp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = temp;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("%s demorou: %.2f ms\n", data->name, get_time_in_ms(start, end));
    pthread_exit(NULL);
}

void* insertion_sort(void* arg) {
    SortData* data = (SortData*)arg;
    print_sort_start(data->sort_id, data->name); 

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int* arr = data->array;
    int n = data->size;
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("%s demorou: %.2f ms\n", data->name, get_time_in_ms(start, end));
    pthread_exit(NULL);
}

void quick_sort_helper(int* arr, int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = (low - 1);
        for (int j = low; j <= high - 1; j++) {
            if (arr[j] < pivot) {
                i++;
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        int temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;
        int pi = i + 1;

        quick_sort_helper(arr, low, pi - 1);
        quick_sort_helper(arr, pi + 1, high);
    }
}

void* quick_sort(void* arg) {
    SortData* data = (SortData*)arg;
    print_sort_start(data->sort_id, data->name); 

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    quick_sort_helper(data->array, 0, data->size - 1);

    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("%s demorou: %.2f ms\n", data->name, get_time_in_ms(start, end));
    pthread_exit(NULL);
}

void init_array(int* array, int size) {
    for (int i = 0; i < size; i++) {
        array[i] = rand() % 10000;  
    }
}

int main() {
    pthread_t threads[4];
    SortData data[4];
    int array[SIZE], v1[SIZE], v2[SIZE], v3[SIZE], v4[SIZE];

    srand(time(NULL));
    init_array(array, SIZE);  

    for (int i = 0; i < SIZE; i++) {
        v1[i] = array[i];
        v2[i] = array[i];
        v3[i] = array[i];
        v4[i] = array[i];
    }

    data[0] = (SortData){v1, SIZE, "Bubble Sort", 1};
    data[1] = (SortData){v2, SIZE, "Selection Sort", 2};
    data[2] = (SortData){v3, SIZE, "Insertion Sort", 3};
    data[3] = (SortData){v4, SIZE, "Quick Sort", 4};

    pthread_create(&threads[0], NULL, bubble_sort, (void*)&data[0]);
    pthread_create(&threads[1], NULL, selection_sort, (void*)&data[1]);
    pthread_create(&threads[2], NULL, insertion_sort, (void*)&data[2]);
    pthread_create(&threads[3], NULL, quick_sort, (void*)&data[3]);

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Main finalizado!\n");
    return 0;
}
