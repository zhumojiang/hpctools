# Module needed: openblas
# => module load openblas
# You need to uncomment the header for openblas

LDLIBS=-llapacke

dgesv: dgesv.o timer.o main.o

clean:
	$(RM) dgesv *.o *~
