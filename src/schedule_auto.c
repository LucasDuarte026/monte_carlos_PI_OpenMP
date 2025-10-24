#include <omp.h>    // Funções de paralelismo e medição de tempo
#include <stdio.h>  // E/S padrão (printf)
#include <stdlib.h> // atol, srand48_r, drand48_r
#include <time.h>   // time() para gerar seeds distintas

/*
 * Versão paralela "optimized em auto" para estimar PI com Monte Carlo.
 * Mantém foco em baixa sobrecarga e uso direto das primitivas OpenMP.
 */

int main(int argc, char *argv[])
{
    /*
     * Garante que o usuário informou um valor para N.
     * Sem argumentos suficientes terminamos a execução com uma mensagem.
     */
    if (argc < 2)
    {
        printf("error: insert the N\n");
        printf(" - Examples\n $ ./bin/sequencial_reference 1000\n");
        return -1;
    }

    /*
     * atol converte a string do argumento para long int.
     * Se a conversão falhar, retorna 0; usamos isso para validar se N é numérico.
     */
    if (!atol(argv[1]))
    {
        printf("error: insert the N numerical\n");
        printf(" - Examples\n $ ./bin/sequencial_reference 1000\n");
        return -1;
    }

    /*
     * samples: quantidade de pontos a lançar; hits: contador de pontos dentro do círculo.
     */
    long int samples = atol(argv[1]);
    unsigned long long hits = 0ULL;
    double start, end, wall_clock_time;

    // Marca o início da medição de tempo (wall clock) com alta resolução.
    start = omp_get_wtime();

    // Ajusta o número de threads para igualar a contagem de processadores disponíveis.
    int local_num_threads = omp_get_num_procs() ;
    omp_set_num_threads(local_num_threads);

    /*
     * base_seed: ponto de partida comum para gerar seeds individuais por thread.
     * Utilizamos time(NULL) para garantir valores diferentes em execuções sucessivas.
     */
    long int base_seed = (long int)time(NULL);

    /*
     * Região paralela:
     * - default(none) obriga a especificar o tratamento (shared/private) de cada variável,
     *   ajudando a evitar condições de corrida por esquecimento.
     * - reduction soma os acertos de forma paralela, eliminando a necessidade de "atomic".
     */
    #pragma omp parallel default(none)  reduction(+ : hits) shared(samples,base_seed)
    {
        const int tid = omp_get_thread_num();
        struct drand48_data rand_buffer;

        /*
         * Calcula a seed específica da thread combinando:
         * - base_seed (tempo atual)
         * - multiplica (tid + 1) pelo multiplicador clássico 0x5DEECE66D da família rand48
         * - XOR para espalhar os bits e diminuir correlações entre threads.
         */
        long int thread_seed = base_seed ^ ((long int)(tid + 1) * 0x5DEECE66DL);

        /*
         * Ajusta a seed para garantir que seja positiva e não zero:
         * valores negativos podem produzir estados repetidos ou inválidos no rand48.
         */
        if (thread_seed <= 0)
        {
            thread_seed = labs(thread_seed) + 1L;
        }

        // Inicializa o estado reentrante do gerador para esta thread.
        srand48_r(thread_seed, &rand_buffer);

        /*
         * Divide o intervalo [0, samples) entre as threads.
         * schedule(auto) e não é tão eficiente pois cria um overload de tarefas
         */
        #pragma omp for schedule(auto)
        for (unsigned long long  i = 0; i < samples; ++i)
        {
            double x, y;
            // Gera duas coordenadas independentes no intervalo [0, 1).
            drand48_r(&rand_buffer, &x); // Gera número aleatório e armazena em x
            drand48_r(&rand_buffer, &y); // Gera número aleatório e armazena em y

            // Calcula o raio ao quadrado para verificar se (x, y) cai dentro do círculo unitário.
            const double radius_sq = x * x + y * y;

            /*
             * Versão branchless: (condição) retorna 1 ou 0; somamos diretamente ao acumulador.
             * Evita custo de branch e geralmente oferece melhor desempenho em laços apertados.
             */
            hits += (radius_sq <= 1.0); 
        }
    }

    // Captura o tempo final e calcula a duração total.
    end = omp_get_wtime();
    // Calcula a estimativa de Pi
    const long double pi = 4.0L * ((long double)hits / samples);
    printf("Estimativa de PI = %.12Lf\n", pi);
    wall_clock_time = end - start;
    printf("Tempo de execução: %f segundos\n", wall_clock_time);
    return 0;
}
