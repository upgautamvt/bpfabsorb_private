//file: ModelFunctionNotInlined.user.c

#include <stdio.h>
#include <time.h>

long add(long x, int y) {
    return x + y;
}

long sub(long x, long y) {
    return add(x, -y);//no inline-optimized
}

long mul(long x, long y) {
    long result = 0;
    for (long i = 0; i < y; i++) { //couldn't vectorize loop
        result = add(result, x);//no inline-optimized, no vectorized
    }
    return result;
}

long div_int(long x, long y) {
    long result = 0;
    while (x >= y) { //couldn't vectorize loop, couldn't vectorized as no. of parameters can't be computed
        x = sub(x, y);//no inline-optimized
        result = add(result, 1);//no inline-optimized
    }
    return result;
}

long performComplexComputation(long iterations) {
    long result = 0;
    for (long i = 0; i < iterations; i++) { //couldn't vectorize loop
        result = add(result, 10);//no inline-optimized, no vectorized
        result = sub(result, 5);//no inline-optimized
        result = mul(result, 3);//no inline-optimized
        result = div_int(result, 2);//no inline-optimized
    }
    return result;
}

int main() {
    long result = 0;
    clock_t start, end;

    start = clock(); //not optimized, stmt. clobbers memory
    result = performComplexComputation(100);  //not optimized
    end = clock(); //not optimized, stmt. clobbers memory

    printf("Result: %ld\n", result);  //o
    printf("Execution time: %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC); //o

    return 0;
}

/*
Compile as:
Using these compiler flags trying to match eBPF programs behavior when they are launched from kernel

gcc -O2 -fno-inline -S -fopt-info-optimized -fopt-info-missed -o ModelFunctionNotInlined.s ModelFunctionNotInlined.user.c 2> gcc_compile_not_inlined.log
gcc -O2 -o ModelFunctionNotInlined ModelFunctionNotInlined.s

gcc -O2 -fno-inline -fopt-info-optimized -fopt-info-missed -o ModelFunctionNotInlined.s ModelFunctionNotInlined.user.c 2> gcc_compile_not_inlined.log


clang -O2 -fno-inline -S -Rpass=.* -Rpass-missed=.* -o ModelFunctionNotInlined.s ModelFunctionNotInlined.user.c 2> clang_compile_not_inlined.log
clang -O2 -o ModelFunctionNotInlined ModelFunctionNotInlined.s

*/
