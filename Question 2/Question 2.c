#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    long long comparisons;
    long long movements;
} SortStats;

SortStats bubble_sort(int arr[], int n)
{
    SortStats stats;
    int i, j;
    stats.comparisons = 0;
    stats.movements = 0;

    for (i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (j = 0; j < n - 1 - i; j++) {
            stats.comparisons++;
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
                stats.movements++;
                swapped = 1;
            }
        }
        if (!swapped) {
            break;
        }
    }
    return stats;
}

SortStats selection_sort(int arr[], int n)
{
    SortStats stats;
    int i, j;
    stats.comparisons = 0;
    stats.movements = 0;

    for (i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (j = i + 1; j < n; j++) {
            stats.comparisons++;
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            int tmp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = tmp;
            stats.movements++;
        }
    }
    return stats;
}

SortStats insertion_sort(int arr[], int n)
{
    SortStats stats;
    int i;
    stats.comparisons = 0;
    stats.movements = 0;

    for (i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0) {
            stats.comparisons++;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                stats.movements++;
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
    }
    return stats;
}

void generate_data(int base[], int n, int order_type)
{
    int i;
    for (i = 0; i < n; i++) {
        base[i] = 20260000 + i;
    }

    if (order_type == 1) {
        int swaps = (n <= 20) ? 1 : (n / 50);
        int s;
        srand(101);
        for (s = 0; s < swaps; s++) {
            int idx1 = rand() % (n - 1);
            int idx2 = idx1 + 1;
            int tmp = base[idx1];
            base[idx1] = base[idx2];
            base[idx2] = tmp;
        }
    } else if (order_type == 2) {
        for (i = 0; i < n; i++) {
            base[i] = 20260000 + (n - 1 - i);
        }
    } else if (order_type == 3) {
        srand(202);
        for (i = n - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            int tmp = base[i];
            base[i] = base[j];
            base[j] = tmp;
        }
    }
}

int main(void)
{
    int trace_sample[6] = {105, 102, 108, 101, 104, 103};
    int work6[6];
    int i, s, ord;
    SortStats s_bub, s_sel, s_ins;
    int sizes[3] = {100, 1000, 10000};
    const char *order_names[4] = {"Sorted", "Nearly Sorted", "Reverse-Sorted", "Random"};

    printf("=== 6-Element Register Hand-Trace Verification ===\n");
    printf("Initial: ");
    for (i = 0; i < 6; i++) printf("%d ", trace_sample[i]);
    printf("\n");

    memcpy(work6, trace_sample, sizeof(trace_sample));
    s_bub = bubble_sort(work6, 6);
    printf("Bubble Sort    -> Comp: %2lld, Swaps:  %2lld | Sorted: ", s_bub.comparisons, s_bub.movements);
    for (i = 0; i < 6; i++) printf("%d ", work6[i]);
    printf("\n");

    memcpy(work6, trace_sample, sizeof(trace_sample));
    s_sel = selection_sort(work6, 6);
    printf("Selection Sort -> Comp: %2lld, Swaps:  %2lld | Sorted: ", s_sel.comparisons, s_sel.movements);
    for (i = 0; i < 6; i++) printf("%d ", work6[i]);
    printf("\n");

    memcpy(work6, trace_sample, sizeof(trace_sample));
    s_ins = insertion_sort(work6, 6);
    printf("Insertion Sort -> Comp: %2lld, Shifts: %2lld | Sorted: ", s_ins.comparisons, s_ins.movements);
    for (i = 0; i < 6; i++) printf("%d ", work6[i]);
    printf("\n\n");

    printf("=== Student B: Comparative Measurement (n = 100, 1,000, 10,000) ===\n");
    printf("%-8s | %-15s | %-20s | %-20s | %-20s\n",
           "Size (n)", "Input Order", "Bubble (Cmp/Swp)", "Select (Cmp/Swp)", "Insert (Cmp/Shf)");
    printf("-------------------------------------------------------------------------------------------------\n");

    for (s = 0; s < 3; s++) {
        int n = sizes[s];
        int *base = (int *)malloc(n * sizeof(int));
        int *work = (int *)malloc(n * sizeof(int));
        if (!base || !work) {
            fprintf(stderr, "Memory allocation failed\n");
            return 1;
        }

        for (ord = 0; ord < 4; ord++) {
            SortStats b, sel, ins;
            char col_b[32], col_s[32], col_i[32];

            generate_data(base, n, ord);

            memcpy(work, base, n * sizeof(int));
            b = bubble_sort(work, n);

            memcpy(work, base, n * sizeof(int));
            sel = selection_sort(work, n);

            memcpy(work, base, n * sizeof(int));
            ins = insertion_sort(work, n);

            sprintf(col_b, "%lld / %lld", b.comparisons, b.movements);
            sprintf(col_s, "%lld / %lld", sel.comparisons, sel.movements);
            sprintf(col_i, "%lld / %lld", ins.comparisons, ins.movements);

            printf("%-8d | %-15s | %-20s | %-20s | %-20s\n",
                   n, order_names[ord], col_b, col_s, col_i);
        }
        printf("-------------------------------------------------------------------------------------------------\n");
        free(base);
        free(work);
    }

    return 0;
}
