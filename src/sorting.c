/* ============================================================
 * sorting.c — Implementacao dos Algoritmos de Ordenacao
 * Autor: Ângelo
 * ============================================================ */

#include "sorting.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ============================================================
 *  Utilitarios internos
 * ============================================================ */
static inline void swap_students(Student *a, Student *b) {
    Student tmp = *a;
    *a = *b;
    *b = tmp;
}

const char *algo_name(AlgoID id) {
    switch (id) {
        case ALGO_BUBBLE:    return "BubbleSort";
        case ALGO_INSERTION: return "InsertionSort";
        case ALGO_SELECTION: return "SelectionSort";
        case ALGO_QUICK:     return "QuickSort";
        case ALGO_MERGE:     return "MergeSort";
        default:             return "Unknown";
    }
}

/* ============================================================
 *  ORDENACÃO EM ARRAY
 * ============================================================ */

/* ---- Bubble Sort ------------------------------------------ */
long arr_bubble_sort(Student *arr, int n, CmpFunc cmp) {
    long comparisons = 0;
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            comparisons++;
            if (cmp(&arr[j], &arr[j + 1]) > 0) {
                swap_students(&arr[j], &arr[j + 1]);
                swapped = 1;
            }
        }
        if (!swapped) break; /* otimizacao: lista ja ordenada */
    }
    return comparisons;
}

/* ---- Insertion Sort --------------------------------------- */
long arr_insertion_sort(Student *arr, int n, CmpFunc cmp) {
    long comparisons = 0;
    for (int i = 1; i < n; i++) {
        Student key = arr[i];
        int j = i - 1;
        while (j >= 0) {
            comparisons++;
            if (cmp(&arr[j], &key) > 0) {
                arr[j + 1] = arr[j];
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
    }
    return comparisons;
}

/* ---- Selection Sort --------------------------------------- */
long arr_selection_sort(Student *arr, int n, CmpFunc cmp) {
    long comparisons = 0;
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            comparisons++;
            if (cmp(&arr[j], &arr[min_idx]) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap_students(&arr[i], &arr[min_idx]);
        }
    }
    return comparisons;
}

/* ---- Quick Sort ------------------------------------------- */
static long comparisons_qs; /* variavel global para contagem recursiva */

static int partition(Student *arr, int low, int high, CmpFunc cmp) {
    /* Pivot: mediana de tres (low, mid, high) para reduzir pior caso */
    int mid = low + (high - low) / 2;

    comparisons_qs++;
    if (cmp(&arr[low], &arr[mid]) > 0)    swap_students(&arr[low],  &arr[mid]);
    comparisons_qs++;
    if (cmp(&arr[low], &arr[high]) > 0)   swap_students(&arr[low],  &arr[high]);
    comparisons_qs++;
    if (cmp(&arr[mid], &arr[high]) > 0)   swap_students(&arr[mid],  &arr[high]);

    /* Pivot esta em arr[mid]; mover para arr[high-1] */
    swap_students(&arr[mid], &arr[high]);
    Student *pivot = &arr[high];

    int i = low - 1;
    for (int j = low; j < high; j++) {
        comparisons_qs++;
        if (cmp(&arr[j], pivot) <= 0) {
            i++;
            swap_students(&arr[i], &arr[j]);
        }
    }
    swap_students(&arr[i + 1], &arr[high]);
    return i + 1;
}

static void quick_sort_recursive(Student *arr, int low, int high, CmpFunc cmp) {
    if (low < high) {
        /* Insertion sort para subarrays pequenos */
        if (high - low < 10) {
            for (int i = low + 1; i <= high; i++) {
                Student key = arr[i];
                int j = i - 1;
                while (j >= low) {
                    comparisons_qs++;
                    if (cmp(&arr[j], &key) > 0) { arr[j + 1] = arr[j]; j--; }
                    else break;
                }
                arr[j + 1] = key;
            }
            return;
        }
        int pi = partition(arr, low, high, cmp);
        quick_sort_recursive(arr, low,    pi - 1, cmp);
        quick_sort_recursive(arr, pi + 1, high,   cmp);
    }
}

long arr_quick_sort(Student *arr, int n, CmpFunc cmp) {
    comparisons_qs = 0;
    if (n > 1) quick_sort_recursive(arr, 0, n - 1, cmp);
    return comparisons_qs;
}

/* ---- Merge Sort ------------------------------------------- */
static long comparisons_ms;

static void merge(Student *arr, int left, int mid, int right, CmpFunc cmp) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Student *L = (Student *)malloc(n1 * sizeof(Student));
    Student *R = (Student *)malloc(n2 * sizeof(Student));
    if (!L || !R) { free(L); free(R); return; }

    memcpy(L, arr + left,    n1 * sizeof(Student));
    memcpy(R, arr + mid + 1, n2 * sizeof(Student));

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        comparisons_ms++;
        if (cmp(&L[i], &R[j]) <= 0) { arr[k++] = L[i++]; }
        else                         { arr[k++] = R[j++]; }
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

static void merge_sort_recursive(Student *arr, int left, int right, CmpFunc cmp) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_recursive(arr, left,    mid,   cmp);
        merge_sort_recursive(arr, mid + 1, right, cmp);
        merge(arr, left, mid, right, cmp);
    }
}

long arr_merge_sort(Student *arr, int n, CmpFunc cmp) {
    comparisons_ms = 0;
    if (n > 1) merge_sort_recursive(arr, 0, n - 1, cmp);
    return comparisons_ms;
}

/* ============================================================
 *  ORDENACÃO EM LISTA ENCADEADA DINÂMICA
 *  Estrategia: troca de dados entre nos (nao reencadeia ponteiros)
 * ============================================================ */

/* ---- Bubble Sort na lista --------------------------------- */
long dlist_bubble_sort(DynamicList *list, CmpFunc cmp) {
    if (!list || list->size < 2) return 0;
    long comparisons = 0;
    int swapped;

    do {
        swapped = 0;
        DNode *cur = list->head;
        while (cur->next != NULL) {
            comparisons++;
            if (cmp(&cur->data, &cur->next->data) > 0) {
                /* Troca os dados (nao os ponteiros) */
                Student tmp  = cur->data;
                cur->data    = cur->next->data;
                cur->next->data = tmp;
                swapped = 1;
            }
            cur = cur->next;
        }
    } while (swapped);

    return comparisons;
}

/* ---- Insertion Sort na lista ------------------------------ */
long dlist_insertion_sort(DynamicList *list, CmpFunc cmp) {
    if (!list || list->size < 2) return 0;
    long comparisons = 0;

    DNode *sorted_end = list->head; /* ultimo no da parte ordenada */
    while (sorted_end->next != NULL) {
        DNode *key_node = sorted_end->next;
        Student key     = key_node->data;

        DNode *cur = list->head;
        while (cur != key_node) {
            comparisons++;
            if (cmp(&cur->data, &key) > 0) {
                /* Desloca dados para a direita */
                Student save = cur->data;
                DNode  *runner = cur->next;
                while (runner != key_node) {
                    Student tmp  = runner->data;
                    runner->data = save;
                    save         = tmp;
                    runner       = runner->next;
                }
                key_node->data = save;
                cur->data      = key;
                break;
            }
            if (cur->next == key_node) {
                /* key ja esta na posicao correta */
                break;
            }
            cur = cur->next;
        }
        sorted_end = sorted_end->next;
    }
    return comparisons;
}

/* ---- Selection Sort na lista ------------------------------ */
long dlist_selection_sort(DynamicList *list, CmpFunc cmp) {
    if (!list || list->size < 2) return 0;
    long comparisons = 0;

    DNode *outer = list->head;
    while (outer != NULL) {
        DNode *min_node = outer;
        DNode *inner    = outer->next;
        while (inner != NULL) {
            comparisons++;
            if (cmp(&inner->data, &min_node->data) < 0) {
                min_node = inner;
            }
            inner = inner->next;
        }
        if (min_node != outer) {
            Student tmp    = outer->data;
            outer->data    = min_node->data;
            min_node->data = tmp;
        }
        outer = outer->next;
    }
    return comparisons;
}

/* ---- Quick Sort recursivo na lista duplamente encadeada --- */
/*
 * Usa o ponteiro prev da lista duplamente encadeada para
 * navegar de volta ao no anterior ao pivot, tornando a
 * recursao simples e correta.
 * Pivot: ultimo no do subintervalo atual.
 */
static long dlist_qs_comps;

static DNode *dlist_partition_rec(DNode *low, DNode *high, CmpFunc cmp) {
    Student pivot = high->data;
    DNode  *i     = low->prev;

    for (DNode *j = low; j != high; j = j->next) {
        dlist_qs_comps++;
        if (cmp(&j->data, &pivot) <= 0) {
            i = (i == NULL) ? low : i->next;
            Student tmp = i->data;
            i->data     = j->data;
            j->data     = tmp;
        }
    }
    i = (i == NULL) ? low : i->next;
    Student tmp = i->data;
    i->data     = high->data;
    high->data  = tmp;
    return i;
}

static void dlist_qs_recursive(DNode *low, DNode *high, CmpFunc cmp) {
    if (high != NULL && low != high && low != high->next) {
        DNode *pivot = dlist_partition_rec(low, high, cmp);
        dlist_qs_recursive(low,          pivot->prev, cmp);
        dlist_qs_recursive(pivot->next,  high,        cmp);
    }
}

long dlist_quick_sort(DynamicList *list, CmpFunc cmp) {
    if (!list || list->size < 2) return 0;
    dlist_qs_comps = 0;

    DNode *last = list->head;
    while (last->next != NULL) last = last->next;

    dlist_qs_recursive(list->head, last, cmp);
    return dlist_qs_comps;
}

/* ---- Merge Sort na lista ---------------------------------- */
static long dlist_ms_comps;

static DNode *dlist_get_middle(DNode *head) {
    DNode *slow = head;
    DNode *fast = head->next;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow; /* slow e o no do meio */
}

static DNode *dlist_merge_sorted(DNode *a, DNode *b, CmpFunc cmp) {
    if (!a) return b;
    if (!b) return a;

    dlist_ms_comps++;
    if (cmp(&a->data, &b->data) <= 0) {
        a->next = dlist_merge_sorted(a->next, b, cmp);
        return a;
    } else {
        b->next = dlist_merge_sorted(a, b->next, cmp);
        return b;
    }
}

static DNode *dlist_merge_sort_node(DNode *head, CmpFunc cmp) {
    if (!head || !head->next) return head;

    DNode *mid  = dlist_get_middle(head);
    DNode *half = mid->next;
    mid->next   = NULL;

    DNode *left  = dlist_merge_sort_node(head, cmp);
    DNode *right = dlist_merge_sort_node(half, cmp);

    return dlist_merge_sorted(left, right, cmp);
}

long dlist_merge_sort(DynamicList *list, CmpFunc cmp) {
    if (!list || list->size < 2) return 0;
    dlist_ms_comps = 0;
    list->head = dlist_merge_sort_node(list->head, cmp);
    return dlist_ms_comps;
}
