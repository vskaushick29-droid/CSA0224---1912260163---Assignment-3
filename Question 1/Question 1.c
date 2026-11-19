#include <stdio.h>
#include <stdlib.h>

int steps;

int binary_search(const int a[], int n, int key)
{
    int low = 0;
    int high = n - 1;
    steps = 0;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        steps++;
        if (a[mid] == key) {
            return mid;
        } else if (a[mid] < key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

int linear_search(const int a[], int n, int key)
{
    int i;
    steps = 0;
    for (i = 0; i < n; i++) {
        steps++;
        if (a[i] == key) {
            return i;
        }
    }
    return -1;
}

void run_experimental_investigation(void)
{
    int sizes[3];
    int s;
    sizes[0] = 1000;
    sizes[1] = 10000;
    sizes[2] = 100000;

    printf("\n--- Student B: Experimental Investigation (n = 1,000, 10,000, 100,000) ---\n");
    printf("%-10s | %-18s | %-18s | %-18s | %-18s\n",
           "Size (n)", "Linear (Avg Succ)", "Linear (Unsucc)", "Binary (Avg Succ)", "Binary (Unsucc)");
    printf("-----------------------------------------------------------------------------------------------\n");

    for (s = 0; s < 3; s++) {
        int n = sizes[s];
        int *sorted_arr = (int *)malloc(n * sizeof(int));
        int *unsorted_arr = (int *)malloc(n * sizeof(int));
        int i, t;
        int trials = 500;
        long long lin_succ_total = 0, lin_unsucc_total = 0;
        long long bin_succ_total = 0, bin_unsucc_total = 0;

        if (!sorted_arr || !unsorted_arr) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        for (i = 0; i < n; i++) {
            sorted_arr[i] = 10000 + i * 2;
            unsorted_arr[i] = sorted_arr[i];
        }

        srand(42);
        for (i = n - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            int tmp = unsorted_arr[i];
            unsorted_arr[i] = unsorted_arr[j];
            unsorted_arr[j] = tmp;
        }

        for (t = 0; t < trials; t++) {
            int idx = (int)(((long long)t * n) / trials);
            int succ_key = sorted_arr[idx];
            int unsucc_key = succ_key + 1;

            linear_search(unsorted_arr, n, succ_key);
            lin_succ_total += steps;

            linear_search(unsorted_arr, n, unsucc_key);
            lin_unsucc_total += steps;

            binary_search(sorted_arr, n, succ_key);
            bin_succ_total += steps;

            binary_search(sorted_arr, n, unsucc_key);
            bin_unsucc_total += steps;
        }

        printf("%-10d | %-18.2f | %-18.2f | %-18.2f | %-18.2f\n",
               n,
               (double)lin_succ_total / trials,
               (double)lin_unsucc_total / trials,
               (double)bin_succ_total / trials,
               (double)bin_unsucc_total / trials);

        free(sorted_arr);
        free(unsorted_arr);
    }
}

int main(void)
{
    int accession[] = {1021, 1048, 1103, 1190, 1222, 1305, 1377, 1402, 1460, 1533};
    int n = sizeof accession / sizeof accession[0];
    int keys[] = {1021, 1305, 1533, 1400};
    int unsorted_trial[] = {1377, 1021, 1533, 1190, 1305, 1048, 1460, 1103, 1402, 1222};
    int lin_keys[] = {1377, 1305, 1222, 1400};
    int k;

    printf("=== Complete the Code: Binary Search Trial (10 Books) ===\n");
    for (k = 0; k < 4; k++) {
        int pos = binary_search(accession, n, keys[k]);
        printf("Book %d -> index %d, comparisons = %d\n", keys[k], pos, steps);
    }

    printf("\n=== Shared Group Work: Linear Search on Unsorted Trial Array ===\n");
    for (k = 0; k < 4; k++) {
        int pos = linear_search(unsorted_trial, n, lin_keys[k]);
        printf("Linear Search Book %d -> index %d, comparisons = %d\n", lin_keys[k], pos, steps);
    }

    run_experimental_investigation();
    return 0;
}
