# Module needed: openblas
# => module load openblas
# You need to uncomment the header for openblas
dgesv_gcc: dgesv.c
	gcc     dgesv.c  -lopenblas -o dgesv_gcc

# Modules needed: intel imkl
# => module load intel imkl
# You need to uncomment the header for mkl
dgesv_icc: dgesv.c
	icc     dgesv.c  -mkl -o dgesv_icc
