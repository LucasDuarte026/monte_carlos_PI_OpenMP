all:
	echo Type for  parallel_reference sequencial_reference or optimized

parallel_reference:
	gcc ./src/parallel_reference.c -o ./bin/parallel_reference -fopenmp

sequencial_reference:
	gcc ./src/sequencial_reference.c -o ./bin/sequencial_reference -fopenmp

optimized:
	gcc ./src/optimized.c -o ./bin/optimized -fopenmp
