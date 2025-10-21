all:
	echo Type for  parallel_reference sequencial_reference or optimized

parallel_reference:
	gcc ./src/parallel_reference.c -o ./bin/parallel_reference -fopenmp

sequencial_reference:
	gcc ./src/sequencial_reference.c -o ./bin/sequencial_reference -fopenmp

optimized:
	gcc ./src/optimized.c -o ./bin/optimized -fopenmp

schedule_dynamic:
	gcc ./src/schedule_dynamic.c -o ./bin/schedule_dynamic -fopenmp

schedule_guided:
	gcc ./src/schedule_guided.c -o ./bin/schedule_guided -fopenmp

schedule_auto:
	gcc ./src/schedule_auto.c -o ./bin/schedule_auto -fopenmp