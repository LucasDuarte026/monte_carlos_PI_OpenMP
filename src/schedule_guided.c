/* montecarloParallel.c
Implementação paralela para estimar pi por Monte Carlo com openMP.
Compilar: gcc montecarloParallel.c -o montecarloParallel
*/
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#define T 8 // Define o nr de threads a usar
int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("error: insert the N\n");
        printf(" - Examples\n $ ./bin/sequencial_reference 1000\n");
        return -1;
    }
    if (!atol(argv[1]))
    {
        printf("error: insert the N numerical\n");
        printf(" - Examples\n $ ./bin/sequencial_reference 1000\n");
        return -1;
    }
    long int n = atol(argv[1]);
    long int count = 0;
    double start, end, wall_clock_time;
    // Inicia a medição de tempo
    start = omp_get_wtime();

    // Definir uma seed unica para garantir o mesmo ponto de partida para geração dos
    // números aleatórios dentro de cada thread
    unsigned int seed = time(NULL);
    // Define o número de threads a serem usadas

    // 1. CADA thread terá seu próprio buffer e variável para o resultado.
    // Isso evita a condição de corrida.

    #pragma omp parallel num_threads(T)
    {
        struct drand48_data randBuffer;
        srand48_r(seed + omp_get_thread_num(), &randBuffer);

        #pragma omp for reduction(+ : count) schedule(guided) // schedule(dynamic) 
        for (long int i = 0; i < n; ++i)
        {
            double x, y;
            // A função armazena o resultado em 'x' e 'y'.
            drand48_r(&randBuffer, &x); // Gera número aleatório e armazena em x
            drand48_r(&randBuffer, &y); // Gera número aleatório e armazena em y
            // Verifica se o ponto (x,y) está dentro do círculo unitário
            if (x * x + y * y <= 1.0)
            {
                count++;
            }
        }
    }

    // Finaliza a medição de tempo
    end = omp_get_wtime();
    // Calcula a estimativa de Pi
    long double pi = 4.0L * ((long double)count / n);
    printf("Estimativa de PI = %.9Lf\n", pi);
    wall_clock_time = end - start;
    printf("Tempo de execução: %f segundos\n", wall_clock_time);
    return 0;
}
