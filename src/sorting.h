#ifndef SORTING_H
#define SORTING_H

/* ============================================================
 * sorting.h — Algoritmos de Ordenação
 * Autor: Ângelo
 *
 * Implementa Bubble Sort, Insertion Sort, Selection Sort,
 * Quick Sort e Merge Sort para:
 *   - Arrays de Student (usado pela lista estática)
 *   - Listas encadeadas dinâmicas (DynamicList)
 *
 * Todos os algoritmos recebem um comparador (CmpFunc) e
 * retornam a contagem de comparações realizadas.
 * ============================================================ */

#include "student.h"
#include "dynamic_list.h"

/* ============================================================
 *  ORDENAÇÃO EM ARRAY (para lista estática e testes gerais)
 * ============================================================ */

/* Bubble Sort — O(n²) pior/médio, O(n) melhor (com flag) */
long arr_bubble_sort(Student *arr, int n, CmpFunc cmp);

/* Insertion Sort — O(n²) pior/médio, O(n) melhor */
long arr_insertion_sort(Student *arr, int n, CmpFunc cmp);

/* Selection Sort — O(n²) todos os casos */
long arr_selection_sort(Student *arr, int n, CmpFunc cmp);

/* Quick Sort — O(n log n) médio, O(n²) pior */
long arr_quick_sort(Student *arr, int n, CmpFunc cmp);

/* Merge Sort — O(n log n) todos os casos */
long arr_merge_sort(Student *arr, int n, CmpFunc cmp);

/* ============================================================
 *  ORDENAÇÃO EM LISTA ENCADEADA DINÂMICA
 * ============================================================ */

/* Bubble Sort na lista encadeada — troca dados entre nós */
long dlist_bubble_sort(DynamicList *list, CmpFunc cmp);

/* Insertion Sort na lista encadeada */
long dlist_insertion_sort(DynamicList *list, CmpFunc cmp);

/* Selection Sort na lista encadeada */
long dlist_selection_sort(DynamicList *list, CmpFunc cmp);

/* Quick Sort na lista encadeada */
long dlist_quick_sort(DynamicList *list, CmpFunc cmp);

/* Merge Sort na lista encadeada */
long dlist_merge_sort(DynamicList *list, CmpFunc cmp);

/* ============================================================
 *  Identificadores de algoritmos (para benchmark)
 * ============================================================ */
typedef enum {
    ALGO_BUBBLE    = 0,
    ALGO_INSERTION = 1,
    ALGO_SELECTION = 2,
    ALGO_QUICK     = 3,
    ALGO_MERGE     = 4,
    ALGO_COUNT     = 5
} AlgoID;

const char *algo_name(AlgoID id);

#endif /* SORTING_H */
