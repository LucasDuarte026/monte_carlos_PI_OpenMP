parallel_reference:
	gcc ./src/parallel_reference.c -o ./bin/parallel_reference -fopenmp

sequencial_reference:
	gcc ./src/sequencial_reference.c -o ./bin/sequencial_reference -fopenmp
