#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long merge_comparisons = 0;
long long qsort_comparisons = 0;

int merge(const int a[], int na, const int b[], int nb, int out[])
{
    int i = 0, j = 0, k = 0;
    merge_comparisons = 0;

    while (i < na && j < nb) {
        merge_comparisons++;
        if (a[i] >= b[j]) {
            out[k++] = a[i++];
        } else {
            out[k++] = b[j++];
        }
    }
    while (i < na) {
        out[k++] = a[i++];
    }
    while (j < nb) {
        out[k++] = b[j++];
    }

    return k;
}

int is_sorted_desc(const int arr[], int n)
{
    int i;
    for (i = 0; i < n - 1; i++) {
        if (arr[i] < arr[i + 1]) return 0;
    }
    return 1;
}

int cmp_desc(const void *p1, const void *p2)
{
    int v1 = *(const int *)p1;
    int v2 = *(const int *)p2;
    qsort_comparisons++;
    return (v2 > v1) - (v2 < v1);
}

void print_list(const char *label, const int arr[], int n)
{
    int i;
    printf("%s (%d): ", label, n);
    for (i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("[Ordered: %s]\n", is_sorted_desc(arr, n) ? "YES" : "NO");
}

int main(void)
{
    int campusA[4] = {98, 91, 85, 72};
    int campusB[3] = {95, 91, 80};
    int merged[7];
    int emptyList[1] = {0};
    int out_empty[3];
    int eqA[3] = {90, 90, 90};
    int eqB[2] = {90, 90};
    int out_eq[5];
    int test_sizes[2] = {1000, 10000};
    int i, n, n_emp, n_eq, s;

    printf("=== Complete the Code: Inter-Campus Merit List Merge ===\n");
    n = merge(campusA, 4, campusB, 3, merged);
    for (i = 0; i < n; i++) printf("%d ", merged[i]);
    printf("\n(Comparisons made: %lld, Ordered: %s)\n\n",
           merge_comparisons, is_sorted_desc(merged, n) ? "YES" : "NO");

    printf("=== Edge Case 1: One Empty List ===\n");
    n_emp = merge(emptyList, 0, campusB, 3, out_empty);
    print_list("Merged (Empty A + Campus B)", out_empty, n_emp);
    printf("Comparisons made: %lld\n\n", merge_comparisons);

    printf("=== Edge Case 2: All Scores Equal ===\n");
    n_eq = merge(eqA, 3, eqB, 2, out_eq);
    print_list("Merged (All 90s)", out_eq, n_eq);
    printf("Comparisons made: %lld\n\n", merge_comparisons);

    printf("=== Student A: Comparison Analysis (Merge vs Join + qsort) ===\n");
    printf("%-10s | %-18s | %-18s\n", "m = n", "Merge Comparisons", "qsort Comparisons");
    printf("-----------------------------------------------------\n");

    for (s = 0; s < 2; s++) {
        int m = test_sizes[s];
        int *listA = (int *)malloc(m * sizeof(int));
        int *listB = (int *)malloc(m * sizeof(int));
        int *outMerge = (int *)malloc(2 * m * sizeof(int));
        int *outQsort = (int *)malloc(2 * m * sizeof(int));

        if (!listA || !listB || !outMerge || !outQsort) {
            fprintf(stderr, "Memory allocation failed\n");
            return 1;
        }

        for (i = 0; i < m; i++) {
            listA[i] = (m - i) * 2 + 1;
            listB[i] = (m - i) * 2;
        }

        merge(listA, m, listB, m, outMerge);

        memcpy(outQsort, listA, m * sizeof(int));
        memcpy(outQsort + m, listB, m * sizeof(int));
        qsort_comparisons = 0;
        qsort(outQsort, 2 * m, sizeof(int), cmp_desc);

        printf("%-10d | %-18lld | %-18lld\n", m, merge_comparisons, qsort_comparisons);

        free(listA);
        free(listB);
        free(outMerge);
        free(outQsort);
    }

    return 0;
}
