//file: ModelFunctionInlined.user.c
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

inline long functionA(ComplexObject obj) {
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
gcc -O2 -flto -funroll-loops -o ModelFunctionInlined ModelFunctionInlined.user.c



upgautamvt@upg-vt-lab:~/CLionProjects/bpfabsorb/bpf-progs/filtering$ gcc -O2 -fno-lto -fno-inline -fno-unroll-loops -o ModelFunctionNotInlined ModelFunctionNotInlined.user.c
upgautamvt@upg-vt-lab:~/CLionProjects/bpfabsorb/bpf-progs/filtering$ gcc -O2 -flto -funroll-loops -o ModelFunctionInlined ModelFunctionInlined.user.c
upgautamvt@upg-vt-lab:~/CLionProjects/bpfabsorb/bpf-progs/filtering$ ./ModelFunctionInlined
Vaue: 149999985000000
Function time (inlined): 0.000001 seconds
upgautamvt@upg-vt-lab:~/CLionProjects/bpfabsorb/bpf-progs/filtering$ ./ModelFunctionNotInlined
Vaue: 149999985000000
Function time (inlined): 4.908199 seconds

Runtime improved by million-fold.


eBPF programs are limited in terms of optimization because they run in a restricted environment.
Since eBPF programs often involve indirect function calls, the compiler
cannot apply optimizations like inlining, constant folding, or loop unrolling. For example,
in the provided code, the function functionA behaves like an eBPF program with indirect calls,
preventing optimizations such as loop unrolling. In contrast, when you implement the same logic
directly in the kernel, you can pass full context to the function, allowing the compiler to apply
these optimizations. This is similar to how inlining and other optimizations work in kernel code,
where the compiler has full knowledge of the execution context. For instance, if the logic of
functionA were inside the kernel, the compiler could inline the function, unroll loops, and
fold constants for better performance. Thus, while eBPF offers flexibility, it sacrifices some
optimizations that can be achieved when the same logic is implemented directly in the kernel.
*/
