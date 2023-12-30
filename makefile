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

# 编译器自动向量化和优化选项
OPT_CFLAGS=-g -Wall -ftree-vectorize -march=native
ifeq ($(CC),gcc)
	OPT_CFLAGS+=-O1
else ifeq ($(CC),icx)
	OPT_CFLAGS+=-O1
endif

# OpenMP并行化
LDFLAGS=-fopenmp

SRCS=dgesv.c timer.c main.c test.c

OBJS=$(SRCS:.c=.o)

.PHONY: all clean memcheck advisor vtune

all: $(TARGETS)

dgesv: dgesv.o timer.o main.o
	$(CC) -g -Wall -o $@ $^ $(LDLIBS)
test: dgesv.o timer.o test.o
	$(CC) -g -Wall -o $@ $^ $(LDLIBS)
memcheck:dgesv 
	valgrind --leak-check=full --track-origins=yes ./dgesv 1024

dgesv-opt: dgesv.o timer.o main.o
	$(CC) $(OPT_CFLAGS) -o $@ $^ $(LDLIBS) $(LDFLAGS)

test-opt: dgesv.o timer.o test.o
	$(CC) $(OPT_CFLAGS) -o $@ $^ $(LDLIBS) $(LDFLAGS)

advisor: dgesv-opt
	advixe-cl --collect survey --project-dir=./advisor -- ./dgesv-opt 1024

vtune: dgesv-opt
	vtune -collect hotspots -- ./dgesv-opt 1024

clean:
	$(RM) $(TARGETS) *.o *~

