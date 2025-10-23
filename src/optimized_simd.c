#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h> // Necessário para uint64_t

 
/*
 * Usa os 53 bits superriores para preencher a mantissa de um double,
 * que é o método padrão para conversão de alta qualidade.
 * (uint64_t >> 11) * (1.0 / (2^53))
 * Referência: https://en.wikipedia.org/wiki/Double-precision_floating-point_format#Double-precision_examples
 */
static inline double hash_to_double(uint64_t hash)
{
    // 0x1.0p-53 é 1.0 / 9007199254740992.0
    return (hash >> 11) * 0x1.0p-53;
}

/**
 *
 * Esta é uma função "stateless" como se denomina em sua documentação.
 *  Ela gera o N-ésimo número independentemente de N-1, tornando-a 100% compatível com SIMD.
 *
 * Referênccia disponível em 'splitmix64': http://xorshift.di.unimi.it/splitmix64.c
 */
static inline uint64_t hash64(uint64_t i, uint64_t seed)
{
    uint64_t z = (i + seed); // Mistura o índice e a semente
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    z = z ^ (z >> 31);
    return z;
}
 

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

    long int samples = atol(argv[1]);
    unsigned long long hits = 0ULL;
    double start, end, wall_clock_time;

    start = omp_get_wtime();

    int local_num_threads = omp_get_num_procs();
    omp_set_num_threads(local_num_threads);

    const long int base_seed = (long int)time(NULL);

    /*
     * Usa-se duas sementes diferentes (derivadas da semente base)
     * para garantir que os fluxos de 'x' e 'y' sejam independentes, melhorando a aleatoriedade por diminuir correlação.
     */
    const uint64_t seed_x = hash64((uint64_t)base_seed, 0xDEADBEEFCAFEF00DULL);
    const uint64_t seed_y = hash64((uint64_t)base_seed, 0x123456789ABCDEF0ULL);

    /*
     * Região paralela:
     * - 'seed_x' e 'seed_y' são compartilhadas (e devem pois têm de ser um ponto de partida para o a geração dos números randomicos) e são constantes.
     * - 'samples' também é constante e a variavel de complexidade da aplicação.
     */
    #pragma omp parallel default(none) reduction(+ : hits) shared(samples, seed_x, seed_y)
    {
        /* Novo modo de cálculo dos números aleatórios:
         * O 'tid', 'rand_buffer' e 'srand48_r' foram removidos.
         * O estado agora é derivado diretamente do índice do loop 'i', viabilizando geração em vetorizada (pipeline) de números randômicos.
         */

        /* agora com SIMD
         * - "for": Divide o trabalho de 'samples' entre as threads (Ex: 4 threads).
         * - "simd": Instrui CADA thread a usar instruções vetoriais  AVX/SSE - Ambas disponíveis no Cluster da USP (plataforma onde será rodado o código).
         * - "schedule(static)": Mantido pela eficiência pela natureza homogênea de carga de trabalho da aplicação.
         */
        #pragma omp for simd schedule(static)
        for (unsigned long long i = 0; i < samples; ++i)
        {
            /*
             * Gera x e y usando o PRNG stateless (hash) com a função acima explicada.
             * O compilador vetoriza este bloco de forma muito eficiente.
             */
            double x = hash_to_double(hash64(i, seed_x));
            double y = hash_to_double(hash64(i, seed_y));

            const double radius_sq = x * x + y * y;

            /*
             * A versão branchless (sem if) é excelente para SIMD.
             * O compilador a converterá em instruções de "compare" vetoriais.
             */
            hits += (radius_sq <= 1.0);
        }
    }

    end = omp_get_wtime();
    const long double pi = 4.0L * ((long double)hits / samples);
    printf("Estimativa de PI = %.12Lf\n", pi);
    wall_clock_time = end - start;
    printf("Tempo de execução: %f segundos\n", wall_clock_time);
    return 0;
}