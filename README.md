Task: Basic benchmarking of your dgesv implementation (Task #1, after possible modifications introduced in step #0)
-> benchmark both with gcc and icx
-> run multiple versions of your code, compiled using different optimization options/levels


Makefile code
# Default Lapacke: Openblas at CESGA
LDLIBS=-lopenblas

# DebianLapacke
#LDLIBS=-llapacke

# Intel MKL at CESGA
# Module needed: imkl
# => module load openblas
# LDLIBS for intel compiler: icx (module needed: intel)
# Just invoke make like this: make CC=icx
#LDLIBS=-qmkl=sequential -lmkl_intel_lp64

TARGETS=dgesv test

CC=gcc
#icx编译所有目标：make all CC=icx


# 编译器优化选项
OPT_CFLAGS=-g -Wall
ifeq ($(OPT_LEVEL),)
	OPT_LEVEL=-O1 # 默认优化级别1
endif
OPT_CFLAGS+=$(OPT_LEVEL)

# 自动向量化选项，可以在需要时启用
# VECTORIZE_FLAGS=-ftree-vectorize -march=native


# OpenMP并行化
LDFLAGS=-fopenmp

SRCS=dgesv.c timer.c main.c test.c

OBJS=$(SRCS:.c=.o)

.PHONY: all clean memcheck advisor vtune

all: $(TARGETS)

dgesv: dgesv.o timer.o main.o
	$(CC) -g -Wall -o $@ $^ $(LDLIBS)

.PHONY: gprof-analysis

gprof-analysis:
	for cc in gcc icx; do \
		for opt in O0 O1 O2 O3; do \
			$(MAKE) dgesv-opt CC=$$cc OPT_LEVEL=-$$opt; \
			./dgesv-opt 300; \
			gprof dgesv-opt gmon.out > analysis-$$cc-$$opt.txt; \
		done \
	done

test: dgesv.o timer.o test.o
	$(CC) -g -Wall -o $@ $^ $(LDLIBS)
memcheck:dgesv 
	valgrind --leak-check=full --track-origins=yes ./dgesv 1024

dgesv-opt: dgesv.o timer.o main.o
	$(CC) $(OPT_CFLAGS) -pg -o $@ $^ $(LDLIBS) 
test-opt: dgesv.o timer.o test.o
	$(CC) $(OPT_CFLAGS) -o $@ $^ $(LDLIBS)

advisor: dgesv-opt
	advixe-cl --collect survey --project-dir=./advisor -./dgesv-opt 1024

vtune: dgesv-opt
	vtune -collect hotspots -./dgesv-opt 1024

clean:
	$(RM) $(TARGETS) *.o *~ dgesv-opt test-opt


Code Analysis
1. Compiler optimization options
OPT_CFLAGS=-g -Wall
ifeq ($(OPT_LEVEL),)
	OPT_LEVEL=-O1 # Default optimization level 1
endif
OPT_CFLAGS+=$(OPT_LEVEL)


2. Gprof performance analysis
.PHONY: gprof-analysis

gprof-analysis:
    for cc in gcc icx; do \
        for opt in O0 O1 O2 O3; do \
            $(MAKE) dgesv-opt CC=$$cc OPT_LEVEL=-$$opt; \
            ./dgesv-opt 300; \
            gprof dgesv-opt gmon.out > analysis-$$cc-$$opt.txt; \
        done \
    done

This is a phony target used for performance analysis of the `dgesv-opt` program.
The script loops through both `gcc` and `icx` compilers and different optimization levels (`-O0` to `-O3`), compiles and runs the `dgesv-opt` program, and performs performance analysis using the `gprof` tool, saving the analysis results in the corresponding text files.


3. Compilation of `dgesv-opt` (optimized version)
dgesv-opt: dgesv.o timer.o main.o
    $(CC) $(OPT_CFLAGS) -pg -o $@ $^ $(LDLIBS)

This is a rule for compiling the `dgesv-opt` program. It uses the `$(OPT_CFLAGS)` flag, which includes optimizations and debug information.
`-pg` is necessary for `gprof` analysis and inserts specific profiling code during compilation.


Here is the performance analysis report for the `dgesv-opt` program:

Compilation environment:
Compiler: `icx`/`gcc`
Optimization level: `-O1`, `-O2`, `-O3`
Analysis tool: `gprof`

Summary:
The `my_dgesv` function accounts for the majority of the total runtime of the program (100.13%). This indicates that almost all computational resources are consumed by the `my_dgesv` function.
The cumulative runtime is 0.10 seconds, and the independent runtime of the `my_dgesv` function is also 0.10 seconds.
Since the number of function calls is not provided, the average call time of `my_dgesv` cannot be determined.

Analysis Conclusion:
1. Performance bottleneck: From the results, the `my_dgesv` function is a clear performance bottleneck. Optimizing this function may significantly improve the overall performance of the program.

2. Optimization suggestions:
   Code optimization: Perform a detailed analysis of the `my_dgesv` function internals to identify computationally intensive sections that can be optimized, such as loop structures, conditional statements, and algorithm efficiency.
   Algorithm improvement: Consider if there are more efficient algorithms available to achieve the functionality of `my_dgesv`.
   Parallel computation: If there are parallelizable computations within `my_dgesv`, consider using multi-threading or vectorization techniques.

3. Using other performance analysis tools: To obtain more detailed performance analysis, it is recommended to use other tools such as `perf` or `VTune`, especially in multi-core and multi-threaded environments.