# Default Lapacke: Openblas at CESGA
LDLIBS=-lopenblas

# Other systems (my Debian boxes, for example)
#LDLIBS=-llapacke

# Intel MKL at CESGA
# Module needed: imkl
# => module load openblas
# LDLIBS for intel compiler: icx (module needed: intel)
# Just invoke make like this: make CC=icx
#LDLIBS=-qmkl=sequential -lmkl_intel_lp64


TARGETS=dgesv test


CC=gcc


SRCS=dgesv.c timer.c main.c test.c


OBJS=$(SRCS:.c=.o)


all: $(TARGETS)


dgesv: dgesv.o timer.o main.o
	$(CC) -o $@ $^ $(LDLIBS)


test: dgesv.o timer.o test.o
	$(CC) -o $@ $^ $(LDLIBS)


clean:
	$(RM) $(TARGETS) *.o *~
