all:
	echo Type for  parallel_reference sequencial_reference or optimized

parallel_reference:
	gcc -Wall ./src/parallel_reference.c -o ./bin/parallel_reference -fopenmp

sequencial_reference:
	gcc -Wall ./src/sequencial_reference.c -o ./bin/sequencial_reference -fopenmp


schedule_dynamic:
	gcc -Wall ./src/schedule_dynamic.c -o ./bin/schedule_dynamic -fopenmp

schedule_guided:
	gcc -Wall ./src/schedule_guided.c -o ./bin/schedule_guided -fopenmp

schedule_auto:
	gcc -Wall ./src/schedule_auto.c -o ./bin/schedule_auto -fopenmp

optimized:
	gcc -Wall ./src/optimized.c -o ./bin/optimized -fopenmp

optimized_simd:
# 	gcc -Wall ./src/optimized_simd.c -o ./bin/optimized_simd -fopenmp -march=native
# 	gcc -O3 -Wall ./src/optimized_simd.c -o ./bin/optimized_simd -fopenmp -march=native
	gcc -O2  -ftree-vectorize -finline-functions -fopenmp -mavx2 ./src/optimized_simd.c -o ./bin/optimized_simd 
# teste em maquina local
# 	gcc -O2  -ftree-vectorize -finline-functions -fopenmp -mavx512f ./src/optimized_simd.c -o ./bin/optimized_simd  