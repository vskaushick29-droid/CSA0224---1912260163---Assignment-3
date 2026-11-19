#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 30

typedef struct {
    long long strcmp_calls;
    long long char_copies;
} StringSortStats;

void strip_newline(char *s)
{
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';
    }
}

int strcasecmp_custom(const char *a, const char *b)
{
    while (*a && *b) {
        int ca = tolower((unsigned char)*a);
        int cb = tolower((unsigned char)*b);
        if (ca != cb) {
            return ca - cb;
        }
        a++;
        b++;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

StringSortStats sort_names_direct(char names[][MAX_LEN], int n)
{
    StringSortStats st;
    char temp[MAX_LEN];
    int i, j;
    st.strcmp_calls = 0;
    st.char_copies = 0;

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            st.strcmp_calls++;
            if (strcmp(names[j], names[j + 1]) > 0) {
                st.char_copies += (long long)(strlen(names[j]) + 1);
                strcpy(temp, names[j]);

                st.char_copies += (long long)(strlen(names[j + 1]) + 1);
                strcpy(names[j], names[j + 1]);

                st.char_copies += (long long)(strlen(temp) + 1);
                strcpy(names[j + 1], temp);
            }
        }
    }
    return st;
}

StringSortStats sort_names_indexed(const char names[][MAX_LEN], int idx[], int n)
{
    StringSortStats st;
    int i, j;
    st.strcmp_calls = 0;
    st.char_copies = 0;

    for (i = 0; i < n; i++) {
        idx[i] = i;
    }

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            st.strcmp_calls++;
            if (strcmp(names[idx[j]], names[idx[j + 1]]) > 0) {
                int tmp = idx[j];
                idx[j] = idx[j + 1];
                idx[j + 1] = tmp;
            }
        }
    }
    return st;
}

int binary_search_name(const char names[][MAX_LEN], int n, const char *target, int *comps)
{
    int low = 0, high = n - 1;
    *comps = 0;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp;
        (*comps)++;
        cmp = strcmp(names[mid], target);
        if (cmp == 0) {
            return mid;
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

void report_duplicates(const char names[][MAX_LEN], int n)
{
    int found = 0;
    int i = 0;
    printf("Duplicate Residents Found: ");
    while (i < n - 1) {
        if (strcmp(names[i], names[i + 1]) == 0) {
            int count = 2;
            while (i + count < n && strcmp(names[i], names[i + count]) == 0) {
                count++;
            }
            printf("\"%s\" (%d times)  ", names[i], count);
            found = 1;
            i += count;
        } else {
            i++;
        }
    }
    if (!found) {
        printf("None");
    }
    printf("\n");
}

void test_boundary_lengths(void)
{
    const char *name29 = "Alexandros_Constantinople_29C";
    const char *name30 = "Alexandros_Constantinople_30Ch";
    char buf29[MAX_LEN], buf30[MAX_LEN];

    snprintf(buf29, MAX_LEN, "%s", name29);
    snprintf(buf30, MAX_LEN, "%s", name30);

    printf("=== Student A: 29 vs 30 Character Name Boundary Test ===\n");
    printf("29-char input stored in char[30]: \"%s\" (strlen = %lu - intact)\n",
           buf29, (unsigned long)strlen(buf29));
    printf("30-char input stored in char[30]: \"%s\" (strlen = %lu - last char truncated for '\\0')\n\n",
           buf30, (unsigned long)strlen(buf30));
}

int main(void)
{
    char sample[8][MAX_LEN] = {
        "Karthik Subramanian",
        "Ananya Sharma",
        "Rahul Verma",
        "Ananya Sharma",
        "Deepak Nair",
        "Meera Krishnan",
        "Rahul Verma",
        "Bala Murugan"
    };
    int sample_n = 8;
    int i, s, comps = 0, pos1, pos2;
    int sizes[3] = {100, 500, 5000};

    test_boundary_lengths();

    sort_names_direct(sample, sample_n);
    printf("=== Sorted Hostel Directory (Sample) ===\n");
    for (i = 0; i < sample_n; i++) {
        printf("%2d. %s\n", i + 1, sample[i]);
    }
    report_duplicates(sample, sample_n);

    pos1 = binary_search_name(sample, sample_n, "Meera Krishnan", &comps);
    printf("Binary Search (Present: \"Meera Krishnan\") -> index %d, comparisons = %d\n", pos1, comps);
    pos2 = binary_search_name(sample, sample_n, "Zoya Khan", &comps);
    printf("Binary Search (Missing: \"Zoya Khan\")      -> index %d, comparisons = %d\n\n", pos2, comps);

    printf("=== Student A: Direct String Copy vs Index Sorting (n = 100, 500, 5,000) ===\n");
    printf("%-8s | %-15s | %-20s | %-18s\n",
           "Names(n)", "strcmp Calls", "Direct Char Copies", "Index Char Copies");
    printf("----------------------------------------------------------------------\n");

    for (s = 0; s < 3; s++) {
        int n = sizes[s];
        char (*arr_direct)[MAX_LEN] = (char (*)[MAX_LEN])malloc(n * sizeof(*arr_direct));
        char (*arr_index)[MAX_LEN]  = (char (*)[MAX_LEN])malloc(n * sizeof(*arr_index));
        int *idx = (int *)malloc(n * sizeof(int));
        StringSortStats st_dir, st_idx;

        if (!arr_direct || !arr_index || !idx) {
            fprintf(stderr, "Memory allocation failed\n");
            return 1;
        }

        srand(606);
        for (i = 0; i < n; i++) {
            sprintf(arr_direct[i], "Resident_%04d_%c%c%c",
                    rand() % 10000, 'A' + (rand() % 26), 'a' + (rand() % 26), 'a' + (rand() % 26));
            strcpy(arr_index[i], arr_direct[i]);
        }

        st_dir = sort_names_direct(arr_direct, n);
        st_idx = sort_names_indexed(arr_index, idx, n);

        printf("%-8d | %-15lld | %-20lld | %-18lld\n",
               n, st_dir.strcmp_calls, st_dir.char_copies, st_idx.char_copies);

        free(arr_direct);
        free(arr_index);
        free(idx);
    }

    return 0;
}
