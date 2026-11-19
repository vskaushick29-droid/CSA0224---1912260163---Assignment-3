#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CANDIDATES 12

typedef struct {
    int candidate_id;
    int votes;
} CandidateResult;

void run_election(const char *label, int num_candidates, const int ballots[], int num_votes, int verbose)
{
    int count[MAX_CANDIDATES + 1];
    int valid_votes = 0;
    int invalid_votes = 0;
    CandidateResult results[MAX_CANDIDATES];
    int i, c, top_votes, tied_count = 0;
    clock_t t0, t1;
    double elapsed_ms;

    for (c = 0; c <= MAX_CANDIDATES; c++) {
        count[c] = 0;
    }

    t0 = clock();

    for (i = 0; i < num_votes; i++) {
        int v = ballots[i];
        if (v >= 1 && v <= num_candidates) {
            count[v]++;
            valid_votes++;
        } else {
            invalid_votes++;
        }
    }

    for (c = 1; c <= num_candidates; c++) {
        results[c - 1].candidate_id = c;
        results[c - 1].votes = count[c];
    }

    for (i = 1; i < num_candidates; i++) {
        CandidateResult key = results[i];
        int j = i - 1;
        while (j >= 0 && results[j].votes < key.votes) {
            results[j + 1] = results[j];
            j--;
        }
        results[j + 1] = key;
    }

    t1 = clock();
    elapsed_ms = ((double)(t1 - t0) / CLOCKS_PER_SEC) * 1000.0;

    printf("=== %s ===\n", label);
    printf("Total Ballots: %d | Valid Votes: %d | Invalid Votes: %d | Time: %.4f ms\n",
           num_votes, valid_votes, invalid_votes, elapsed_ms);

    if (verbose) {
        printf("Candidate Standings (Descending):\n");
        for (i = 0; i < num_candidates; i++) {
            double pct = valid_votes > 0 ? (100.0 * results[i].votes / valid_votes) : 0.0;
            printf("  Rank %d: Candidate #%d -> %d votes (%.2f%%)\n",
                   i + 1, results[i].candidate_id, results[i].votes, pct);
        }
    }

    if (valid_votes == 0) {
        printf("Outcome: No valid votes cast.\n\n");
        return;
    }

    top_votes = results[0].votes;
    for (i = 0; i < num_candidates; i++) {
        if (results[i].votes == top_votes) {
            tied_count++;
        } else {
            break;
        }
    }

    if (tied_count > 1) {
        printf("Outcome: TIE between %d candidates with %d votes each (", tied_count, top_votes);
        for (i = 0; i < tied_count; i++) {
            printf("Candidate #%d%s", results[i].candidate_id, (i < tied_count - 1) ? ", " : "");
        }
        printf(")\n\n");
    } else {
        printf("Outcome: WINNER is Candidate #%d with %d votes!\n\n",
               results[0].candidate_id, results[0].votes);
    }
}

int main(void)
{
    int hand_ballots[15] = {1, 3, 0, 2, 1, 5, 3, 4, -2, 1, 3, 2, 1, 3, 99};
    int hand_n = 15;
    int n_5k = 5000;
    int n_500k = 500000;
    int *ballots_5k;
    int *ballots_500k;
    int i;

    run_election("Hand-Verified Trial (4 Candidates, 15 Ballots, Tie + Invalids)",
                 4, hand_ballots, hand_n, 1);

    srand(707);
    ballots_5k = (int *)malloc(n_5k * sizeof(int));
    if (!ballots_5k) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    for (i = 0; i < n_5k; i++) {
        ballots_5k[i] = (rand() % 16) - 1;
    }
    run_election("Benchmark 1: 5,000 Votes (12 Candidates)", 12, ballots_5k, n_5k, 1);
    free(ballots_5k);

    ballots_500k = (int *)malloc(n_500k * sizeof(int));
    if (!ballots_500k) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    for (i = 0; i < n_500k; i++) {
        ballots_500k[i] = (rand() % 16) - 1;
    }
    run_election("Benchmark 2: 500,000 Votes (12 Candidates)", 12, ballots_500k, n_500k, 1);
    free(ballots_500k);

    return 0;
}
