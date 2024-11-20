//file: ModelFunctionInlined.user.c

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

static inline __attribute__((always_inline)) int getValue() {
    return 10;
}

int main() {
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    int x = getValue();

    // This block will execute if the condition is true
    // Compiler knows during compiler optimization because of x coming from non-inlined function
    // it means anything inside the block will be eliminated as dead code
    if (x > 100) { //this is computed false during compilation
        printf("This gets eliminated inlined.\n");

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
    printf("Execution time (inlined): %lf seconds\n", elapsed);

    return 0;
}


/*
Compile as:
Using these compiler flags trying to match eBPF programs behavior when they are launched from kernel

gcc -O2 -S -fopt-info-optimized -fopt-info-missed -o ModelFunctionInlined.s ModelFunctionInlined.user.c 2> gcc_compile_inlined.log
gcc -O2 -g -o ModelFunctionInlined ModelFunctionInlined.s
objdump -d ModelFunctionInlined > objdump_inlined.txt

clang -O2 -S -Rpass=.* -Rpass-missed=.* -o ModelFunctionInlined.s ModelFunctionInlined.user.c 2> clang_compile_inlined.log
clang -O2 -o ModelFunctionInlined ModelFunctionInlined.s

*/
