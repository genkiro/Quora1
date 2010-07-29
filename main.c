/*
 * File:   main.c
 * Author: RonnyWijaya
 *
 * Created on June 3, 2010, 2:43 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/*
 * Test Cases:
7 8
2 0 0 0 0 0 0
0 0 0 0 0 0 0
0 0 0 0 0 0 0
0 0 0 0 0 0 0
0 0 0 0 0 0 0
0 0 0 0 0 0 0
0 0 0 0 0 0 0
3 0 0 0 0 1 1
 * Answer = 301716 ways
 * With gcc -O3, it took about one minute
 *
6 6
2 0 0 0 0 0
0 0 0 0 0 0
0 0 0 0 0 0
0 0 0 0 0 0
0 0 0 0 0 0
3 0 0 0 1 1
 * Answer = 419
 *
4 3
2 0 0 0
0 0 0 0
0 0 3 1
 * Answer = 2
 *
 * Short explanation of the algorithm:
 * It is a recursive depth first search with separate visited logs for every recursion call.
 * A simple coverage checking is implemented to reduce the redundancy.
 * It checks whether we have separated zeros in every recursive call.
 * When we have separated zeros, we skip that route.
 *
 * Assumption:
 * No fault input. Enough memory.
 *
 */

void coverageCheck (int * stepsTracker, int startrow, int startcol, int nrow, int ncol) {
    // a DFS from a zero element. When the function is finished, we should
    // be able to set all of the tracker's elements into ones.
    // If it fails, it indicates that some zeros are separated.
    // Separated means that they cannot be connected with any available unoccupied route.

    *(stepsTracker + startrow*ncol + startcol) = 1;

    if (startrow + 1 < nrow && ( *(stepsTracker + (startrow+1)*ncol + startcol) == 0 ) ) {
        coverageCheck(stepsTracker, startrow + 1, startcol, nrow, ncol);
    }

    if (startrow - 1 >= 0   && ( *(stepsTracker + (startrow-1)*ncol + startcol) == 0 ) ) {
        coverageCheck(stepsTracker, startrow - 1, startcol, nrow, ncol);
    }

    if (startcol + 1 < ncol && ( *(stepsTracker + startrow*ncol + startcol+1) == 0 ) ) {
        coverageCheck(stepsTracker, startrow, startcol + 1, nrow, ncol);
    }
    if (startcol - 1 >= 0   && ( *(stepsTracker + startrow*ncol + startcol-1) == 0 ) ) {
        coverageCheck(stepsTracker, startrow, startcol - 1, nrow, ncol);
    }

}

void quora1 (int * stepsTracker, int nrow, int ncol, int startrow, int startcol, int endrow, int endcol, int nsteps, int nroomsownd, int * nways) {

    if (startrow == endrow && startcol == endcol) { // Has it reached the end?
        nsteps++;
        if (nsteps == nroomsownd) { // Are they all covered?
            (*nways)++;
            return;
        } else {
            return;
        }
    }

    nsteps++;
    int i;
    int j;
    int newStepsTracker[nrow][ncol]; // Idea: Replace these trackers
    int    rangeTracker[nrow][ncol]; //       with long integer bitvectors

    // newStepsTracker (visited logs) for each recursion call
    for (i = 0; i < nrow; i++) {
        for (j = 0; j < ncol; j++) {
            newStepsTracker[i][j] = *(stepsTracker + i*ncol + j);
            rangeTracker[i][j] = *(stepsTracker + i*ncol + j);
        }
    }
    newStepsTracker [startrow][startcol] = 1;

    // DFS from a zero that is the neighbor of the starting point
    // When we have separated zeros, do not bother to continue the exploration
    if (newStepsTracker [startrow+1][startcol] == 0) {
        coverageCheck ( (int *) rangeTracker, startrow+1, startcol, nrow, ncol );
        for (i = 0; i < nrow; i++) {
            for (j = 0; j < ncol; j++) {
                if (rangeTracker[i][j] == 0) { // Means zeros separated
                    return;
                }
            }
        }
    } else if (newStepsTracker [startrow-1][startcol] == 0) {
        coverageCheck ( (int *) rangeTracker, startrow-1, startcol, nrow, ncol );
        for (i = 0; i < nrow; i++) {
            for (j = 0; j < ncol; j++) {
                if (rangeTracker[i][j] == 0) { // Means zeros separated
                    return;
                }
            }
        }
    } else if (newStepsTracker [startrow][startcol+1] == 0) {
        coverageCheck ( (int *) rangeTracker, startrow, startcol+1, nrow, ncol );
        for (i = 0; i < nrow; i++) {
            for (j = 0; j < ncol; j++) {
                if (rangeTracker[i][j] == 0) { // Means zeros separated
                    return;
                }
            }
        }
    } else if (newStepsTracker [startrow][startcol-1] == 0) {
        coverageCheck ( (int *) rangeTracker, startrow, startcol-1, nrow, ncol );
        for (i = 0; i < nrow; i++) {
            for (j = 0; j < ncol; j++) {
                if (rangeTracker[i][j] == 0) { // Means zeros separated
                    return;
                }
            }
        }
    }

    int dirFlag = 0;
    /* 1 means go up only
     * 2 means go right only
     * 3 means go down only
     * 4 means go left only
     */
    // Check if any scooped neighbor exist
    if ( (newStepsTracker[startrow-2][startcol] == 1 || startrow-2 < 0) &&
            ( newStepsTracker[startrow-1][startcol-1] == 1 || startrow-1 < 0 ||
            startcol-1 < 0 || newStepsTracker[startrow-1][startcol+1] == 1 ||
            startcol+1 >= ncol) && newStepsTracker[startrow-1][startcol] == 0 &&
            startrow-1 >= 0) { // Check above
        if (startrow-1 != endrow || startcol != endcol) { // it is not the end point
            dirFlag = 1;
        }
    } else if ( (newStepsTracker[startrow][startcol+2] == 1 || startcol+2 >= ncol) &&
            (newStepsTracker[startrow-1][startcol+1] == 1 || startrow-1 < 0 ||
            startcol+1 >= ncol || newStepsTracker[startrow+1][startcol+1] == 1 ||
            startrow+1 >= nrow) && newStepsTracker[startrow][startcol+1] == 0 &&
            startcol+1 < ncol) { // Check right
        if (startrow != endrow || startcol+1 != endcol) {
            dirFlag = 2;
        }
    } else if ( (newStepsTracker[startrow+2][startcol] == 1 || startrow+2 >= nrow) &&
            (newStepsTracker[startrow+1][startcol+1] == 1 || startrow+1 >= nrow ||
            startcol+1 >= ncol || newStepsTracker[startrow+1][startcol-1] == 1 ||
            startcol-1 < 0) && newStepsTracker[startrow+1][startcol] == 0 &&
            startrow+1 < nrow) { // Check below
        if (startrow+1 != endrow || startcol != endcol) {
            dirFlag = 3;
        }
    } else if ( (newStepsTracker[startrow][startcol-2] == 1 || startcol-2 < 0) &&
            (newStepsTracker[startrow+1][startcol-1] == 1 || startrow+1 >= nrow ||
            startcol-1 < 0 || newStepsTracker[startrow-1][startcol-1] == 1 ||
            startrow-1 < 0) && newStepsTracker[startrow][startcol-1] == 0 &&
            startcol-1 >= 0) { // Check left
        if (startrow != endrow || startcol-1 != endcol) {
            dirFlag = 4;
        }
    }

    if (startrow+1 < nrow && (newStepsTracker [startrow+1][startcol] == 0) && (dirFlag == 0 || dirFlag == 3) ) { // below
        quora1 ((int *) newStepsTracker, nrow, ncol, startrow+1, startcol, endrow, endcol, nsteps, nroomsownd, nways);
    }

    if (startrow-1 >= 0 && (newStepsTracker [startrow-1][startcol] == 0) && (dirFlag == 0 || dirFlag == 1) ) { // above
        quora1 ((int *) newStepsTracker, nrow, ncol, startrow-1, startcol, endrow, endcol, nsteps, nroomsownd, nways);
    }

    if (startcol+1 < ncol && (newStepsTracker [startrow][startcol+1] == 0) && (dirFlag == 0 || dirFlag == 2) ) { // right
        quora1 ((int *) newStepsTracker, nrow, ncol, startrow, startcol+1, endrow, endcol, nsteps, nroomsownd, nways);
    }
    if (startcol-1 >= 0 && (newStepsTracker [startrow][startcol-1] == 0) && (dirFlag == 0 || dirFlag == 4) ) { // left
        quora1 ((int *) newStepsTracker, nrow, ncol, startrow, startcol-1, endrow, endcol, nsteps, nroomsownd, nways);
    }

}

int main(int argc, char** argv) {
    int nrow = 0;
    int ncol = 0;
    int startrow = 0;
    int startcol = 0;
    int endrow = 0;
    int endcol = 0;
    int i = 0;
    int j = 0;
    int temp = 0;
    int nways = 0;
    int reqsteps = 0;
    int nsteps = 0;
    int nroomsownd = 0;
    clock_t start;

    printf("Enter input: ");
    scanf("%d", &ncol);
    scanf("%d", &nrow);
    printf("ncol = %d\n", ncol);
    printf("nrow = %d\n", nrow);

    int arr[nrow][ncol];
    int stepsTracker[nrow][ncol];

    for (i = 0; i < nrow; i++) {
        for (j = 0; j < ncol; j++) {
            scanf("%d", &temp);
            arr[i][j] = temp;
            if (temp == 1) {
                stepsTracker[i][j] = 1;
            } else {
                nroomsownd++;
                stepsTracker[i][j] = 0;
            }
            if (temp == 2) {
                startrow = i;
                startcol = j;
            } else if (temp == 3) {
                endrow = i;
                endcol = j;
            } else if (temp == 0) {
                reqsteps++;
            }
        }
    }

    printf("startrow = %d\n", startrow);
    printf("startcol = %d\n", startcol);
    printf("endrow = %d\n", endrow);
    printf("endcol = %d\n", endcol);

    for (i = 0; i < nrow; i++) {
        for (j = 0; j < ncol; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }


    start = clock();

    quora1((int *) stepsTracker, nrow, ncol, startrow, startcol, endrow, endcol, nsteps, nroomsownd, &nways);

    printf("After function quora1 is finished, nways = %d\n", nways);
    printf("It took %d seconds.\n", (int) (clock() - start) / (int) CLOCKS_PER_SEC);
    return (EXIT_SUCCESS);
}
