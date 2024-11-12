//file: ModelFunctionInlined.user.c

#include <stdio.h>
#include <time.h>

static inline __attribute__((always_inline)) long add(long x, int y) {
    return x + y; //not vectorized
}

static inline __attribute__((always_inline)) long sub(long x, long y) {
    return add(x, -y);//o
}

static inline __attribute__((always_inline)) long mul(long x, long y) {
    long result = 0;
    for (long i = 0; i < y; i++) {//o loop with 3 iterations
        result = add(result, x);//o
    }
    return result;
}

static inline __attribute__((always_inline)) long div_int(long x, long y) {
    long result = 0;
    while (x >= y) {
        x = sub(x, y);//o
        result = add(result, 1);//o
    }
    return result;
}

static inline __attribute__((always_inline)) long performComplexComputation(long iterations) {
    long result = 0;
    for (long i = 0; i < iterations; i++) { //couldn't vectorize loop
        result = add(result, 10);//o
        result = sub(result, 5);//o
        result = mul(result, 3);//o
        result = div_int(result, 2);//o
    }
    return result;
}

int main() {
    long result = 0;
    clock_t start, end;

    start = clock();//not optimized, stmt. clobbers memory
    result = performComplexComputation(100);  //optimized
    end = clock();//not optimized, stmt. clobbers memory

    printf("Result: %ld\n", result);  //o
    printf("Execution time: %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC); //o

    return 0;
}


/*
Compile as:
gcc -O2 -fno-inline -S -fopt-info-optimized -fopt-info-missed -o ModelFunctionInlined.s ModelFunctionInlined.user.c 2> gcc_compile_inlined.log
gcc -O2 -o ModelFunctionInlined ModelFunctionInlined.s

gcc -O2 -fno-inline -fopt-info-optimized -fopt-info-missed -o ModelFunctionInlined.s ModelFunctionInlined.user.c 2> gcc_compile_inlined.log

clang -O2 -fno-inline -S -Rpass=.* -Rpass-missed=.* -o ModelFunctionInlined.s ModelFunctionInlined.user.c 2> clang_compile_inlined.log
gcc -O2 -o ModelFunctionInlined ModelFunctionInlined.s

Actual test
    upgautam@UddhavPc:~/CLionProjects/bpfabsorb/bpf-progs/filtering$ ./ModelFunctionNotInlined
    Result: 58
    Execution time: 43.606462 seconds
    upgautam@UddhavPc:~/CLionProjects/bpfabsorb/bpf-progs/filtering$ ./ModelFunctionInlined
    Result: 58
    Execution time: 0.000002 seconds


Abstract
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
