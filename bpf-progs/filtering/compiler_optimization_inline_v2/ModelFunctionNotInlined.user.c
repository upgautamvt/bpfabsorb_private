//file: ModelFunctionNotInlined.user.c

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int getValue() {
    return 10;
}

int main() {
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    int x = getValue();

    for (long i = 0; i < 1000000000l; i++) {
        //the if block comparision is not calculated in compile time because it is not known during compile time because x is from non-inline function
        if (x > 100) {
            //No DCE this if body
            printf("Inside loop.\n");
      }
    }

    // This block will execute if the condition is true
    // although the if block below is always false, but compiler can't know during compiler optimization because of x coming from non-inlined function
    // it means anything inside the block can't be eliminated as dead code
    if (x > 100) {
        //DCE this whole if body
        printf("This gets not eliminated in non-inlined.\n");

        // Expensive matrix calculation
        int size = 100;
        double matrix[size][size];

        // Initialize matrix
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                matrix[i][j] = (double)(i * j);
            }
        }

        // Perform some computations
        double sum = 0.0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                sum += matrix[i][j];
            }
        }
        printf("Sum of matrix elements: %f\n", sum);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Execution time (not inlined): %lf seconds\n", elapsed);

    return 0;
}


/*
Compile as:
Using these compiler flags trying to match eBPF programs behavior when they are launched from kernel

gcc -O2 -fno-inline -fno-dce -S -fopt-info-optimized -fopt-info-missed -o ModelFunctionNotInlined.s ModelFunctionNotInlined.user.c 2> gcc_compile_not_inlined.log
gcc -O2 -g -o ModelFunctionNotInlined ModelFunctionNotInlined.s
objdump -d ModelFunctionNotInlined > objdump_notinlined.txt
*/
