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
	advixe-cl --collect survey --project-dir=./advisor -- ./dgesv-opt 1024

vtune: dgesv-opt
	vtune -collect hotspots -- ./dgesv-opt 1024

clean:
	$(RM) $(TARGETS) *.o *~ dgesv-opt test-opt


