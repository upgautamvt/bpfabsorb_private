//file: ModelFunctionNotInlined.user.c
#include <stdio.h>
#include <time.h>

typedef struct {
    int x;
    int y;
    int z;
} ComplexObject;

long longFunction() {
    long dummy = 0;
    for (unsigned long long i = 0; i < 10000000000; i++) {
        dummy += i;
    }
    return dummy;
}

long functionA(ComplexObject obj) {
    long dummy = 0;
    for (unsigned long long i = 0; i < obj.x; i++) {
        dummy += i;
    }

    // Unrolling loop with a factor of 4
    unsigned long long i = 0;
    for (; i < obj.x / 4 * 4; i += 4) {
        dummy += i;
        dummy += i + 1;
        dummy += i + 2;
        dummy += i + 3;
    }

    // Handle remaining iterations (less than 4)
    for (; i < obj.x; i++) {
        dummy += i;
    }

    //constant folding
    if (obj.x == 10) {
        return 45;  // Precomputed sum of numbers from 0 to 9
    }
    for (unsigned long long i = 0; i < obj.x; i++) {
        dummy += i;
    }

    //dead code elimination if we don't consume the return value from longFunction
    longFunction();

    return dummy;
}

// Main function
int main() {
    long val = 0l;
    clock_t start, end;
    start = clock();  // Start the timer
    ComplexObject obj1 = {10000000, 0, 0};  // Increased x for multiple calls
    for (int i = 0; i < 1000; i++) {  // Call it many times
        val = functionA(obj1);
    }
    end = clock();
    printf("Vaue: %ld\n", val); //if we don't consume the return value O2 eliminates the whole function as deadcode.
    printf("Function time (inlined): %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    return 0;
}

/*
Compile as:
Using these compiler flags trying to match eBPF programs behavior when they are launched from kernel
gcc -O2 -fno-lto -fno-inline -fno-unroll-loops -o ModelFunctionNotInlined ModelFunctionNotInlined.user.c
*/
