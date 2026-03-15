#ifndef BENCHMARK_H
#define BENCHMARK_H

#define _POSIX_C_SOURCE 200809L

/* ============================================================
 * benchmark.h — Medição de desempenho dos algoritmos
 * Autor: Dorian
 *
 * Executa cada algoritmo BENCH_RUNS vezes por cenário e
 * salva os resultados em CSV para análise posterior.
 * ============================================================ */

#include "student.h"
#include "sorting.h"
#include "dynamic_list.h"
#include "static_list.h"
#include <time.h>
#include <stdint.h>

/* Número de repetições por experimento */
#define BENCH_RUNS 100

/* Tamanhos de entrada a testar */
#define N_SIZES 3
static const int BENCH_SIZES[N_SIZES] = {100, 1000, 10000};

/* Estruturas de dados */
typedef enum {
    STRUCT_DYNAMIC = 0,
    STRUCT_STATIC  = 1,
    STRUCT_COUNT   = 2
} StructID;

const char *struct_name(StructID id);

/*
 * Tipo para acumular comparacoes no benchmark sem risco de overflow
 * quando somamos varias execucoes (BENCH_RUNS).
 */
typedef int64_t BenchCmpAcc;

/* ------------------------------------------------------------ *
 *  Resultado de um único experimento
 * ------------------------------------------------------------ */
typedef struct {
    AlgoID    algo;
    StructID  structure;
    int       n;
    InputType input_type;
    double    avg_time_ns;    /* Tempo médio em nanossegundos */
    double    min_time_ns;
    double    max_time_ns;
    double    avg_comparisons;
} BenchResult;

/* ------------------------------------------------------------ *
 *  Protótipos
 * ------------------------------------------------------------ */

/* Retorna tempo atual em nanossegundos */
long long bench_now_ns(void);

/* Executa todos os experimentos e preenche results[] */
void bench_run_all(BenchResult *results, int *n_results);

/* Salva resultados em arquivo CSV */
void bench_save_csv(const BenchResult *results, int n_results,
                    const char *filepath);

/* Imprime tabela resumida no terminal */
void bench_print_summary(const BenchResult *results, int n_results);

#endif /* BENCHMARK_H */
