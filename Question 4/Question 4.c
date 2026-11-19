#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DISTRICTS 38
#define MONTHS 12

void compute_district_totals(int rows, int cols, const double *rain, double totals[])
{
    int i, j;
    for (i = 0; i < rows; i++) {
        double sum = 0.0;
        for (j = 0; j < cols; j++) {
            sum += rain[i * cols + j];
        }
        totals[i] = sum;
    }
}

void compute_monthly_averages(int rows, int cols, const double *rain, double monthly_avg[])
{
    int i, j;
    for (j = 0; j < cols; j++) {
        double sum = 0.0;
        for (i = 0; i < rows; i++) {
            sum += rain[i * cols + j];
        }
        monthly_avg[j] = sum / rows;
    }
}

double find_wettest(int rows, int cols, const double *rain, int *best_d, int *best_m)
{
    int i, j;
    double max_val = rain[0];
    *best_d = 0;
    *best_m = 0;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            double val = rain[i * cols + j];
            if (val > max_val) {
                max_val = val;
                *best_d = i;
                *best_m = j;
            }
        }
    }
    return max_val;
}

void report_deficient_districts(int rows, const double totals[])
{
    int i;
    int found = 0;
    double state_total = 0.0;
    double state_avg_annual;
    double threshold;

    for (i = 0; i < rows; i++) {
        state_total += totals[i];
    }
    state_avg_annual = state_total / rows;
    threshold = 0.60 * state_avg_annual;

    printf("State Average Annual Rainfall: %.2f mm | 60%% Deficit Threshold: %.2f mm\n",
           state_avg_annual, threshold);
    printf("Deficient Districts: ");
    for (i = 0; i < rows; i++) {
        if (totals[i] < threshold) {
            printf("District %d (%.1f mm)  ", i + 1, totals[i]);
            found++;
        }
    }
    if (!found) {
        printf("None");
    }
    printf("\n");
}

void benchmark_cache_locality(void)
{
    const int N = 2000;
    static double big_grid[2000][2000];
    int i, j, rep;
    volatile double sink = 0.0;
    clock_t t0, t1;
    double sum_row = 0.0;
    double sum_col = 0.0;
    double row_sec, col_sec;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            big_grid[i][j] = (i + j) * 0.01;
        }
    }

    t0 = clock();
    for (rep = 0; rep < 50; rep++) {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                sum_row += big_grid[i][j];
            }
        }
    }
    t1 = clock();
    sink = sum_row;
    row_sec = (double)(t1 - t0) / CLOCKS_PER_SEC;

    t0 = clock();
    for (rep = 0; rep < 50; rep++) {
        for (j = 0; j < N; j++) {
            for (i = 0; i < N; i++) {
                sum_col += big_grid[i][j];
            }
        }
    }
    t1 = clock();
    sink += sum_col;
    col_sec = (double)(t1 - t0) / CLOCKS_PER_SEC;

    (void)sink;
    printf("\n=== Student B: Cache Locality Benchmark (2,000 x 2,000 x 50 passes) ===\n");
    printf("Row-wise traversal time   : %.4f seconds\n", row_sec);
    printf("Column-wise traversal time: %.4f seconds (%.2fx slower)\n",
           col_sec, row_sec > 0.0 ? col_sec / row_sec : 0.0);
}

int main(void)
{
    double small_rain[3][4] = {
        {100.0, 120.0, 140.0, 140.0},
        {200.0, 250.0, 300.0, 250.0},
        { 40.0,  50.0,  60.0,  50.0}
    };
    double small_totals[3];
    double small_m_avg[4];
    int wd, wm, i, j;
    double w_val, max_rain;
    double rain[DISTRICTS][MONTHS];
    double totals[DISTRICTS];
    double monthly_avg[MONTHS];

    printf("=== Hand-Verified 3 x 4 Rainfall Verification ===\n");
    compute_district_totals(3, 4, &small_rain[0][0], small_totals);
    compute_monthly_averages(3, 4, &small_rain[0][0], small_m_avg);
    w_val = find_wettest(3, 4, &small_rain[0][0], &wd, &wm);

    for (i = 0; i < 3; i++) {
        printf("District %d Annual Total: %.2f mm\n", i + 1, small_totals[i]);
    }
    printf("Monthly Averages: ");
    for (j = 0; j < 4; j++) {
        printf("M%d=%.2f ", j + 1, small_m_avg[j]);
    }
    printf("\nWettest District-Month: District %d, Month %d (%.2f mm)\n", wd + 1, wm + 1, w_val);
    report_deficient_districts(3, small_totals);

    printf("\n=== Full State Dataset: 38 Districts x 12 Months ===\n");
    srand(38);
    for (i = 0; i < DISTRICTS; i++) {
        for (j = 0; j < MONTHS; j++) {
            double base = (j >= 5 && j <= 10) ? 140.0 : 35.0;
            if (i == 6 || i == 28) {
                base *= 0.35;
            }
            rain[i][j] = base + (rand() % 40);
        }
    }

    compute_district_totals(DISTRICTS, MONTHS, &rain[0][0], totals);
    compute_monthly_averages(DISTRICTS, MONTHS, &rain[0][0], monthly_avg);
    max_rain = find_wettest(DISTRICTS, MONTHS, &rain[0][0], &wd, &wm);

    printf("Sample District Annual Totals (First 5): ");
    for (i = 0; i < 5; i++) {
        printf("D%d=%.1f ", i + 1, totals[i]);
    }
    printf("\nState Monthly Averages (Jan-Dec): ");
    for (j = 0; j < MONTHS; j++) {
        printf("M%d=%.1f ", j + 1, monthly_avg[j]);
    }
    printf("\nWettest District-Month: District %d, Month %d (%.2f mm)\n", wd + 1, wm + 1, max_rain);
    report_deficient_districts(DISTRICTS, totals);

    benchmark_cache_locality();

    return 0;
}
