/* ============================================================
 * main.c — Programa principal: Sistema de Banco de Dados de Alunos
 *
 * Demonstra o uso das estruturas de dados implementadas e
 * executa o benchmark completo, gerando o arquivo CSV de
 * resultados para analise.
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "student.h"
#include "dynamic_list.h"
#include "static_list.h"
#include "sorting.h"
#include "benchmark.h"

/* ------------------------------------------------------------ *
 *  Demo interativa: mostra funcionalidades das listas
 * ------------------------------------------------------------ */
static void demo_lists(void) {
    printf("\n");
    printf("+======================================================+\n");
    printf("|       SISTEMA DE BANCO DE DADOS DE ALUNOS            |\n");
    printf("|       Demonstracao das Estruturas de Dados           |\n");
    printf("+======================================================+\n\n");

    /* ---- Lista Encadeada Dinamica ---- */
    printf(">>> LISTA ENCADEADA DINÂMICA\n");
    DynamicList dlist;
    dlist_init(&dlist);

    /* Gera 8 alunos aleatorios */
    Student *sample = student_generate_array(8, INPUT_RANDOM);
    dlist_load_from_array(&dlist, sample, 8);
    student_free_array(sample);

    printf("\n[1] Estado inicial (aleatorio):\n");
    dlist_print(&dlist);

    /* Insere um aluno manualmente */
    Student new_s = {9999, "Zara Cardoso", 9.8f, 20, "Ciencia da Computacao"};
    dlist_insert_back(&dlist, new_s);
    printf("\n[2] Apos inserir 'Zara Cardoso' (GPA 9.8):\n");
    dlist_print(&dlist);

    /* Ordena por GPA */
    dlist_merge_sort(&dlist, student_cmp_gpa);
    printf("\n[3] Apos Merge Sort por GPA (crescente):\n");
    dlist_print(&dlist);

    /* Busca por ID */
    DNode *found = dlist_search_by_id(&dlist, 9999);
    if (found) {
        printf("\n[4] Busca por ID 9999: encontrado!\n    ");
        student_print(&found->data);
    }

    /* Remocao */
    dlist_remove_by_id(&dlist, 9999);
    printf("\n[5] Apos remover ID 9999: %d alunos restantes.\n", dlist_size(&dlist));

    dlist_free(&dlist);

    /* ---- Lista Encadeada Estatica ---- */
    printf("\n>>> LISTA ENCADEADA ESTÁTICA\n");
    StaticList *slist = (StaticList *)malloc(sizeof(StaticList));
    slist_init(slist);

    sample = student_generate_array(6, INPUT_REVERSE);
    slist_load_from_array(slist, sample, 6);
    student_free_array(sample);

    printf("\n[1] Estado inicial (inversamente ordenado por GPA):\n");
    slist_print(slist);

    /* Ordena via Quick Sort (extrai array, ordena, recarrega) */
    Student *arr = slist_to_array(slist);
    arr_quick_sort(arr, slist_size(slist), student_cmp_gpa);
    slist_load_from_array(slist, arr, slist_size(slist));
    free(arr);

    printf("\n[2] Apos Quick Sort por GPA:\n");
    slist_print(slist);

    int idx = slist_search_by_name(slist, "Silva");
    if (idx != SLIST_NULL) {
        printf("\n[3] Primeiro aluno 'Silva' encontrado:\n    ");
        student_print(&slist->pool[idx].data);
    }

    slist_clear(slist);
    free(slist);
}

/* ------------------------------------------------------------ *
 *  Execucao do benchmark
 * ------------------------------------------------------------ */
static void run_benchmark(void) {
    printf("\n");
    printf("+======================================================+\n");
    printf("|              BENCHMARK DE DESEMPENHO                 |\n");
    printf("|  5 algoritmos x 2 estruturas x 3 N x 3 tipos        |\n");
    printf("|  %3d execucoes por experimento = 90 experimentos     |\n", BENCH_RUNS);
    printf("+======================================================+\n\n");

    /*
     * Total maximo de resultados:
     *   5 × 3 × 3 × 2 estruturas = 90
     */
    int max_results = ALGO_COUNT * N_SIZES * 3 * STRUCT_COUNT;
    BenchResult *results = (BenchResult *)malloc(max_results * sizeof(BenchResult));
    if (!results) {
        fprintf(stderr, "[ERRO] Falha ao alocar resultados\n");
        return;
    }

    int n_results = 0;
    bench_run_all(results, &n_results);

    /* Salva CSV */
    bench_save_csv(results, n_results, "results/resultados_benchmark.csv");

    /* Imprime tabela resumida */
    bench_print_summary(results, n_results);

    free(results);
}

/* ------------------------------------------------------------ *
 *  Main
 * ------------------------------------------------------------ */
int main(int argc, char *argv[]) {
    /* Semente para reprodutibilidade (pode ser alterada) */
    srand((unsigned int)time(NULL));

    int run_demo  = 1;
    int run_bench = 1;

    /* Argumentos opcionais: --only-demo  ou  --only-bench */
    if (argc > 1) {
        if (strcmp(argv[1], "--only-demo")  == 0) { run_bench = 0; }
        if (strcmp(argv[1], "--only-bench") == 0) { run_demo  = 0; }
    }

    if (run_demo)  demo_lists();
    if (run_bench) run_benchmark();

    printf("\n[FIM] Execucao concluida.\n");
    return 0;
}
