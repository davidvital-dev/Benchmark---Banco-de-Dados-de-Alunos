/* ============================================================
 * benchmark.c — Implementacao do modulo de benchmark
 * Autor: Dorian
 * ============================================================ */

#define _POSIX_C_SOURCE 200809L

#include "benchmark.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#ifdef _WIN32
#include <windows.h>
#endif

/* ------------------------------------------------------------ *
 *  Utilitarios de tempo
 * ------------------------------------------------------------ */
long long bench_now_ns(void) {
#ifdef _WIN32
    static LARGE_INTEGER freq;
    static int initialized = 0;
    LARGE_INTEGER counter;

    if (!initialized) {
        QueryPerformanceFrequency(&freq);
        initialized = 1;
    }
    QueryPerformanceCounter(&counter);
    return (long long)((counter.QuadPart * 1000000000LL) / freq.QuadPart);
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
#endif
}

const char *struct_name(StructID id) {
    switch (id) {
        case STRUCT_DYNAMIC: return "Lista_Dinamica";
        case STRUCT_STATIC:  return "Lista_Estatica";
        default:             return "Unknown";
    }
}

/* ------------------------------------------------------------ *
 *  Funcoes de ordenacao em array (estatico) via ponteiro
 * ------------------------------------------------------------ */
typedef long (*ArrSortFn)(Student *, int, CmpFunc);
typedef long (*DynSortFn)(DynamicList *, CmpFunc);

static ArrSortFn arr_sort_fns[ALGO_COUNT] = {
    arr_bubble_sort,
    arr_insertion_sort,
    arr_selection_sort,
    arr_quick_sort,
    arr_merge_sort
};

static DynSortFn dyn_sort_fns[ALGO_COUNT] = {
    dlist_bubble_sort,
    dlist_insertion_sort,
    dlist_selection_sort,
    dlist_quick_sort,
    dlist_merge_sort
};

/* ------------------------------------------------------------ *
 *  Execucao de um unico experimento — Lista Dinamica
 * ------------------------------------------------------------ */
static BenchResult run_dynamic(AlgoID algo, int n, InputType itype,
                                CmpFunc cmp) {
    BenchResult res;
    res.algo        = algo;
    res.structure   = STRUCT_DYNAMIC;
    res.n           = n;
    res.input_type  = itype;
    res.avg_time_ns     = 0.0;
    res.min_time_ns     = DBL_MAX;
    res.max_time_ns     = 0.0;
    res.avg_comparisons = 0.0;

    DynamicList list;
    dlist_init(&list);

    long long total_ns   = 0;
    BenchCmpAcc total_cmps = 0;

    for (int run = 0; run < BENCH_RUNS; run++) {
        /* Gera dados novos a cada rodada */
        Student *data = student_generate_array(n, itype);

        /* Carrega na lista */
        dlist_load_from_array(&list, data, n);
        student_free_array(data);

        /* ---- Medicao ---- */
        long long t0 = bench_now_ns();
        long cmps = dyn_sort_fns[algo](&list, cmp);
        long long t1 = bench_now_ns();
        /* ------------------ */

        long long elapsed = t1 - t0;
        total_ns   += elapsed;
        total_cmps += (BenchCmpAcc)cmps;

        if ((double)elapsed < res.min_time_ns) res.min_time_ns = (double)elapsed;
        if ((double)elapsed > res.max_time_ns) res.max_time_ns = (double)elapsed;
    }

    res.avg_time_ns     = (double)total_ns   / BENCH_RUNS;
    res.avg_comparisons = (double)total_cmps / BENCH_RUNS;

    dlist_free(&list);
    return res;
}

/* ------------------------------------------------------------ *
 *  Execucao de um unico experimento — Lista Estatica
 * ------------------------------------------------------------ */
static BenchResult run_static(AlgoID algo, int n, InputType itype,
                               CmpFunc cmp) {
    BenchResult res;
    res.algo        = algo;
    res.structure   = STRUCT_STATIC;
    res.n           = n;
    res.input_type  = itype;
    res.avg_time_ns     = 0.0;
    res.min_time_ns     = DBL_MAX;
    res.max_time_ns     = 0.0;
    res.avg_comparisons = 0.0;

    /* Aloca estaticamente a lista (grande! fica na heap do benchmark) */
    StaticList *slist = (StaticList *)malloc(sizeof(StaticList));
    if (!slist) {
        fprintf(stderr, "[ERRO] Falha ao alocar StaticList\n");
        exit(EXIT_FAILURE);
    }
    slist_init(slist);

    /* Buffer de trabalho para o array (a lista estatica ordena via array) */
    Student *work = (Student *)malloc(n * sizeof(Student));
    if (!work) { free(slist); exit(EXIT_FAILURE); }

    long long total_ns   = 0;
    BenchCmpAcc total_cmps = 0;

    for (int run = 0; run < BENCH_RUNS; run++) {
        Student *data = student_generate_array(n, itype);

        /* Carrega na lista estatica e exporta para array de trabalho */
        slist_load_from_array(slist, data, n);
        student_free_array(data);

        /* Exporta para array */
        Student *tmp = slist_to_array(slist);
        memcpy(work, tmp, n * sizeof(Student));
        free(tmp);

        /* ---- Medicao (ordenacao no array, que e o que a lista estatica usa) */
        long long t0 = bench_now_ns();
        long cmps = arr_sort_fns[algo](work, n, cmp);
        long long t1 = bench_now_ns();
        /* -------------------------------------------------------------------- */

        long long elapsed = t1 - t0;
        total_ns   += elapsed;
        total_cmps += (BenchCmpAcc)cmps;

        if ((double)elapsed < res.min_time_ns) res.min_time_ns = (double)elapsed;
        if ((double)elapsed > res.max_time_ns) res.max_time_ns = (double)elapsed;
    }

    res.avg_time_ns     = (double)total_ns   / BENCH_RUNS;
    res.avg_comparisons = (double)total_cmps / BENCH_RUNS;

    free(work);
    free(slist);
    return res;
}

/* ------------------------------------------------------------ *
 *  Execucao de todos os experimentos
 * ------------------------------------------------------------ */
void bench_run_all(BenchResult *results, int *n_results) {
    *n_results = 0;

    /*
     * Total de combinacoes:
     *   5 algoritmos x 2 estruturas x 3 tamanhos x 3 tipos = 90 experimentos
     */
    int idx = 0;

    InputType input_types[3] = {INPUT_RANDOM, INPUT_SORTED, INPUT_REVERSE};
    CmpFunc   cmp = student_get_cmp(SORT_BY_GPA); /* ordena por GPA */

    for (int a = 0; a < ALGO_COUNT; a++) {
        for (int s = 0; s < N_SIZES; s++) {
            int n = BENCH_SIZES[s];
            for (int t = 0; t < 3; t++) {
                InputType itype = input_types[t];

                printf("  [%2d/90] %-14s | %-20s | N=%-6d | %-24s ...",
                       idx + 1,
                       algo_name((AlgoID)a),
                       struct_name(STRUCT_DYNAMIC),
                       n,
                       input_type_name(itype));
                fflush(stdout);

                results[idx++] = run_dynamic((AlgoID)a, n, itype, cmp);
                printf(" OK (%.1f us)\n",
                       results[idx-1].avg_time_ns / 1000.0);

                printf("  [%2d/90] %-14s | %-20s | N=%-6d | %-24s ...",
                       idx + 1,
                       algo_name((AlgoID)a),
                       struct_name(STRUCT_STATIC),
                       n,
                       input_type_name(itype));
                fflush(stdout);

                results[idx++] = run_static((AlgoID)a, n, itype, cmp);
                printf(" OK (%.1f us)\n",
                       results[idx-1].avg_time_ns / 1000.0);
            }
        }
    }

    *n_results = idx;
}

/* ------------------------------------------------------------ *
 *  Exportacao CSV
 * ------------------------------------------------------------ */
void bench_save_csv(const BenchResult *results, int n_results,
                    const char *filepath) {
    FILE *f = fopen(filepath, "w");
    if (!f) {
        fprintf(stderr, "[ERRO] Nao foi possivel criar '%s'\n", filepath);
        return;
    }

    /* Cabecalho */
    fprintf(f, "Algoritmo,Estrutura,N,Tipo_Entrada,"
               "Tempo_Medio_ns,Tempo_Medio_us,Tempo_Medio_ms,"
               "Tempo_Min_ns,Tempo_Max_ns,Comparacoes_Medias\n");

    for (int i = 0; i < n_results; i++) {
        const BenchResult *r = &results[i];
        fprintf(f, "%s,%s,%d,%s,%.2f,%.4f,%.6f,%.2f,%.2f,%.2f\n",
                algo_name(r->algo),
                struct_name(r->structure),
                r->n,
                input_type_name(r->input_type),
                r->avg_time_ns,
                r->avg_time_ns / 1000.0,
                r->avg_time_ns / 1000000.0,
                r->min_time_ns,
                r->max_time_ns,
                r->avg_comparisons);
    }

    fclose(f);
    printf("\n[OK] Resultados salvos em '%s'\n", filepath);
}

/* ------------------------------------------------------------ *
 *  Impressao resumida no terminal
 * ------------------------------------------------------------ */
void bench_print_summary(const BenchResult *results, int n_results) {
    printf("\n");
    printf("%-14s %-20s %7s %-24s %12s %14s\n",
           "Algoritmo", "Estrutura", "N", "Tipo",
           "Tempo(us)", "Comparacoes");
    printf("%-14s %-20s %7s %-24s %12s %14s\n",
           "--------------", "--------------------", "-------",
           "------------------------", "------------", "--------------");

    for (int i = 0; i < n_results; i++) {
        const BenchResult *r = &results[i];
        printf("%-14s %-20s %7d %-24s %12.2f %14.2f\n",
               algo_name(r->algo),
               struct_name(r->structure),
               r->n,
               input_type_name(r->input_type),
               r->avg_time_ns / 1000.0,
               r->avg_comparisons);
    }
}
